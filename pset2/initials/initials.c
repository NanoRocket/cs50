// File: initials.c
// Name: Jan Clawson
// Date: 4/13/2017
// Course: CS50 
// Desc: Implement a program that, given a person’s name, prints a person’s initials
// Usage: The program prompts the user for input that only contains letters plus spaces.
//        Program prints the user's initials with no spaces or periods followed by newline.
// File Dependencies: stdio.h , cs50.h , string.h , ctype.h

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    
    //Prompt user for string
    string s = get_string();
     
    
    //Make a new string for the initials and get length
    int name_length = strlen(s);
    char initial_letters[name_length];
    
    //iterator for the second array
    int j = 0;
    
    //If the first thing a user enters is a string, place into array copy
    if (isalpha(s[0]))
    {
        initial_letters[0] = toupper(s[0]);
        j++;
    }
    
    //Iterate through name. Start at second letter to avoid going before array  
    for(int i = 1; i < name_length; i++)
    {
        //If we have a letter after a space, place this letter in the array copy
        if (isalpha(s[i]) && isspace(s[i-1]))
        {
            initial_letters[j] = toupper(s[i]);
            j++;
        }
        
    }
//Print out the initials array
printf("%s\n", initial_letters);
}