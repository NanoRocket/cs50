/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

//set up array giving row number and column number of blank space
int blanktile[2];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(20000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(20000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
 
void init(void)
{
    // set the initial value of the biggest integer
    int current_value = d*d -1;
    
    // go through each element of the array and assign the proper value to that position
    for (int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            board[i][j] = current_value;
            current_value -= 1;
        }
    }
    
    // if the board dimensions are even, we must switch the one and two ( I used magic numbers for this )
    if (d%2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
    
    //set up where initial blank space is
    blanktile[0] = d-1;
    blanktile[1] = d-1;
    
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            if(board[i][j]==0)
            {
                printf("__ | ");
            }
            else
            {
                printf("%2i | ", board[i][j]);
            }
        }
        printf("\n");
        // NOTE: I Took this technique from stackexchange, I'm guessing it is 'revealing' the string at the side
        printf("%.*s", 5*d, "----------------------------------------------------------------------------------------------------");
        printf("\n");
        
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. This created function is O(Constant)
 */
bool move(int tile)
{
    int blank_row = blanktile[0];
    int blank_col = blanktile[1];
    
    // check above the blank for specified tile, if match, make blank tile value equal tile, make chage to blanktile location
    if (board[blank_row+1][blank_col] == tile && blank_row+1 < d)
    {
        board[blank_row+1][blank_col] = 0;
        board[blank_row][blank_col] = tile;
        blanktile[0] = blank_row+1;
        return true;
    }
    
    // check below the blank for specified tile, similar to above
    else if (board[blank_row-1][blank_col] == tile && blank_row-1 > -1)
    {
        board[blank_row-1][blank_col] = 0;
        board[blank_row][blank_col] = tile;
        blanktile[0] = blank_row-1;
        return true;
    }
    
    // check to the right of the blank for specified tile, similar to above
    else if (board[blank_row][blank_col+1] == tile && blank_col+1 < d)
    {
        board[blank_row][blank_col+1] = 0;
        board[blank_row][blank_col] = tile;
        blanktile[1] = blank_col+1;
        return true;
    }
    
    //check to the left of the blank for the specified tile, similar to above
    else if (board[blank_row][blank_col-1] == tile && blank_col-1 > -1)
    {
        board[blank_row][blank_col-1] = 0;
        board[blank_row][blank_col] = tile;
        blanktile[1] = blank_col-1;
        return true;
    }
    
    // specified tile must be not in correct spot
    else
    {
        return false;
    }
    
    // int tile_row;
    // int tile_col;
    // int blank_row;
    // int blank_col;
    // // locate number in array
    // for (int i=0; i<d; i++)
    // {
    //     for (int j=0; j<d; j++)
    //     {
    //         if(board[i][j] == tile)
    //         {
    //             tile_row = i;
    //             tile_col = j;
    //         }
    //         else if(board[i][j] == 0)
    //         {
    //             blank_row = i;
    //             blank_col = j;
    //         }
    //     }
    // }
    
    // //check above and below
    // int y_dif = abs(tile_row-blank_row);
    // int x_dif = abs(tile_col-blank_col);
    
    // if(((x_dif < 2) || (y_dif < 2)) && (x_dif != y_dif))
    // {
    //     board[tile_row][tile_col] = 0;
    //     board[blank_row][blank_col] = tile;
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }
    
    
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // have trivial check to instantly get out of this function before doing for loop

    if(blanktile[0] != d-1 && blanktile[1] != d-1)  
    {
        return false;
    }
    
    else
    {
        // if it passes those conditions, it could be a winner, for loop check for each element (modulo for that final value to be blank)
        int current_value = 1;
        int max_val = d*d;
        for (int i=0; i<d; i++)
        {
            for (int j=0; j<d; j++)
            {
                if(board[i][j] != current_value)
                {
                    return false;
                }
                else
                {
                    current_value = (current_value+1) % max_val;
                }
                
                
            }
        }
    }
    
    // If we haven't failed by now, we aren't going to
    return true;
}
