# File: crack.py
# Name: Jan Clawson
# Date: 4/17/2017
# Course: CS50 
# Desc: This program cracks the given password (hashed with C’s DES-based 
#       (not MD5-based) crypt function), assuming password is no longer than 4
#       alphabetical characters
# Usage: gives an argument to be dehashed
# File Dependencies: 
# Subfunctions: 


# get hashed password view command line

# crack hashed password
# assumed hashed with crypt module
# crypt.crypt(word, salt)
# assume only 4 characters loong and alphabetical

import sys
import crypt
import string
import itertools

def main():
    
    word_length = 4;
    
    # insure we have correct number of arguments
    if len(sys.argv) != 2:
        print("Incorrect arguments: Wrong amount of arguments")
        exit(1)
        
    ## make an array of characters we are going to go through
    character_array = string.ascii_lowercase+string.ascii_uppercase
    
    # get the salt (string)
    salt = sys.argv[1][0:2]
    
    # get every permutation
    # for each word length
    for word_length in range(1,word_length+1):
        
        # use itertools product to get all the character arrays we could have
        for word in itertools.product(character_array, repeat=word_length):
            
            # create string by joining the outputed tuples
            user_trial_string = ''.join(word)
            
            # compare the trial string to the provided password and salt
            if compare(user_trial_string, sys.argv[1],salt) == 0:
                exit(0)
        
def compare(trialstring, hashedstring, salt):
    """ Compare trialstring to a hashed string after passing it to crpyt
    """
    if (crypt.crypt(trialstring, salt) == hashedstring):
        print(trialstring)
        return 0
    
    else:
        return 1

if __name__ == "__main__":
    main()