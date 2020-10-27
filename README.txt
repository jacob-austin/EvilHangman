      ___           ___           ___          _____          ___           ___     
     /  /\         /  /\         /  /\        /  /::\        /__/\         /  /\    
    /  /::\       /  /:/_       /  /::\      /  /:/\:\      |  |::\       /  /:/_   
   /  /:/\:\     /  /:/ /\     /  /:/\:\    /  /:/  \:\     |  |:|:\     /  /:/ /\  
  /  /:/~/:/    /  /:/ /:/_   /  /:/~/::\  /__/:/ \__\:|  __|__|:|\:\   /  /:/ /:/_ 
 /__/:/ /:/___ /__/:/ /:/ /\ /__/:/ /:/\:\ \  \:\ /  /:/ /__/::::| \:\ /__/:/ /:/ /\
 \  \:\/:::::/ \  \:\/:/ /:/ \  \:\/:/__\/  \  \:\  /:/  \  \:\~~\__\/ \  \:\/:/ /:/
  \  \::/~~~~   \  \::/ /:/   \  \::/        \  \:\/:/    \  \:\        \  \::/ /:/ 
   \  \:\        \  \:\/:/     \  \:\         \  \::/      \  \:\        \  \:\/:/  
    \  \:\        \  \::/       \  \:\         \__\/        \  \:\        \  \::/   
     \__\/         \__\/         \__\/                       \__\/         \__\/

Author: Jacob Austin
Date: April 27, 2020

# Evil Hangman

This project is intended to create a hangman game where the game will cheat and always attempt to make the user fail.

## Libraries

The program only uses these standard C libraries:
errno.h
stdlib.h
stdio.h

## Method

The program creates word vectors of words of every length given in a dictionary text file. The user can then select the length of word they want to guess, and the number of guesses they want. Once the user makes a guess, the program creates a "key" of where that letter appears in each word in the selected vector, then creates an associative array using an AVL tree of each key and all the words that fit that key. The program then selects the vector with the largest size, attempting to retain possibilities to fail the user. It is possible to win, upon selection of a small enough initial vector and given enough guesses. However, the program attempts to make the user fail as often as possible.

