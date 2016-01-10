#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 80

int main()
{
  /* The pointer to hold output string*/
  char *outputstr;
  /* Char that is read in input*/
  char input;
  /* Array index*/
  int counter = 0;
  /* Allocate memory for the string*/
  outputstr = malloc(sizeof(char) * LENGTH);

  //Read input by char until end of file is reached
  while ((input = getchar()) != EOF)
    {
      //If the read input is a new line character then add a space instead
      if (input == '\n')
	{
	  outputstr[counter++] = ' ';
	}
      //If a star is found and the previous char is a star
      else if (input == '*' && counter > 0 && outputstr[counter - 1] == '*') 
	{
	  outputstr[counter - 1] = '^';
	}
      else
	{
	  //Increment to next char and set as the input char
	  outputstr[counter++] = input;
	}

      /* When the length of the outputstr becomes LENGTH (80)*/
      if (strlen(outputstr) == LENGTH)
	{
	  /* Print string, clear the string, and reset counter*/
	  outputstr[counter - 1] = '\n';
	  printf("%s", outputstr);
	  memset(outputstr, 0, LENGTH);
	  counter = 0;
	}
    }

  /* Free up the pointer */
  free(outputstr);
  outputstr = 0;

  return 0;
}
