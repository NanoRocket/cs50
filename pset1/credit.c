#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

long asknumber(void);
int checknumber_length(long n);
void checkCard(long n, int num_digits);

int main(void)
{
    
    long number = asknumber();
    int digits = checknumber_length(number);
    checkCard(number, digits);
    
}
//--------------------------------------------------------
long asknumber(void)
{
    printf("Number: ");
    long x = get_long_long();
    
    return x;
}
//--------------------------------------------------------
int checknumber_length(long n)
{
    int num_digits = 0;
    while (n>=1)
    {
        num_digits = num_digits + 1;
        n = n/10;
        
    }
    
    return num_digits;
}
//--------------------------------------------------------
void checkCard(long n, int num_digits)
{
    ///Extract digits into an array
    int numberArray[num_digits];
    int c = 0;
    while (n != 0)
    {
        numberArray[c] = n % 10;
        n /= 10;
        c++;
    }

    //Step  0
    long sum = 0;
    long secondSum = 0;
    
    for (int i=0; i < num_digits; i=i+1)
    {
        if (i%2 == 1)
        {
            if (2*numberArray[i]>9)
            {
                sum = sum + 1;
                sum = sum + 2*(numberArray[i])%10;
            }
            else
            {
                sum = sum + 2*numberArray[i];
            }
        }
        
       else
        {
            secondSum = secondSum + numberArray[i];
        }
    }
    
    if ((sum+secondSum)%10 == 0)
    {
        if (numberArray[num_digits-1] == 3 && (numberArray[num_digits-2] == 4 || numberArray[num_digits-2] == 7) && num_digits == 15)
        {
            printf("AMEX\n");
        }
        
        else if (numberArray[num_digits-1] == 5 && (numberArray[num_digits-2] == 1 || numberArray[num_digits-2] == 2 || numberArray[num_digits-2] == 3 || numberArray[num_digits-2] == 4 || numberArray[num_digits-2] == 5) && num_digits == 16)
        {
            printf("MASTERCARD\n");
        }
        
        else if (numberArray[num_digits-1] == 4 && (num_digits == 13 || num_digits == 16))
        {
            printf("VISA\n");
        }
        
        else
        {
            printf("INVALID\n");
        }
    }
    
    
    else
    {
        printf("INVALID\n");
    }
    
}
//--------------------------------------------------------
