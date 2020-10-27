#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "my_string.h"
#include "vector.h"
#include "avl.h"

#define BUFFSIZE 8192
#define INT_MAX 2147483646

void clear_keyboard_buffer();

int get_word_length(VECTOR arr[]);

int get_guesses();

char get_letter(char taken[]);

int get_play_again();

int get_display_word_total();

void print_guessed_letters(char taken[]);

int main(int argc, char* argv[])
{
    VECTOR arr[30];
    char guessed_letters[30];
    MY_STRING hMy_string = NULL;
    FILE* fp;
    int i, count, guesses, length, play_again, display_word_total, win, num;
    char letter;

    for(i = 0; i < 30; i++) //make vectors of each word length
    {
	arr[i] = NULL;
	arr[i] = vector_init_default();
    }

    hMy_string = my_string_init_default();
    fp = fopen("dictionary.txt", "r");

    while(my_string_extraction(hMy_string, fp))
    {
	if(my_string_get_size(hMy_string))
	{
	    vector_push_back(arr[my_string_get_size(hMy_string)], (Item*) hMy_string);
	}
    }
	
    do //Game loop
    {
	MY_STRING old_key = NULL;
	MY_STRING new_key = NULL;
  	VECTOR cur_word_family = NULL;

	win = 0;
	num = 0;
	
	for(i = 0; i < 29; i++) //clear guessed letters
	{
	    guessed_letters[i] = '\0';
	}
	length = get_word_length(arr); //prompt for word length
	
	old_key = my_string_init_default(); //initialize new key and old key for given length of word
	new_key = my_string_init_default();
	for(i = 0; i < length; i++)
	{
	    my_string_push_back(old_key, '-');
	    my_string_push_back(new_key, '-');
	}
	
	guesses = get_guesses(); //prompt for number of guesses

	display_word_total = get_display_word_total(); //prompt for running total of words left

	count = 0;
	
	while(guesses) //guesses loop
	{
	    AVL hAvl = NULL;
	    print_guessed_letters(guessed_letters);
	    letter = get_letter(guessed_letters); //prompt for letter to guess
	    guessed_letters[count] = letter;
	    hAvl = avl_init_default();
	    
	    if(!count)
	    {
		cur_word_family = vector_init_default();
		vector_copy(cur_word_family, arr[length]);
		//make copy of original vector, saves time on multiple plays as it allows one time
		// initialization of array of vectors
	    }
	    
	    for (i = 0; i < vector_get_size(cur_word_family); i++)
	    {
		if (FAILURE == get_word_key_value(old_key, new_key, vector_at(cur_word_family, i), letter))
		{
		    fprintf(stderr, "Error getting key value in guesses loop");
		}
		avl_insert(hAvl, new_key, vector_at(cur_word_family, i));
	        my_string_clear_key(new_key);
	    }
	    
	    if(display_word_total)
	    {
		avl_print(hAvl);
		if(avl_max_size(hAvl) == 1)
		{
		    printf("\nThere is 1 possibility remaining\n");
		}
		else
		{
		    printf("\nThere are %d possibilities remaining\n", avl_max_size(hAvl));
		}
	    }
	    printf("\nWord: %s\n\n", my_string_c_str(avl_max_key(hAvl)));
	    
	    
	    if(my_string_compare(avl_max_key(hAvl), old_key) < 0)//correct guess, update key
	    {
		my_string_assignment((Item*) &old_key, (Item*) avl_max_key(hAvl));
	        if(vector_get_size(cur_word_family) <= 1 && !key_has_hyphen(old_key))
	        {
		    win = 1;
		    avl_destroy(&hAvl);
		    break;
		}
	    }
	    else
	    {
		guesses--; //One guess has been used after wrong guess
	    }
	    
	    printf("You have %d guesses left.\n", guesses);

	    vector_destroy(&cur_word_family);
	    cur_word_family = vector_init_default();
	    avl_copy_max_vector(hAvl, cur_word_family);
	    avl_destroy(&hAvl);



	    if(guessed_letters[25] != '\0')
	    {
		printf("Sorry, you've run out of letters!\n");
		break;
	    }
	    count++;
	}
	if(win)
	{
	    printf("\n\nCongradulations, you win! the word was: %s\n\n", my_string_c_str(vector_at(cur_word_family, 0)));
	}
	else
	{
	    num = rand() % vector_get_size(cur_word_family); //find random word to display
	    printf("\n\nSorry, the word was: %s\n\n", my_string_c_str(vector_at(cur_word_family, num)));
	}
	vector_destroy(&cur_word_family);

	play_again = get_play_again(); //prompt for play again

	my_string_destroy(&old_key);
	my_string_destroy(&new_key);
    }while(play_again);

    for(i = 0; i < 30; i++) //destroy 30 vectors of words
    {
	vector_destroy((Item*) &arr[i]);
    }

    my_string_destroy((Item*) &hMy_string);
    fclose(fp);
    
    return 0;
}

int get_word_length(VECTOR arr[])
{
    char buff[BUFFSIZE];
    char *end;
    long number;
    int valid_input = 0;
    int length;
    do
    {
	printf("Enter the length of the word for HANGMAN: ");
	if (fgets (buff, sizeof buff, stdin) == NULL)
	{
	    fprintf(stderr, "error getting input");
	    return 0;
	}

	errno = 0;
	number = strtol(buff, &end, 10);
	
	if(number > 1 && number < 30 && !errno)
	{
	    if(!vector_get_size(arr[number]))
	    {
		printf("Sorry, there are no words of that size. Please try again.\n");
	    }
	    else
	    {
		length = (int) number;
		valid_input = 1;
	    }
	}
	else
	{
	    printf("Sorry, there are no words of that size. Please try again.\n");
	}
	
    }while(!valid_input);

    return length;
}

int get_guesses()
{
    char buff[BUFFSIZE];
    char *end;
    int valid_input = 0;
    long number;
    int guesses;
    do
    {
        printf("Enter number of guesses you get: ");
        if (fgets (buff, sizeof buff, stdin) == NULL)
	{
	    fprintf(stderr, "error getting input");
	    return 0;
	}

	errno = 0;
	number = strtol(buff, &end, 10);
	if(number > 0 && number <= INT_MAX && !errno)
	{
	    guesses = (int)number;
	    valid_input = 1;
	}
	else if(number >= INT_MAX || errno)
	{
	    printf("Not very confident, huh? I'm sure you could get it in two billion guesses or so. Try again.\n");
	}
	else
	{
	    printf("Please enter a positive number.\n");
	}
	
     }while(!valid_input);

    return guesses;
}

char get_letter(char taken[])
{
    int valid_input = 0;
    int i;
    int is_taken;
    char letter;
    char is_second_letter; //use to check for input of a single char
    do
    {
	printf("Please enter a letter to guess: ");
        letter = fgetc(stdin);
	is_second_letter = fgetc(stdin);
	is_taken = 0;
	
	if(is_second_letter == '\n') //check input only if exactly one char is entered
	{
	    if(letter >= 65 && letter <= 90)
	    {
		
		letter += 32; //make all letters lowercase
	    }
	    if(letter >= 97 && letter <= 122)
	    {
		for(i = 0; taken[i]; i++)
		{
		    if(taken[i] == letter)
		    {
			is_taken = 1;
			break;
		    }
		}
		
		if(!is_taken)
		{
		    valid_input = 1;
		}
	    }
	}
	else
	{
	    clear_keyboard_buffer();  //clear input buffer if multiple characters are entered
	}
	
	if(is_taken)
	{
	    printf("Sorry, you've already guessed that letter! Please try again.\n");
	}
	else if(!valid_input)
	{
	    printf("Sorry, that's not a letter of the alphabet. Please try again.\n");
	}
	
    }while(!valid_input);

    return letter;
}

int get_play_again()
{
    char letter;
    char is_second_letter;
    int valid_input = 0;
    int play_again;

    do
    {
	printf("Play again? (y/n): ");
	letter = fgetc(stdin);
	is_second_letter = fgetc(stdin);
	
	if(is_second_letter == '\n') //check input only if exactly one char is entered
	{
	    if(letter >= 65 && letter <= 90)
	    {
		
		letter += 32; //make all letters lowercase
	    }
	    if(letter >= 97 && letter <= 122)
	    {
		if(letter == 'y')
		{
		    play_again = 1;
		    valid_input = 1;
		}
		else if(letter == 'n')
		{
		    play_again = 0;
		    valid_input = 1;
		}
		else
		{
		    printf("Please enter y or n\n");
		}
	    }
	}
	else
	{
	    clear_keyboard_buffer();  //clear input buffer if multiple characters are entered
	    printf("Please enter y or n\n");
	}
    }while(!valid_input);

    return play_again;
}

int get_display_word_total()
{
    char letter;
    char is_second_letter;
    int valid_input = 0;
    int display_word_total;

    do
    {
	printf("Display running total of words? (y/n): ");
	letter = fgetc(stdin);
	is_second_letter = fgetc(stdin);
	
	if(is_second_letter == '\n') //check input only if exactly one char is entered
	{
	    if(letter >= 65 && letter <= 90)
	    {
		
		letter += 32; //make all letters lowercase
	    }
	    if(letter >= 97 && letter <= 122)
	    {
		if(letter == 'y')
		{
		    display_word_total = 1;
		    valid_input = 1;
		}
		else if(letter == 'n')
		{
		    display_word_total = 0;
		    valid_input = 1;
		}
		else
		{
		    printf("Please enter y or n\n");
		}
	    }
	}
	else
	{
	    clear_keyboard_buffer();  //clear input buffer if multiple characters are entered
	    printf("Please enter y or n\n");
	}
    }while(!valid_input);

    return display_word_total;
}

void print_guessed_letters(char taken[])
{
    int i = 0;
    if(taken[i])
    {
	printf("Used letters: ");
	for(i = 0; taken[i]; i++)
	{
	    if(taken[i + 1])
	    {
		printf("%c, ", taken[i]);
	    }
	    else
	    {
		printf("%c", taken[i]);
	    }
	}
	printf("\n");
    }
}

void clear_keyboard_buffer()
{
    char c;
    scanf("%c", &c);
    while (c != '\n' && c != '\0')
    {
	scanf("%c", &c);
    }
}
