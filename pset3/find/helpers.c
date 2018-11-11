/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
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
        // printf("------START LOOP-------\n");
        // printf("Start: %i\n",start);
        // printf("Mid: %i\n",middle);
        // printf("End: %i\n",end);
        // printf("Value: %i\n",values[middle]);
        
        // if it is element i am looking for, return true
        if (value == values[middle])
        {
            // printf("TRUE");
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
//        printf("Middle: %i\n",middle);
    }
    //If we reach the end of the loop, the value must not be there
//    printf("False\n");
    return false;
    
}

/**
 * Sorts array of n values.
 * sort, from smallest to largest, the array of numbers that it’s passed.
 * Assume that each of the array’s numbers will be non-negative and less than 65,536. But the array might contain duplicates.
 * The running time of your implementation must be in O(n), where n is the array’s size. Yes, linear! Keep in mind that 65,536 is a constant.
 **/
 
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
