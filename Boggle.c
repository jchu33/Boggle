/*
Boggle implements a trie to store all the words in the dictionary, and then conducts depth first search on that trie when
trying to find words from the Boggle grid.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "/c/cs223/Hwk3/getLine.h"

typedef struct tnode{
    bool isWord;
    int count;
    struct tnode *letters[26];
        //struct tnode *letters[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
} tnode;

typedef struct tnode Trie;

Trie head;
long ROWS;
long COLS;
bool CFLAG = false;
bool TFLAG = false;

// trim the new line character at the end of each word
void trim(char *s)
{
    char *index;
    if ((index = strchr(s, '\n')) != NULL)
    {
        *index = '\0';
    }
}

// prints out all words found in the Boggle grid to standard output; if -c flag is specified, then print all words NOT found
void printWords(Trie *t, char **word, int size)
{
    for (int i = 0; i < 26; i++)
    {
        if (t->letters[i] != NULL)
        {
            bool doubled = false;
            // need to malloc more room, double the storage
            if (strlen(*word) + 1 == size)
            {
                doubled = true;
                *word = realloc(*word, ((size * 2) + 1) * sizeof(char));
                (*word)[strlen(*word)] = ((char) (i + 97));
                (*word)[strlen(*word) + 1] = '\0';
                size = size * 2;
            }
            // dont need to malloc more room
            else
            {
                if (strlen(*word) == 0)
                {
                    (*word)[0] = ((char) i + 97);
                    (*word)[1] = '\0';
                }
                else
                {
                    (*word)[strlen(*word)] = ((char) i + 97);
                    (*word)[strlen(*word) + 1] = '\0';
                }
            }
            // count > 0 and not -c flag
            if (t->letters[i]->count > 0 && !CFLAG)
            {
                printf("%s: %d\n", *word, t->letters[i]->count);
            }
            // count equals to 0 and -c flag
            else if (t->letters[i]->count == 0 && t->letters[i]->isWord && CFLAG)
            {
                printf("%s\n", *word);
            }
            printWords(t->letters[i], word, size);
            // now remove letter
            (*word)[strlen(*word) - 1] = '\0';
            if (doubled)
            {
                *word = realloc(*word, ((size/2) + 1) * sizeof(char));
                size = size/2;
            }
        }
    }
}

// iterates through the Boggle board, forming new "words" and then testing these words on the trie; if the addition of a new letter
// leads to a null trie, then don't add that letter and find another letter to add.
// if -t flag is specified, then each letter in the grid can be used only once.
void checkWord(Trie *t, int row, int col, char grid[ROWS][COLS], bool seen[ROWS][COLS])
{
    for (int dx = ((col > 0)? -1 : 0); dx <= ((col < (COLS - 1))? 1 : 0); dx++)
    {
        for (int dy = ((row > 0)? -1 : 0); dy <= ((row < (ROWS - 1 ))? 1 : 0); dy++)
        {
            if (dx != 0 || dy != 0)
            {
                if ((TFLAG && seen[row + dy][col + dx] == false) || (!TFLAG))
                {
                    char c = grid[row + dy][col + dx];
                    seen[row + dy][col + dx] = true;
                    // letter and not a wildcard
                    if (c != '_')
                    {
                        int val = ((int) c) - 97;
                        if (t->letters[val] != NULL)
                        {
                            if (t->letters[val]->isWord)
                            {
                                t->letters[val]->count += 1;
                            }
                            checkWord(t->letters[val], row + dy, col + dx, grid, seen);
                        }
                    }
                    // wildcard
                    else
                    {
                        for (int i = 0; i < 26; i++)
                        {
                            if (t->letters[i] != NULL)
                            {
                                if (t->letters[i]->isWord)
                                {
                                    t->letters[i]->count += 1;
                                }
                                checkWord(t->letters[i], row + dy, col + dx, grid, seen);
                            }
                        }
                    }
                    seen[row + dy][col + dx] = false;
                }
            }
        }
    }
}

// insert the char *word into the trie t
void insertWord(Trie *t, char* word)
{
    char c = word[0];
    if (c == '\0')
    {
        return;
    }
    else
    {
        int val = ((int) c) - 97;
        if (t->letters[val] == NULL && word[1] == '\0')
        {
            Trie *newLetter = malloc(sizeof(tnode));
            newLetter->count = 0;
            newLetter->isWord = true;
            for (int i = 0; i < 26 ; i++)
            {
                newLetter->letters[i] = NULL;
            }
            t->letters[val] = newLetter;;
        }
        else if (t->letters[val] == NULL && word[1] != '\0')
        {
            Trie *newLetter = malloc(sizeof(tnode));
            newLetter->count = 0;
            newLetter->isWord = false;
            for (int i = 0; i < 26 ; i++)
            {
                
                newLetter->letters[i] = NULL;
            }
            t->letters[val] = newLetter;
            insertWord(newLetter, word + 1);
        }
        else if (t->letters[val] != NULL && word[1] == '\0')
        {
            t->letters[val]->isWord = true;
        }
        else
        {
            insertWord(t->letters[val], word + 1);
        }
    }
}

int main(int argc, char* argv[])
{
    char *board;
    // argument handling
    if (argc != 4 && argc != 5)
    {
        fprintf(stderr, "%s\n", "Boggle: usage: Boggle [-c] [-t] nRows nCols board");
        exit(EXIT_FAILURE);
    }
    else
    {
        int flagIndex, colIndex, rowIndex, boardIndex;
        // no -t or -c flags
        if (argc == 4)
        {
            flagIndex = -1;
            rowIndex = 1;
            colIndex = 2;
            boardIndex = 3;
        }
        else
        {
            flagIndex = 1;
            rowIndex = 2;
            colIndex = 3;
            boardIndex = 4;
        }
        // if has five argument terms, the second one should be a -t or -c
        if (flagIndex != -1)
        {
            if (strcmp(argv[flagIndex], "-t") != 0 && strcmp(argv[flagIndex], "-c") != 0)
            {
                fprintf(stderr, "%s\n", "Boggle: usage: Boggle [-c] [-t] nRows nCols board");
                exit(EXIT_FAILURE);
            }
            if (strcmp(argv[flagIndex], "-t") == 0)
            {
                TFLAG = true;
            }
            else if (strcmp(argv[flagIndex], "-c") == 0)
            {
                CFLAG = true;
            }
        }
        // handling invalid row input
        char *ptr = NULL;
        ROWS = strtol(argv[rowIndex], &ptr, 10);
        if (ROWS < 0 || ROWS == 0)
        {
            fprintf(stderr, "%s\n", "Boggle: invalid nr");
            exit(EXIT_FAILURE);
        }
        else if (!ptr)
        {
            fprintf(stderr, "%s\n", "Boggle: invalid nr");
            exit(EXIT_FAILURE);
        }
        
        // handling invalid col input
        char *ptr2 = NULL;
        COLS = strtol(argv[colIndex], &ptr2, 10);
        if (COLS < 0 || COLS == 0)
        {
            fprintf(stderr, "%s\n", "Boggle: invalid nc");
            exit(EXIT_FAILURE);
        }
        else if (!ptr2)
        {
            fprintf(stderr, "%s\n", "Boggle: invalid nc");
            exit(EXIT_FAILURE);
        }
        
        // board does not contain nRow x nCol characters
        if (strlen(argv[boardIndex]) != ROWS * COLS)
        {
            fprintf(stderr, "%s\n", "Boggle: board is not nRows x nCols");
            exit(EXIT_FAILURE);
        }
        // board contains a char that is not _ and is not in the alphabet
        int index = 0;
        while (argv[boardIndex][index] != '\0')
        {
            if (!isalpha(argv[boardIndex][index]) && argv[boardIndex][index] != '_')
            {
                fprintf(stderr, "%s\n", "Boggle: board must be alphabetic or blank");
                exit(EXIT_FAILURE);
            }
            index++;
        }
        
        // copy the board input to string board
        board = malloc((strlen(argv[boardIndex]) + 1) * sizeof(char));
        strcpy(board, argv[boardIndex]);
        // convert board to lower case letters
        for (int i = 0; board[i]; i++)
        {
            if (board[i] != '_')
            {
                board[i] = tolower(board[i]);
            }
        }
    }
    
    
    // head is the head node of the trie
    head.count = 0;
    head.isWord = false;
    for (int i = 0; i < 26 ; i++)
    {
        head.letters[i] = NULL;
    }
    
    // add all words given by standard input to the trie
    char *inputString;
    inputString = getLine(stdin);
    while (inputString != NULL)
    {
        bool addWord = true;
        trim(inputString);
        for (int i = 0; inputString[i]; i++)
        {
            if (!isalpha(inputString[i]))
            {
                addWord = false;
            }
            inputString[i] = tolower(inputString[i]);
        }
        if (addWord)
        {
            insertWord(&head, inputString);
        }
        free(inputString);
        inputString = getLine(stdin);
    }
    
    if (inputString != NULL)
    {
        free(inputString);
    }
    
    // fill out the grid representation of the word
    char grid[ROWS][COLS];
    int count = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            grid[i][j] = board[count];
            count++;
        }
    }
    free(board);
    
    // boolean array for t flag
    bool seen[ROWS][COLS];
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            seen[i][j] = false;
        }
    }
    
    // first step in finding words in the Boggle grid; iterate through each index as potential starting point of word
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            seen[i][j] = true;
            // not a wildcard
            if (grid[i][j] != '_')
            {
                int val = ((int) grid[i][j]) - 97;
                if (head.letters[val] != NULL)
                {
                    if (head.letters[val]->isWord)
                    {
                        head.letters[val]->count += 1;
                    }
                    checkWord(head.letters[val], i, j, grid, seen);
                }
            }
            // wildcard
            else
            {
                for (int k = 0; k < 26; k++)
                {
                    if (head.letters[k] != NULL)
                    {
                        if (head.letters[k]->isWord)
                        {
                            head.letters[k]->count += 1;
                        }
                        checkWord(head.letters[k], i, j, grid, seen);
                    }
                }
            }
            seen[i][j] = false;
        }
    }
    char *str = calloc(101, sizeof(char));
    printWords(&head, &str, 100);
    free(str);
    
    return 0;
}
