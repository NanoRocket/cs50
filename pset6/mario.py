import sys

def main():
    # Get the user's height, if it is zero just stop the program
    height = int(input("Height: "))
    
    if (height == 0):
        sys.exit
    
    while(height < 0 or height > 23):
        height = int(input("Height: "))
        
    for i in range(1,height+1):
        print_pyramid_char(i, height)
        
def print_pyramid_char(row, size):
    """Create a row like a pyramid in mario.
    Takes in a specific row (number of # to display)
    Takes in a size (how big the pyramid will be)
    Outputs a line of characters
    """
    
    # look at how easy string arithmetic makes this
    spaces = size - row
    print(" "*spaces, end="")
    print("#"*row+" "+"#"*row)

#Ensure that the program is always runs last    
if __name__ == "__main__":
    main()
    
    