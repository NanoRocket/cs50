// File: caesar.c
// Name: Jan Clawson
// Date: 4/13/2017
// Course: CS50 
// Desc: Design and implement a program, caesar, that encrypts messages using Caesar’s cipher
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
        printf("Incorrect arguments");
        return 1;
    }
    
    // copy the key, convert to make sure it falls in the right range of numbers (1-26)
    int key = atoi(argv[1])%26;
    
    //Prompt user for string
    printf("plaintext: ");
    string s = get_string();
    
    // Get length of plaintext strength, initialize ciphered_letters character array
    int name_length = strlen(s);
    char ciphered_letters[name_length];
    
    //go through each letter, cipher each letter using functions
    for (int i=0; i < name_length; i++)
    {
        // shift a lowercase letter to another lowercase letter, copy into ciphered array
        if (islower(s[i]))
        {
            ciphered_letters[i] = shiftlowercase((int)s[i], key);
        }
        
        // shift an uppercase letter to another uppercase letter, copy into ciphered array
        else if (isupper(s[i]))
        {
            ciphered_letters[i] = shiftuppercase((int)s[i], key);
        }
        
        // if it isn't a letter, do not shift it at all, copy into ciphered array
        else
        {
            ciphered_letters[i] = s[i];
        }
        
    }
    
    // Print out the cipher text
    printf("ciphertext: ");
    printf("%s\n", ciphered_letters);
    
    return 0;
}

//This function shifts uppercase letters by a cipher while keeping case
// Input: an int type of an ASCII character, and an int key to shift the character
// Output: A char that is shifted by the key
char shiftuppercase(int character, int key)
{
    char x =((character-65)+key)%26+65;
    return x;
}

//This function shifts lowercase letters by a cipher while keeping case
// Input: an int type of an ASCII character, and an int key to shift the character
// Output: A char that is shifted by the key
char shiftlowercase(int character, int key)
{
    char x = ((character-97)+key)%26+97;
    return x;
}