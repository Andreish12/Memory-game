#include "stdio.h"
#include "stdlib.h"
#include "ncurses.h"
#include "time.h"



int boardSize = 3;
char** board;

char* usedSimbols;
int usedSimbolsSize;

// bot related
int** botKnownChars;
int botKnownCharsSize;

// themes
int selectedTheme = 11;                                                                                 // default theme if first run
int* themes;
int themesSize;

// player related
int selected1X = -1, selected1Y = -1, selected2X = -1,selected2Y = -1;
int playerPosX = 0, playerPosY = 0;
int player1Score = 0, player2Score = 0;

// game setting
bool player1Move = true;
bool bothPlayersPlay = false;


bool alreadyInUse(char* arr, int size, char ch);                                                        // check if char is already used in the board generation
bool moveCursor();
bool checkLeftFields();                                                                                 // check if there are any occupied fields 

int getIndex(char* arr, int size, char ch);                                                             // get an integer to use specific theme

void generateBoardAndAllRelated();
void resetBoard();
void fillBoardAndAllRelated();
void printBoard();
void printColoredSymbol(char ch, int theme);
void matchCheck();
void addPoints();
void printScore();
void printWinner();
void vsComputer();
void vsPlayer();
void practice();
void botMove();
void confirm();
void customization();
void uploadThemeToFile();
void downloadThemeFromFile();
void freeArrays();                                                                                      // deallocation of memory
void quit();




int main() {
    srand(time(0));

    downloadThemeFromFile();

    int ch;
    int selectedOption = 0;
    
    initscr();                                                                                          // Initialize ncurses mode
    start_color();                                                                                      // Enable color support
    cbreak();
    keypad(stdscr, TRUE);                                                                               // Enable function and arrow keys
    noecho();                                                                                           // Don't print user input


    
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // White on black                                         // color print  (num of pair, letter color, background color)
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Green on black
    init_pair(3, COLOR_BLUE, COLOR_BLACK);    // Blue on black
    init_pair(4, COLOR_RED, COLOR_BLACK);     // Red on black
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    // Cyan on black
    init_pair(6, COLOR_BLACK, COLOR_WHITE);   // Black on white
    init_pair(7, COLOR_BLUE, COLOR_WHITE);    // Blue on white
    init_pair(8, COLOR_YELLOW, COLOR_WHITE);  // Yellow on white
    init_pair(9, COLOR_RED, COLOR_WHITE);     // Red on white
    init_pair(10, COLOR_MAGENTA, COLOR_WHITE); // Magenta on white
    init_pair(11, COLOR_CYAN, COLOR_YELLOW);  // Cyan on yellow
    init_pair(12, COLOR_GREEN, COLOR_BLUE);   // Green on blue
    init_pair(13, COLOR_YELLOW, COLOR_RED);   // Yellow on red
    init_pair(14, COLOR_MAGENTA, COLOR_BLUE); // Magenta on blue
    init_pair(15, COLOR_RED, COLOR_YELLOW);   // Red on yellow
    init_pair(16, COLOR_WHITE, COLOR_GREEN);  // White on green
    init_pair(17, COLOR_BLACK, COLOR_RED);    // Black on red
    init_pair(18, COLOR_BLUE, COLOR_YELLOW);  // Blue on yellow
    init_pair(19, COLOR_MAGENTA, COLOR_CYAN); // Magenta on cyan
    init_pair(20, COLOR_BLACK, COLOR_MAGENTA); // Black on magenta
    init_pair(21, COLOR_WHITE, COLOR_BLUE);   // White on blue
    init_pair(22, COLOR_YELLOW, COLOR_BLACK); // Yellow on black
    init_pair(23, COLOR_CYAN, COLOR_GREEN);   // Cyan on green
    init_pair(24, COLOR_RED, COLOR_CYAN);     // Red on cyan
    init_pair(25, COLOR_WHITE, COLOR_RED);    // White on red

    char menu[][255] = {"VS Computer", "VS Player", "Practice","Customization", "Exit"};
    void (*menu_functions[])() = {vsComputer, vsPlayer, practice, customization, quit};                 // array of pointers to function (difficult, but i like it)
    int menuLen = sizeof(menu)/ sizeof(menu[0]);


    while (true)
    {   
        // reset all the stupidity 
        playerPosX = 0;
        playerPosY = 0;
        player1Score = 0;
        player2Score = 0;
        selected1X = -1;
        selected1Y = -1;
        selected2X = -1;
        selected2Y = -1;

        for (size_t i = 0; i < menuLen; i++)                                                            // print menu
        {
            if (selectedOption == i)
                attron(COLOR_PAIR(selectedTheme));                                                      // Turn on color text

            printw("%s\n", menu[i]);    
            attroff(COLOR_PAIR(selectedTheme));                                                         // Turn off color text
            
        }
        



        ch = getch();
        switch (ch)                                                                                     // selection in menu
        {
            case 119:  
            case KEY_UP:
                if (selectedOption > 0)
                    selectedOption--;
                else 
                    selectedOption = 4;     // yes it is hardcoded, but it's fine (hah, already suffered from this) 
            break;

            case 115:
            case KEY_DOWN:
                if (selectedOption < menuLen -1)
                    selectedOption++;
                else
                    selectedOption = 0;
            break;

            case 27:
                printw("\nO Kurwa esc pressed\n");
                confirm();
                endwin();                                                                               // Restore terminal settings
                return 0;
            break;    


            case 32:
            case 10:
                menu_functions[selectedOption]();
            break;

            default:
                printw("Key code: %d\n", ch);
                confirm();

            break;
        }

        clear(); 
        refresh();
    }
    
    confirm();
    endwin(); 
    return 0;
}

bool alreadyInUse(char *arr,int size, char ch)
{
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] == ch)
            return true;
        
        if(arr[i] == ' ')
        {
            arr[i] = ch;
            return false;
        }
    }

    return false;
}

int getIndex(char *arr, int size, char ch)
{
    for (size_t i = 0; i < size; i++)
        if (arr[i] == ch)
            return i;


return 0;
}

void generateBoardAndAllRelated()
{

    char key;
    do
    {   clear();
        printw("Select grid size\n");

        attron(COLOR_PAIR(selectedTheme));
        printw("%d",boardSize);
        attroff(COLOR_PAIR(selectedTheme)); // Turn off color text

        refresh();
        key = getch();
        switch (key)
        {
            case 115:
            case 2:
                if(boardSize > 2)
                boardSize --;
            break;

            case 119:
            case 3:
                boardSize++;
            break;
            
            case 27:
                quit();
            break;

           
            default:
            // printw("Key code: %d\n", key);
            // confirm();
            break;

           
        }
        clear();

    } while (key != 10 && key != 32  || boardSize > 11);



    board = malloc(boardSize * sizeof(char*));
    for (size_t i = 0; i < boardSize; i++)
    {
        board[i] = malloc(boardSize * sizeof(char));
    }


    usedSimbolsSize = boardSize * boardSize / 2 + 1;
    usedSimbols = malloc(usedSimbolsSize * sizeof(char));


    themesSize = boardSize * boardSize / 2 +1;
    themes = malloc(themesSize * sizeof(int));


    botKnownCharsSize = boardSize;
    botKnownChars = malloc(botKnownCharsSize * sizeof(int*));
    for (size_t i = 0; i < botKnownCharsSize; i++)
    {
        botKnownChars[i] = malloc(botKnownCharsSize * sizeof(int));
    }
    

}

void resetBoard()
{
   
    
    for (size_t i = 0; i < boardSize; i++)
        for (size_t j = 0; j < boardSize; j++)
            board[i][j] = ' ';
        

    for (size_t i = 0; i < usedSimbolsSize; i++)
        usedSimbols[i] = ' ';

    for (size_t i = 0; i < themesSize; i++)
        themes[i] = 0;
    
    

}



void fillBoardAndAllRelated()
{
    if (boardSize % 2 != 0 )
    {
        int x = rand() % boardSize;
        int y = rand() % boardSize;

        board[x][y] = '$';
    }

    for (size_t i = 0; i < boardSize; i++)
    {
        for (size_t j = 0; j < boardSize; j++)
        {
            if (board[i][j] == ' ')
            {
                char sign;
                do
                {
                    if(boardSize * boardSize < 51)
                        sign = rand() % 25 + 65;
                    else
                        sign = rand() % 65 + 48;
                } while (alreadyInUse(usedSimbols, usedSimbolsSize, sign));
                
                board[i][j] = sign;

                int x = 0, y = 0;
                do
                {
                    x = rand() % boardSize;
                    y = rand() % boardSize;
    
                } while (board[x][y] != ' ');
                
                board[x][y] = sign;
            }
            
        }
    }

    // themes 
    for (size_t i = 0; i < themesSize; i++)
    {
        int tmp = rand() % 23 + 2;
        themes[i] = tmp;
        if(i == themesSize -1)
            themes[i] = 1;
    }

    //bot
    for (size_t i = 0; i < botKnownCharsSize; i++)
    {
        for (size_t j = 0; j < botKnownCharsSize; j++)
            botKnownChars[i][j] = 0;
    }
    
}


void printBoard()
{
    clear();
    printw("\n");
    for (size_t i = 0; i < boardSize; i++)
    {
        for (size_t j = 0; j < boardSize; j++)
        {
            if(j == 0)
                printw(" ");

            else
                printw("| ");
            

                if ((playerPosY == i && playerPosX == j) && !((playerPosY == selected1Y && playerPosX == selected1X) || (playerPosY == selected2Y && playerPosX == selected2X) ))
                printColoredSymbol('#', selectedTheme);

            else
            {
                if((selected1Y == i && selected1X == j) || (selected2Y == i && selected2X == j) || board[i][j] == ' ')
                    printColoredSymbol(board[i][j], themes[getIndex(usedSimbols, usedSimbolsSize, board[i][j])]);
                else
                    //printColoredSymbol('?', themes[getIndex(usedSimbols, usedSimbolsSize, board[i][j])]);     // cheat colors
                    //printColoredSymbol(board[i][j], 3);                                                       //print open symbol
                    printColoredSymbol('?', 3);   // finally use this 

            }

            printw(" ");
        }
        
        if (i != boardSize -1)
        {
            for (size_t j = 0; j < boardSize; j++)
            {
                if(j == 0)
                    printw(" \n---");
    
                else
                    printw("|---");
            }
        }
        
        printw("\n");
        
    }
}


void printColoredSymbol(char ch, int theme)
{
    attron(COLOR_PAIR(theme));
    printw("%c",ch);
    attroff(COLOR_PAIR(theme));
}

bool moveCursor()
{
    char ch = getch();
        switch (ch) 
        {
            case 119:  
            case 3:
                if (playerPosY > 0)
                    playerPosY--;
                else
                    playerPosY = boardSize - 1;
            break;

            case 115:
            case 2:
                if (playerPosY < boardSize -1)
                    playerPosY++;
                else
                    playerPosY = 0;
            break;

            case 97:
            case 4:
                if (playerPosX > 0)
                    playerPosX--;
                else
                    playerPosX = boardSize - 1;
            break;

            case 100:
            case 5:
                if (playerPosX < boardSize -1)
                    playerPosX++;
                else
                    playerPosX = 0;
            break;

            case 27:
                printw("\nO Kurwa esc pressed\n");
                return false;
            break;    


            case 32:
            case 10:
                matchCheck();   
            break;

            default:
                printw("Key code: %d\n", ch);
            break;
        }
    return true;

}



void matchCheck()
{
    bool changePlayer = true;
    
    if (selected2X != -1)                                                                                       // reset if not 
    {
        selected1X = -1;
        selected1Y = -1; 
        selected2X = -1;
        selected2Y = -1;
    
    }
    
    if (board[playerPosY][playerPosX] == ' ')                                                                   // if empty
        return;
    
    if(board[playerPosY][playerPosX] == '$')                                                                    // if a powerup 
    {
        board[playerPosY][playerPosX] = ' ';
        addPoints();
        return;
    }


    if(selected1X == -1)                                                                                        // check if it is a first card
    {
        selected1X =playerPosX;
        selected1Y = playerPosY;
        return;
    }

    if (selected1X == playerPosX && selected1Y == playerPosY)                                                   // check for bad use 
        return;

    
    selected2X = playerPosX;
    selected2Y = playerPosY;

    if (board[selected1Y][selected1X] == board[selected2Y][selected2X])                                         // if match 
    {
        board[selected1Y][selected1X] = ' ';
        board[selected2Y][selected2X] = ' ';
        addPoints();
        changePlayer = false;
        
        selected1Y = selected1X = selected2Y = selected2X = -1;
    }
    
   
    if(changePlayer && bothPlayersPlay)
        player1Move = !player1Move;
}

void addPoints()
{
    if (bothPlayersPlay && !player1Move)
    {
        player2Score++;
        return;
    }
    player1Score++;
}

void printScore()
{
    if(player1Move)
    {
        attron(COLOR_PAIR(2));
        printw("\nPlayer 1 score: %d\n", player1Score);
        attroff(COLOR_PAIR(2));
    }
    else
    {
        attron(COLOR_PAIR(4));
        printw("Player 2 score: %d\n", player2Score);
        attroff(COLOR_PAIR(4));
    }
}

bool checkLeftFields()
{
    for (size_t i = 0; i < boardSize; i++)
    {
        for (size_t j = 0; j < boardSize; j++)
        {
            if (board[i][j] != ' ')
                return true;
        }
    }
    printWinner();                                                                                              // if all empty print winner 
    return false;
}

void printWinner()
{
    if (player1Score > player2Score)
    {
        attron(COLOR_PAIR(2));
        printw("\nPlayer 1 won with score of: %d\n", player1Score);
        attroff(COLOR_PAIR(2));
        return;
    }
    
    if (player2Score > player1Score)
    {
        attron(COLOR_PAIR(4));
        printw("\nPlayer 2 won with score of: %d\n", player2Score);
        attroff(COLOR_PAIR(4));
        
        return;
    }

    
    attron(COLOR_PAIR(22));
    printw("Draw");
    attroff(COLOR_PAIR(22));

}



void vsComputer()
{
    bothPlayersPlay = true;
    bool keepPlaying = true;

    generateBoardAndAllRelated();
    resetBoard();
    fillBoardAndAllRelated();
    while (keepPlaying)
    {
        printBoard();
        
        printw("\n");
        printScore();



        keepPlaying = checkLeftFields();    
        if(!keepPlaying)
            break;

        if(player1Move)
            keepPlaying = moveCursor();
        printBoard();

        if(!keepPlaying)
            break;


        if (player1Move == 0)
        {
            printw("Bots turn");
            confirm();    
            botMove();
        }
        
        

    }
    confirm();

}

void vsPlayer()
{
    bool keepPlaying = true;
    bothPlayersPlay = true;
    generateBoardAndAllRelated();
    resetBoard();
    fillBoardAndAllRelated();
    while (keepPlaying)
    {
        printBoard();
        printScore();

        keepPlaying = checkLeftFields();    
        if(keepPlaying)
            keepPlaying = moveCursor();
    }
    
    
    confirm();
}

void practice()
{
    bothPlayersPlay = false;                                // yes the only difference between this and vsPlayer is this line  theoretically i could change it to accept argument but for this i would have to change all other function to accept it, so idk what is better
    player1Move = true;
    bool keepPlaying = true;

    generateBoardAndAllRelated();
    resetBoard();
    fillBoardAndAllRelated();

    while (keepPlaying)
    {
        printBoard();
        printScore();

        keepPlaying = checkLeftFields();    
        if(keepPlaying)
            keepPlaying = moveCursor();
    }
    confirm();

}

void botMove()
{
    if (selected2X != -1 && board[selected1Y][selected1X] != ' ')                                               // store opened cards
    {
        botKnownChars[selected1Y][selected1X] = 1;
        botKnownChars[selected2Y][selected2X] = 1;

    }

    for (size_t i = 0; i < boardSize; i++)                                                                      // add empty fields 
    {
        for (size_t j = 0; j < boardSize; j++)
        {
            if (board[i][j] == ' ')
                botKnownChars[i][j] = -1;     
            
            
        }
        
    }
    
    // findig a pair from remembered board

    for (int i1 = 0; i1 < boardSize; i1++) 
    {
        for (int j1 = 0; j1 < boardSize; j1++) 
        {
            for (int i2 = i1; i2 < boardSize; i2++) 
            {
                int start_j = (i2 == i1) ? j1 + 1 : 0;
                for (int j2 = start_j; j2 < boardSize; j2++) 
                {
                    if(botKnownChars[i1][j1] == 1 && botKnownChars[i2][j2] == 1)
                    {
                        if (board[i1][j1] == board[i2][j2]) {
                            board[i1][j1] = ' ';
                            board[i2][j2] = ' ';
                            botKnownChars[i1][j1] = -1;
                            botKnownChars[i2][j2] = -1;
                            selected1Y = selected1X = selected2Y = selected2X = -1;
                            player2Score++;
                            return;  
                        }
                    }
                    
                    
                    
                }
            }
        }
    }


    // guessing unknown cards

    int x1,y1,x2,y2;
    do
    {
        y1 = rand() % boardSize;
        x1 = rand() % boardSize;

        if(board[y1][x1] == '$')           // to handle "$"
        {   
            player2Score++;
            board[y1][x1] = ' ';
            botKnownChars[y1][x1] = -1;
            if(!checkLeftFields())
                return;
        }
    } while (botKnownChars[y1][x1] != 0);
    botKnownChars[y1][x1] = 1;
    selected1X = x1;
    selected1Y = y1;

    
    do
    {
        y2 = rand() % boardSize;
        x2 = rand() % boardSize;
        if(board[y2][x2] == '$')           // to handle "$"
        {   
            player2Score++;
            board[y2][x2] = ' ';
            botKnownChars[y2][x2] = -1;
            if(!checkLeftFields())
                return;
            
        }
    } while (botKnownChars[y2][x2] == -1 ||  (y2 == y1 && x2 == x1));
    botKnownChars[y2][x2] = 1;
    selected2X = x2;
    selected2Y = y2;

    if(board[y1][x1] == board[y2][x2])
    {
        board[y1][x1] = ' ';
        board[y2][x2] = ' ';
        selected1Y = selected1X = selected2Y = selected2X = -1;
        botKnownChars[y1][x1] = botKnownChars[y2][x2] = -1;
        player2Score++;
        return;
        //botMove();
    }



    player1Move = 1;
}

void confirm()
{
    printw("\nPress anything to continue\n");
    getch();   // just to pause clearing before new cycle
    refresh();
    clear(); 
}


void freeArrays()
{
    for (size_t i = 0; i < boardSize; i++)
    {
        free(board[i]);
    }
    free(board);
    free(usedSimbols);
    free(themes);
}

void quit()
{
    printw("thank you for playing this game\n");
    confirm();
    //freeArrays();
    endwin();
    exit(0);
}

void customization()
{   
    
    char key;
    do
    {   
        clear();
        attron(COLOR_PAIR(selectedTheme));
        printw("THIS IS A DEMO TEXT FOR THEME TESTING\nPress up or down keys to change themes\n");
        attroff(COLOR_PAIR(selectedTheme)); // Turn off color text
        //printw("%d \n", selectedTheme);


        refresh();
        key = getch();
        switch (key)
        {
            case 115:
            case 2:
                if(selectedTheme > 0)
                selectedTheme --;
                else
                    selectedTheme = 26;
            break;

            case 119:
            case 3:
                if(selectedTheme < 26)
                selectedTheme ++;    
                else 
                    selectedTheme = 1;

            break;
            
            case 27:
                return;
            break;

            case 16:
                clear();
                printw("Wow, you found secret menu!\n(I hope you are not reading this from the source code, otherwise you will delete it)\nThe author of this masterpiece is Andreish12\n");
                confirm();
            break;
            
            default:
                //printw("Key code: %d\n", key);
                // confirm();
            break;        
        }
    } while (key != 10 && key != 32);

    uploadThemeToFile();
}

void uploadThemeToFile()
{
    FILE *settingsFile = fopen("settings for memory game.txt", "w");
    if(settingsFile == NULL)
    {
        printw("а кто это тут навалил такую кучу?");
        return;
    }

    fprintf(settingsFile, "%d", selectedTheme);
    fclose(settingsFile);
}

void downloadThemeFromFile()
{
    FILE *settingsFile = fopen("settings for memory game.txt", "r");
    if(settingsFile == NULL)
        return;
    

    int num;
    while (fscanf(settingsFile, "%d", &num) == 1) 
    {
        printw("Read number: %d\n", num);
        selectedTheme = num;
        confirm();
    }
    fclose(settingsFile);
}
