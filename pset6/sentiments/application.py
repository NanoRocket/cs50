"""
In this file is a "controller" for a Flask-based web app with two endpoints: / and /search.
The first displays the simplest of forms via which you can search for a user on Twitter by 
screen name. The second displays one of those pie charts categorizing that user’s tweets.

"""

from flask import Flask, redirect, render_template, request, url_for

import helpers
import sys
import os
from analyzer import Analyzer
from nltk.tokenize import TweetTokenizer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, count = 100)
    if tweets == None:
        sys.exit("helpers didn't work")

    # instantiate analyzer
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    analyzer = Analyzer(positives, negatives)
    
    # instantiate tweet tokenizer
    tknzr = TweetTokenizer()
    
    positive = 0
    negative = 0
    neutral = 0
    
    # analyzes the sentiment of each of those tweets
    for tweet in tweets:
        score = 0
        
        #get score of tweet
        score += analyzer.analyze(tweet)
        
        # display score of the tweet
        if score > 0.0:
            positive += 1
        elif score < 0.0:
            negative += 1
        else:
            neutral += 1 


    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
