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
        usleep(500000);
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
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int size = d*d - 1;
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = size--; // assigning values in the 2D array and decrementing each time
        }
    }
    
    if ((d*d) % 2 == 0) // using the modulo to check for even value of d, in case of such, 1 and 2 are swapped
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0) // instead of printing 0, a blank tile is printed
            {
                printf(" _");   
            }
            
            else
            {
                printf("%2i", board[i][j]);  //adjusting for space in the board to make it more visually appealing
            }
        }
        
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    if (tile > d*d - 1 || tile < 1) // classifying the range of integers the user can input
    {
        return false;
    }
    int row = 0;
    int column = 0;
    
    // searching the board for the desired location (row and column)
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                row = i;
                column = j;
            }
        }
    }
    
    if (board[row - 1][column] == 0 && row - 1 >= 0) // in case the number above the blank space is moved by the user
    {
        board[row  - 1][column] = board[row][column];
        board[row][column] = 0;
        return true;
        
    }
    
    else if (board[row + 1][column] == 0 && row + 1 < d) // in case the number below the blank space is moved by the user
    {
        board[row + 1][column] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    
    else if (board[row][column - 1] == 0 && column - 1 >= 0) // in case the number on the left of the blank space is moved by the user
    {
        board[row][column - 1] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    
    else if (board[row][column + 1] == 0 && column + 1 < d) // in case the number on the right of the blank space is moved by the user
    {
        board[row][column + 1] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    
    else 
    {
        return false; // classifying all other attempted moves as 'illegal'
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void) // checking whether the integers in the board are arranged in asscending order
{
    int check = 0;
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            check++;
            if (board[i][j] != check && (d*d) != check) // if the numbers are not in order, the game continues
            {
                return false;
            }
        }
    }
    return true;
    
}
