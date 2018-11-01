#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

int askheight(void);
bool checkheight(int n);
void print_pyramid_char(int row, int size);

int main(void)
{
    int height = askheight();
    
    if (height == 0)
    {
        exit(height);
    }
    
    while (checkheight(height) == false)
    {
        height = askheight();
    }
    
    for (int i = 1; i < height+1; i++)
    {
        print_pyramid_char(i, height);
        printf("\n");
    }
}

int askheight(void)
{
    printf("Height: ");
    int x = get_int();
    return x;
}

bool checkheight(int n)
{
    if ( n>=0 && n<=23 ) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

void print_pyramid_char(int row, int size)
{
    int total = size;
    int hashes = row;
    int spaces = total - hashes;
    
    for (int i=0; i < spaces; i++)
    {
        printf(" ");
    }
    
    for (int i=0; i < hashes; i++)
    {
        printf("#");
    }
    
    printf("  ");
    
    for (int i=0; i < hashes; i++)
    {
        printf("#");
    }
    
}