// File: vigenere.c
// Name: Jan Clawson
// Date: 4/15/2017
// Course: CS50 
// Desc: Design and implement a program, vigenere, that encrypts messages using Vigenere’s cipher
//       This encrypts a message by shifting the letters a set amount given by a key
// Usage: The program prompts the user to run the program with a positive int 
//        The program will then prompt the user for a string of plaintext .
//        Program prints ciphertext of plaintext rotated by k positions, preserve case.
// File Dependencies: stdio.h , cs50.h , string.h , ctype.h
// Subfunctions: shiftuppercase, shiftlowercase

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

char shiftuppercase(int character, int key);
char shiftlowercase(int character, int key);

int main(int argc, string argv[])
{
    // insure that program was run with correct arguments
    if (argc != 2)
    {
        printf("Incorrect arguments: Wrong amount of arguments\n");
        return 1;
    }
    
    // copy the key
    string keyword = argv[1];
    
    //get length of keyword, then go through and make sure each letter is alphabetical
    // also convert the keyword to an array of integers
    int keyword_length = strlen(keyword);
    int keyword_number[keyword_length];
    
    for (int i=0; i < keyword_length; i++)
    {
        //if character not an alphabetical character make function return 1
        if (!isalpha(keyword[i]))
        {
            printf("Incorrect arguments: Not alphabetical character\n");
            return 1;
        }
        //normalize by 'A' Ascii if it is uppercase letter, place into keyword array
        else if (isupper(keyword[i])) 
        {
            keyword_number[i] = keyword[i] - 'A';
        }
        //normalize by 'a' Ascii if it is uppercase letter, place into keyword array
        else if (islower(keyword[i]))
        {
            keyword_number[i] = keyword[i] - 'a';
        }
    }
    
    //Prompt user for string
    printf("plaintext: ");
    string s = get_string();
    
    //Get length of plaintext strength, initialize ciphered_letters character array
    int name_length = strlen(s);
    char ciphered_letters[name_length+1];
    
    // go through each letter, cipher each letter using keyword number. Have additional
    // iterator j for going through the ciphered letter over and over. The modulo operation
    // allows us to loop through the keyword over and over
    int j = 0;
    for (int i=0; i < name_length; i++)
    {
        // shift a lowercase letter to another lowercase letter, copy into ciphered array
        if (islower(s[i]))
        {
            ciphered_letters[i] = shiftlowercase((int)s[i], keyword_number[j]);
//            printf("i=%i, j=%i, encoding %c with %c\n",i,j,s[i],ciphered_letters[i]);
            j = (j+1)%keyword_length;
        }
        
        // shift an uppercase letter to another uppercase letter, copy into ciphered array
        else if (isupper(s[i]))
        {
            ciphered_letters[i] = shiftuppercase((int)s[i], keyword_number[j]);
//            printf("i=%i, j=%i, encoding %c with %c\n",i,j,s[i],ciphered_letters[i]);
            j = (j+1)%keyword_length;
        }
        
        // if it isn't a letter, do not shift it at all, copy into ciphered array
        else
        {
            ciphered_letters[i] = s[i];
//            printf("i=%i, j=%i, encoding %c with %c\n",i,j,s[i],ciphered_letters[i]);
        }
        
        //printf("%s\n", ciphered_letters);
        // Fun thing I just learned from check50 and http://stackoverflow.com/questions/32234188/vigenere-cipher-extra-characters
        // We need to terminate our string with NUL terminator
        ciphered_letters[name_length] = '\0';
        
    }
    // Print out the cipher text
    printf("ciphertext: ");
    printf("%s\n", ciphered_letters);
    
    return 0;
}

// WAS ABLE TO USE THESE HELPER FUNCTIONS FROM CAESAR----------------------------------

// //This function shifts uppercase letters by a cipher while keeping case
// // Input: an int type of an ASCII character, and an int key to shift the character
// // Output: A char that is shifted by the key
char shiftuppercase(int character, int key)
{
    char x =((character-65)+key)%26+65;
    return x;
}

// - ---------------------------------------------------------------------------------
// //This function shifts lowercase letters by a cipher while keeping case
// // Input: an int type of an ASCII character, and an int key to shift the character
// // Output: A char that is shifted by the key
char shiftlowercase(int character, int key)
{
    char x = ((character-97)+key)%26+97;
    return x;
}