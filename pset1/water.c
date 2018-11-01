#include <stdio.h>
#include <cs50.h>

int main(void)
{
    double shower_flow = 1.5; // shower flow in gallons
    int gallon = 128;  //gallons in ounces
    int water_bottle = 16;   // ounces in a water bottle
    printf("Minutes: ");
    int minutes = get_int();
    
    
    printf("Bottles: %f\n", shower_flow*minutes*gallon/water_bottle);
}