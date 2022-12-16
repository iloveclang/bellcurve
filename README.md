# bellcurve

I had to make an account because people don't appreciate  
the subtle genius of using reddit comments as a respository.

# [DOWNLOAD](https://github.com/iloveclang/bellcurve/releases/tag/banana)

## About

bellcurve is a small meme program made with the standard C library.  
It draws a bellcurve meme interpetation a set number of times.  

## Usage
$ bellcurve filepath loops
  - [filepath] - a file with newline-delimited opinions alternating between positive and negative ones
  - [loops] - number of times the program will draw a randomized scenario with opinions, wojaks, and the bell-curve

## Features:
- amazing revolutionary functions like:  
  - strnull() - fills string with '\0' characters
  - strncpylen() - formerly known as non_sh*t_strncpy(), it does the amazing feat of returning the number of chars copied. Amazing. 
  - randomn() - really proud of this one because I naturally thought of it without ever having seen people using modulo remainder as a way of getting a range from 0 to n-1.
- freeing allocated memory
- draw_face() function can color and flip faces so they always have the correct orientation for the meme
- the bell curve gets drawn in a random color each loop
- the opinions from the file get applied to the meme in random order
- the faces get applied to the meme in random order
- bitmasking
- 4d chess randomization algorithms 
- no segfaults (afaik, tested with valgrind) 
- no weird header files you need to link (like math.h). Compilation is simple.
- cross-platform? (haven't tested it)

## Files:
- old_main.c - the OG file where it all began - ignore
- main.c - this is the file with the code you need to compile
- opinions.txt - a short example input file that needs to be passed to the program together with the number of loops
