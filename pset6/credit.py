def main():
    
    creditcard = input("Number: ")
    
    while creditcard.isnumeric() == False:
        creditcard = input("Retry: ")
    
    checkCard(creditcard)
    
def checkCard(number):
    """Takes in credit card number.
    Outputs a print string to terminal saying if it is a card
    If it is a card it will give the type of credit card
    If it is not a card, it will print INVALID
    """
    #convert number into an array for operations
    number_array = []
    for i in number:
        number_array.append(int(i))

    # Multiply every other digit by 2, starting with the number’s second-to-last digit
    l = number_array[::-1]
    l[1::2] = [i*2 for i in l[1::2]]
    
    # Add all the digits together!
    summation = 0
    for i in l:
        # if the number is over 10, we need to add its digits to get the proper sum
        if i>9:
            i = i//10 + i%10
        summation = summation+i
    
    # If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
    if summation%10 == 0:
        
        # American Express numbers all start with 34 or 37
        if int(number[0]+number[1]) in {34, 37}:
            print("AMEX")
        
        # MasterCard numbers all start with 51, 52, 53, 54, or 55;
        elif int(number[0]+number[1]) in {51, 52, 53, 54, 55}:
            print("MASTERCARD")
        
        # Visa numbers all start with 4.
        elif number[0] == 4:
            print("VISA")
            
        else:
            print("INVALID")
    
    else:
        print("INVALID")
    
if __name__ == "__main__":
    main()