#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <math.h>

int askamount(void);

int main(void)
{
    printf("O hai! ");
    int total = askamount();
    int number_of_coins = 0;
    
    while(total <0)
    {
        total = askamount();
    }
    
    while(total > 25)
    {
        total = total - 25;
        number_of_coins = number_of_coins + 1;
    }
    
    while(total > 10)
    {
        total = total - 10;
        number_of_coins = number_of_coins + 1;
    }
    
    while(total > 5)
    {
        total = total - 5;
        number_of_coins = number_of_coins + 1;
    }
    
    while(total > 0)
    {
        total = total - 1;
        number_of_coins = number_of_coins + 1;
    }
    
    printf("%i\n", number_of_coins);
    
}

int askamount(void)
{
    printf("How much change is owed?\n");
    float amount = get_float();
    int rounded_amount = round(amount * 100);
    return rounded_amount%100;
}

