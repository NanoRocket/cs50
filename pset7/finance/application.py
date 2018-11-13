from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached, this gets fresh data every time
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/", methods=["GET", "POST"])
# "decorated" with @login_required (a function defined in helpers.py too). 
# That decorator ensures that, if a user tries to visit any of those routes, 
# he or she will first be redirected to login so as to log in.
@login_required  # user must be logged in to access route
def index():
    """displays an HTML table summarizing, for the user currently logged in, 
    which stocks the user owns, the numbers of shares owned, the current price 
    of each stock, and the total value of each holding (i.e., shares times price). 
    Also display the user’s current cash balance along with a grand total (i.e., 
    stocks' total value plus cash). """
    
    # Odds are you’ll want to execute multiple SELECTs. Depending on how 
    # you implement your table(s), you might find GROUP BY, HAVING, SUM, and/or WHERE of interest.
    
    # Odds are you’ll want to call lookup for each stock.
    if request.method == "GET":
        print(session["user_id"])
        print('---------------------------------------------------------------')
        
        # query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :username", username=session["user_id"])
        user_cash = rows[0]["cash"]
        
        # get stock information for user by slect stock_name, stock_symbol
        temprows = db.execute("SELECT stock_symbol, SUM(shares_owned) AS total_shares  \
                                FROM Portfolio WHERE id =:username \
                                GROUP BY stock_symbol ORDER BY stock_symbol ", username=session["user_id"])
        
        # append to retrieved stock dictionaries current price info
        # also go ahead and find total user value
        user_value = user_cash
        for stock in temprows:
            stock['current_price'] = lookup(stock["stock_symbol"])['price']
            stock['stock_name'] = lookup(stock["stock_symbol"])['name']
            stock['total_value'] = stock['total_shares'] * stock['current_price'] 
            user_value += stock['total_value']
        
        return render_template("index.html", usercash=usd(user_cash), 
                                stockinfo = temprows, uservalue = usd(user_value))
    
    if request.method == "POST":
        
        # query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :username", username=session["user_id"])
        user_cash = rows[0]["cash"]
        
        # add ash to above amount from form
        try:
            user_cash += float(request.form.get("add_cash"))
        except ValueError:
            return apology("Insert cash as decimal")
        db.execute("UPDATE users SET cash = :user_cash WHERE id = :username \
        ", username=session["user_id"], user_cash = user_cash)
                                
        return redirect(url_for("index"))                     

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
    
        # Require that a user input a stock’s symbol. Render an apology if the 
        # input is blank or the symbol does not exist (as per the return value of lookup).
        # ensure stock is entered and exists
        stock_symbol = request.form.get("buy_symbol")
        if not stock_symbol:
            return apology("Missing buy symbol!")
            
        # Require that a user input a number of shares. Render an apology if the input is not a positive integer.
        shares = int(request.form.get("numberofshares")) 
        if not shares or shares < 0:
            return apology("Input a positive number of shares")
        
            
        # using lookup from helpers.py to get the stock quote
        # look up provides dict with "price" , "name" , "symbol"
        company_quote = lookup(stock_symbol)
        
        # ensure stock is valid, else apology
        if not company_quote:
            return apology("Stock wasn't valid!")
            
        # Odds are you’ll want to SELECT how much cash the user currently has in users.
        rows = db.execute("SELECT cash FROM users WHERE id = :username", username=session["user_id"])
        user_cash = rows[0]["cash"]
        
        
        # Render an apology, without completing a purchase, if the user cannot afford the number of shares at the current price.
        transaction_price = company_quote["price"]*shares
        if transaction_price > user_cash:
            return apology("Not enough cash to do transaction")
        
        # Add one or more new tables to finance.db via which to keep track of the purchase. 
        # Store enough information so that you know who bought what at what price and when.
        # Use appropriate SQLite types. Define UNIQUE indexes on any fields that should be unique.
        # Define (non-UNIQUE) indexes on any fields via which you will search (as via SELECT with WHERE).
        
        # First, we find a portfolio number for later reference. The portfolio numbers let's us
        # track earnings and losses by the specific purchases and sells
        new_port_number = db.execute("SELECT MAX(portfolio_number) as MAX_port FROM Portfolio \
                                    WHERE id =:username", username=session["user_id"])[0]["MAX_port"]
        if new_port_number == None:
            new_port_number = 1
        else:
            new_port_number += 1
        
        # Update transaction tab to include for overall history 
        db.execute("INSERT INTO Transactions (id, stock_symbol, purchase, price, shares) \
                    VALUES(:username, :stock_symbol, :purchase, :price, :shares)", \
                    username=session["user_id"], stock_symbol=company_quote["symbol"], \
                    purchase = 1, price=company_quote["price"], shares=shares)
        
        # Update Portfolio that keeps a running tally of what everyone has
        db.execute("INSERT INTO Portfolio (id, stock_symbol, stock_name, shares_owned, bought_price, portfolio_number) \
                    VALUES(:id, :stock_symbol, :stock_name, :shares_owned, :bought_price, :portfolio_number)", \
                    id=session["user_id"], stock_symbol=company_quote["symbol"], \
                    stock_name = company_quote["name"], shares_owned = shares, \
                    bought_price=company_quote["price"], portfolio_number = new_port_number)
            
        # Update user cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :username", \
                    cash = (user_cash-transaction_price), username=session["user_id"])
        
        return redirect(url_for("index"))
        
    else:
        return render_template("buy.html")
        
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    if request.method == "GET":
        # Get users portfolio information and cash, display it to them
        # query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :username", username=session["user_id"])
        user_cash = rows[0]["cash"]
            
        # get stock information for user by slect stock_name, stock_symbol
        temprows = db.execute("SELECT stock_symbol, purchase, shares, price, datetime \
        FROM Transactions WHERE id =:username ORDER BY datetime ", username=session["user_id"])
        
        # append to retrieved stock dictionaries current price info
        # also go ahead and find total user value

        for stock in temprows:
            if stock["purchase"] == 0:
                stock["purchase"] = "SELL ORDER"
            else:
                stock["purchase"] = "BUY ORDER"
                
            stock['stock_name'] = lookup(stock["stock_symbol"])['name']

        return render_template("history.html", usercash=usd(user_cash), 
                                stockinfo = temprows, )   

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote.
    Odds are you’ll want to create two new templates (e.g., quote.html and quoted.html). 
    When a user visits /quote via GET, render one of those templates, inside of which 
    should be an HTML form that submits to /quote via POST. In response to a POST, quote 
    can render that second template, embedding within it one or more values from lookup.
    
    """
    
    # Require that a user input a stock’s symbol.
    if request.method == "POST":
        
        if not request.form.get("quote_symbol"):
            return apology("Missing quote symbol!")
        
        # using lookup from helpers.py to get the stock quote    
        company_quote = lookup(request.form.get("quote_symbol"))
        
        # ensure stock is valid, else apology
        if not company_quote:
            return apology("Stock wasn't valid!")
        
        # if stock is valid, render the quoted html
        return render_template("quoted.html", stockquote=company_quote)
    
    else:
        return render_template("quote.html")
        

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # Require that a user input a username. Render an apology if the user’s 
    # input is blank or the username already exists.
    # ensure username was submitted
    if request.method == "POST":
        
        # ensure username was submitted, render apology if there is none entered
        if not request.form.get("username"):
            return apology("Missing username!")
    
        # query database for username, render apologoy if username exists
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
        
        if len(rows) != 0:
            return apology("That username exists")
    
        # Require that a user input a password and then that same password again. 
        # Render an apology if either input is blank or the passwords do not match.
        if not request.form.get("password"):
            return apology("Missing password!")
        
        elif request.form.get("password") != request.form.get("password_confirm"):
            return apology("Password and Password Confirmation do not match")
    
        # INSERT the new user into users, storing a hash of the user’s password, not 
        # the password itself. Odds are you’ll find pwd_context.encrypt of interest.
        
        hashed = pwd_context.hash(request.form.get("password"))
        rows = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hashed)", username=request.form.get("username"), hashed = hashed)
    
        # remember which user has logged in
        rows =  db.execute("SELECT id FROM users WHERE username = :username", username = request.form.get("username"))
        session["user_id"] = rows[0]["id"]
        
        # redirect user to home page
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
        


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "GET":
    
        # Get users portfolio information and cash, display it to them
        # query database for user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :username", username=session["user_id"])
        user_cash = rows[0]["cash"]
            
        # get stock information for user by slect stock_name, stock_symbol
        temprows = db.execute("SELECT stock_symbol, shares_owned, date_purchased \
        FROM Portfolio WHERE id =:username ORDER BY stock_symbol ", username=session["user_id"])
        
        # append to retrieved stock dictionaries current price info
        # also go ahead and find total user value
        user_value = user_cash
        for stock in temprows:
            stock['current_price'] = lookup(stock["stock_symbol"])['price']
            stock['stock_name'] = lookup(stock["stock_symbol"])['name']
            user_value += stock['current_price']* stock["shares_owned"]

        return render_template("sell.html", usercash=usd(user_cash), 
                                stockinfo = temprows, uservalue = usd(user_value))        
    
    if request.method == "POST":
        
        # get stock information for user by slect stock_name, stock_symbol
        temprows = db.execute("SELECT stock_symbol, shares_owned, date_purchased, portfolio_number \
        FROM Portfolio WHERE id =:username ORDER BY stock_symbol ", username=session["user_id"])
        
        # Get the user information from the form
        shares_to_sell = int(request.form.get("sell_amount"))
        item_row_id = int(request.form.get("item_id"))
        
        
        # get information to update the portfolio database
        current_stock_price = lookup(temprows[item_row_id]["stock_symbol"])["price"]
        new_shares = temprows[item_row_id]["shares_owned"] - shares_to_sell
        item_port_number = temprows[item_row_id]["portfolio_number"]
        
        # check that sell amount is there and an acceptable amount
        if not shares_to_sell:
            return apology("Missing sell amount!")
            
        if shares_to_sell < 0 or shares_to_sell > temprows[item_row_id]["shares_owned"]:
            return apology("Enter an appropriate sell amount")
        
        # Update transaction tab with a sell order
        db.execute("INSERT INTO Transactions (id, stock_symbol, purchase, price, shares) \
                    VALUES(:username, :stock_symbol, :purchase, :price, :shares)", \
                    username=session["user_id"], stock_symbol=temprows[item_row_id]["stock_symbol"], \
                    purchase = 0, price = current_stock_price, shares=shares_to_sell)
        
        # go ahead and clear that row in the portfolio if there are no more shares left
        if new_shares == 0:
            db.execute("DELETE FROM Portfolio WHERE id =:username AND portfolio_number = :item_port_number \
            ", username=session["user_id"], item_port_number = item_port_number)
        else:
            db.execute("UPDATE Portfolio SET shares_owned = :new_shares \
            WHERE id =:username AND portfolio_number = :item_port_number \
            ", username=session["user_id"], new_shares = new_shares, item_port_number = item_port_number)
        
        # Update user cash with sold money
        rows = db.execute("SELECT cash FROM users WHERE id = :username", username=session["user_id"])
        user_cash = rows[0]["cash"]
        db.execute("UPDATE users SET cash = :cash WHERE id = :username", \
                    cash = (user_cash + shares_to_sell*current_stock_price), username=session["user_id"])
        
        return redirect(url_for("sell"))
        
