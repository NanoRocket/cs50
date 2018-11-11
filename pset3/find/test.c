/**
 * Prompts user for as many as MAX values until EOF is reached, 
 * then proceeds to search that "haystack" of values for given needle.
 *
 * Usage: ./find needle
 *
 * where needle is the value to find in a haystack of values
 */
       
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

bool search(int value, int values[], int n);
void sort(int values[], int n);

int main(void)
{

    // fill haystack
    int size=4;
    int haystack[4] = {49,43,42,45};
    int needle = 42;
    
    printf("\n");

    // sort the haystack
    sort(haystack, size);
    
    for(int i = 0; i<size; i++)
    {
        printf("Array entry: %i\n",haystack[i]);
    }

    // try to find needle in haystack
    printf("WE found: %d\n",search(needle, haystack, size));

}

bool search(int value, int values[], int n)
{
    // implementation of a searching algorithm,  running time must be in O(log n).
    //must return false immediately if n is non-positive.
    if (n < 1)
    {
        return false;
    }
    
    //must return true if value is in values and false if value is not in values.
    // implement binary search
    // look at middle of array
    // if element you're looking for
    //     return true
    // else if element is to left
    //     search left half of array
    // else if element is to right
    //     search right half of array
    // else
    //     return false
    
   int start = 0;
   int end = n-1;
   int middle = end/2;
    
    while (start <= end)
    {
        printf("------START LOOP-------\n");
        printf("Start: %i\n",start);
        printf("Mid: %i\n",middle);
        printf("End: %i\n",end);
            // if it is element i am looking for, return true
        if (value == values[middle])
        {
            return true;
        }
        // if value less than middle value, search left half of array (note, here we using floor division)
        else if (value < values[middle])
        {
            end = middle -1;
            middle = (start+end)/2;
        }
        
        // if value less than middle value, search right half of array (note, here we using floor division, so we compensate for changing length)
        else if (value > values[middle])
        {
            start = middle + 1;
            middle = (start+end)/2;
        }
        
        printf("-------------\n");
        printf("Start: %i\n",start);
        printf("Mid: %i\n",middle);
        printf("End: %i\n",end);
        printf("------END LOOP-------\n");
        printf("------END LOOP-------\n");
    }
    
    return false;
    
}


void sort(int values[], int n)
{
    // TODO: implement an O(n) sorting algorithm
    //This is a counting array implementation
    int counting_array[65536] = {0};
    int index;
    
    for(int i = 0; i < n; i++)
    {
        index = values[i];
        counting_array[index]+=1;
    }
    
    index = 0;
    for(int j = 0; j < 65536; j++)
    {
        while(counting_array[j] > 0)
        {
            values[index]=j;
            index = index + 1;
            counting_array[j] = counting_array[j] - 1;
        }
            
    }
    
}
