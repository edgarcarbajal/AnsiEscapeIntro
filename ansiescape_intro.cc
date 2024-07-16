#include <iostream>
#include <vector>
#include <utility> // pair
#include <ctime> //srand() & time()
#include <cstdlib> // rand()
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ // both of these syscalls used to get size of console window!
#include <unistd.h> // for STDOUT_FILENO
#include <curses.h> // for getch()

using namespace std;

// ANSI Escape character - Prepended on each ANSI code sequence. Tells the terminal what to do with the following characters/code.
static const char * CSI = "\033["; // Named either ESC or CSI
// then neds an 'm' right after it!


/*
     A full sequence (ex: "\033[<x>m", where <x> is any extra parameters[can be blank]) is called a Control Sequence Introducers(CSIs)
*/

void drawHorizontalLine(int startpos, int row, int bg_color, int max_col) {
    for(int c = startpos; c <= max_col; c++)
        printf("%s%d;%dH%s%dm %sm", CSI, row, c, CSI, bg_color, CSI);
}

void drawVerticalLine(int startpos, int col, int bg_color, int max_row) {
    for(int c = startpos; c <= max_row; c++)
        printf("%s%d;%dH%s%dm %sm", CSI, c, col, CSI, bg_color, CSI);
}

void generateRandomAliveCells(vector<vector<bool>>* grid, int n, int m) {
    int k = (n * m) / 8; // Fill a eighth of the grid with alive cells

    for(int i = 0; i < k; i++) {
        int rand_i = rand() % n;
        int rand_j = rand() % m;

        grid->at(rand_i).at(rand_j) = true;
    }
}

void gameOfLife(vector<vector<bool>>* grid, int n, int m);


int main() {
    // setup random number generation for the whole program(used later in a function):
    srand(time(0));

    //clear console before printing!
    printf("%s2J", CSI);            // <-- cursor still at same position when running the program!!
    printf("%s%d;%dH\n", CSI, 1, 1); // place cursor at beggining of now cleared console - in order to have the right format!

    //printf("%s3J", CSI);

    struct winsize console_window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &console_window); // get console window information into console_window


    printf("This console window has %d rows, and %d columns.\n\n", console_window.ws_row, console_window.ws_col);


    //generate 108 numbers and use those numbers as codes, while also printing them out as characters!
    // Modified Code From Wikipedia: https://en.wikipedia.org/wiki/ANSI_escape_code
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 10; j++) { // double forloop to print numbers in a grid/matrix
            int n = (10*i) + j; // i acts as the tens exponent, and j acts as the ones exponent
            if(n > 108)
                break;

            // 3 in %3d adds spacing!
            printf("%s%dm %3d %sm", CSI, n, n, CSI); // 1 Escape char at begining, and then 1 at end to reset the graphics of the cursor (ie: no more blue/red etc)
        }
        printf("\n"); // new row
    }


    // draw red, green, blue, yellow 1x1 char boxes in the corners of the console
    printf("%s%d;%dH", CSI, 1, 1); // places cursor in pos (1,1) <--> (x,y) of console window (ie: top left)
    printf("%s%dm %sm", CSI, 41, CSI); // makes background color to red, and places a space right after - to make the color show! Then reset color graphics to not effect other sections

    printf("%s%d;%dH", CSI, 1, console_window.ws_col); // top right
    printf("%s%dm %sm", CSI, 42, CSI); // green

    printf("%s%d;%dH", CSI, console_window.ws_row , 1); // bottom left
    printf("%s%dm %sm", CSI, 43, CSI); // yellow

    printf("%s%d;%dH", CSI, console_window.ws_row, console_window.ws_col); // bottom right
    printf("%s%dm %sm", CSI, 44, CSI); // blue

    printf("\n");


    // Now scroll down the amount of rows, in order to make like a new page(and to keep old output!)
    printf("%s%d;%dH\n", CSI, console_window.ws_row, console_window.ws_col); // place cursor at end to make sure dont delete any new content?
    //printf("%s%dT", CSI, console_window.ws_row); // DOES NOT WORK?? MAKES OLD OUTPUT BLANK LINES, maybe a forloop is better???
    
    printf("Basic Demo of \"Conway's Game of Life\" below:\nWill do 10 iterations before stopping - PLEASE DO NOT RESIZE CONSOLE WINDOW!\n");
   
    for(int i = 5; i > 0; i--) {
        printf("\rStarting in %s%dm%d%sm sec...", CSI, 30+i, i, CSI); // print line, then return cursor to begining of line with '/r'
        fflush(stdout); // flush stdout to make sure the new line shows up in next loop
        sleep(1);
    }
    printf("%s7mStart!%sm\n", CSI, CSI);

    //Doing alternative forloop for the new page:
    for(int i = 0; i < console_window.ws_row; i++)
        printf("\n");

    
    //Make a border to show boundaries of the grid
    //---
    //printf("%s7mIteration #%d%sm", CSI, 1, CSI);
    // drawHorizontalLine(1, 1, 104, console_window.ws_col); // Light blue line - top border
    // drawHorizontalLine(1, console_window.ws_row, 104, console_window.ws_col); // bototm border

    // drawVerticalLine(1, 1, 104, console_window.ws_row); // left border
    // drawVerticalLine(1, console_window.ws_col, 104, console_window.ws_row); //right border

    int n = console_window.ws_row;
    int m = console_window.ws_col;
    vector<vector<bool>> grid(n, vector<bool>(m, false));

    generateRandomAliveCells(&grid, n, m);


    // 10 Iterations only!
    for(int t = 0; t < 10; t++) {
        //clear the screen
        printf("%s2J%s%d;%dHIteration #%d", CSI, CSI, n/2, m/2, t+1);
        sleep(1);

        // draw the cells in the terminal
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(grid[i][j]){
                    printf("%s%d;%dH%s43m %sm", CSI, i+1, j+1, CSI, CSI);
                }
            }
        }

        // do a gameOfLife iteration!
        gameOfLife(&grid, n, m);
        
        sleep(1); //Wait 1 sec to show progress more visibly;
    }


    //Do this later (NEED ncurses library with linking options in makefile in order to make it work!!)
    // (New): Decided not to test here since you need to do a bunch of initialization for ncurses!!
    // unsigned char char_inp;
    // while(1) {
    //     char_inp = getch();

    //     switch(char_inp) {
    //         case KEY_UP:
    //             printf("%s%dmUp Arrow Key pressed!\n", CSI, 31); // red
    //             break;
    //         case KEY_DOWN:
    //             printf("%s%dmDown Arrow Key pressed!\n", CSI, 32); // green
    //             break;
    //         case KEY_LEFT:
    //             printf("%s%dmLeft Arrow Key pressed!\n", CSI, 33); // yellow
    //             break;
    //         case KEY_RIGHT:
    //             printf("%s%dmRight Arrow Key pressed!\n", CSI, 34); // blue
    //             break;
    //         default:
    //             printf("Invalid key pressed! Key --> %c", char_inp);
    //     }
    // }

    // printf("%s%dmUp Arrow Key pressed!\n", CSI, 31); // red
    // printf("%s%dmDown Arrow Key pressed!\n", CSI, 32); // green
    // printf("%s%dmLeft Arrow Key pressed!\n", CSI, 33); // yellow
    // printf("%s%dmRight Arrow Key pressed!\n", CSI, 34); // blue
    //Random stuff to put new page!

    return 0;
}



void gameOfLife(vector<vector<bool>>* grid, int n, int m) {
    vector<vector<bool>> newgrid(n, vector<bool>(m, false));

    // vertical movement: +_i
    // top: i-1; down: i+1;

    // horiz movement: +_j
    // left: j-1; right: j+1
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int top = i-1;
            int down = i+1;
            int left = j-1;
            int right = j+1;
            vector<pair<int, int>> neighbors = {
                {top, j}, {down, j}, {i ,left}, {i, right}, // <-- 4 adjacent
                {top, left}, {top, right}, {down, left}, {down, right} // <-- 4 diagonal
            };

            int numOfNeighbors = 0;
            for(auto itr = neighbors.begin(); itr != neighbors.end(); ++itr) {
                bool rowBound = (itr->first > -1) && (itr->first < n);
                bool colBound = (itr->second > -1) && (itr->second < m);

                if(rowBound && colBound) {
                    if(grid->at(itr->first)[itr->second])
                        numOfNeighbors++;
                }
            }

            if(grid->at(i)[j]){
                if(numOfNeighbors < 2 || numOfNeighbors > 3)
                    newgrid[i][j] = false;
                else
                    newgrid[i][j] = true;
            }
            else {
                if(numOfNeighbors == 3)
                    newgrid[i][j] = true;
            }
        }
    }

    grid->assign(newgrid.begin(), newgrid.end());
}