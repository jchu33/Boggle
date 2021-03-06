# Boggle
Takes in a dictionary of words and finds all words and number of occurrences in a Boggle grid

The standard input contains one word per line, each containing only alphabetic
characters.  These words serve as the "Boggle dictionary".

The characters in the array are stored in the command-line argument BOARD by
rows.  For example,

      % Boggle 3 4 ABCDEFGHIJKL

specifies the board

      A B C D
      E F G H
      I J K L

since NROWS = 3 and NCOLS = 4.

A word is a Boggle word if its letters can be spelled out by starting at one
square on the board and passing through a sequence of squares that is connected
horizontally, vertically, or diagonally.  That is, you cannot skip intervening
squares or remain on the same square to get the next letter.  For example, the
board

      E P I
      P R O
      U S T

contains the words PEPPER, SUPERIOR, and REPERTOIRE among others, but not
PEEPER or PROUST.  When comparing letters, case is ignored and a _ on the board
matches any letter (a la the blank tile in Scrabble).  For example, the board

      E P I
      _ R O
      U S T

also contains the words PEEPER and PROUST, but not JOUST.

The Boggle words found are printed out in alphabetical order, in lower case,
along with the number of times that they appear, using the format "%s: %d\n".
For example, for the EPIPROUST board and the list of words above:

      pepper: 4
      repertoire: 2
      superior: 1
      
And for the EPI_ROUST board,

      peeper: 11
      pepper: 12
      proust: 4
      repertoire: 16
      superior: 4

With the -c flag all non-Boggle words in the dictionary are printed using the
format "%s\n".

The real Boggle also requires that letters/wildcards on the board cannot
be used more than once to form a word.  This behavior is invoked by the -t
flag.
