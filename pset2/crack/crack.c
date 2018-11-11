// File: crack.c
// Name: Jan Clawson
// Date: 4/17/2017
// Course: CS50 
// Desc: This program cracks the given password (hashed with C’s DES-based 
//       (not MD5-based) crypt function), assuming password is no longer than 4
//       alphabetical characters
// Usage: gives an argument to be dehashed
// File Dependencies: 
// Subfunctions: 

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#define _GNU_SOURCE
#include <crypt.h>

int compare(string key1, string key2, string salt);

int main(int argc, string argv[]) 
{
    // insure we have the correct number of arguments
    if (argc != 2)
    {
        printf("Incorrect arguments: Wrong amount of arguments\n");
        return 1;
    }
    
    // make an array of the characters we are going to go through, instead of having to do ASCII math
    // can change this to include more characters
    char character_array[52];
    for(int i=0; i<26; i++)
    {
        character_array[i]= (char) i+65;
        character_array[i+26]= (char) i+97;
    }
    int character_array_length = 52;
    
    //get the salt, store as char array
    char salt[3];
    salt[0]=argv[1][0];
    salt[1]=argv[1][1];
    salt[2] = '\0';
    
    // Here we make a character array we will use as the trial array, initially filled with nulls
    char user_trial_key[5] = {'\0','\0','\0','\0','\0'};

    // The goal here is to iterate over all the characters in the character array, for 4 bytes in the array
    // We then compare these strings as we iterate over them using the helper compare function
    // This should be improved so we don't need nested for loops that must be changed when we want to try longer lengths
    for(int i = 0; i<character_array_length; i++)
    {
        for(int j = 0; j<character_array_length; j++)
        {
            for(int k = 0; k<character_array_length; k++)
            {
                for(int m=0; m<character_array_length; m++)
                {
                    user_trial_key[0] = character_array[m];
                    if(compare(user_trial_key, argv[1], salt)==0)
                    {
                        return 0;
                    };
                }
                
                user_trial_key[1] = character_array[k];
                if(compare(user_trial_key, argv[1], salt)==0)
                {
                    return 0;
                };
            }
            
            user_trial_key[2] = character_array[j];
            if(compare(user_trial_key, argv[1], salt)==0)
            {
                return 0;
            };
        }
        
        user_trial_key[3] = character_array[i];
        if(compare(user_trial_key, argv[1], salt)==0)
        {
            return 0;
        };
    }
    
}

        
// Compares two strings, one to be gone through crpyt with a salt, and the other that is already hashed using crypt
int compare(string trialstring, string hashedstring, string salt) 
{
    if (strcmp(crypt(trialstring, salt), hashedstring) == 0)
    {
        //Print password
        printf("%s\n", trialstring);
        return 0;
    }
    else
    {
        return 1;
    }
}