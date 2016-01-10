#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "threadfuncs.h"

void *OutputFunc(void *args)
{
  if (args == NULL)
    return;

	ThreadInit *Init = args;

	char output_line[80];
	int counter = 0;
	//Set the last character of every line to the new line character
	char output;
	do
	  {
	    /* Get input buffer value and add to the 80 char string*/
	    output = remoove(Init->input_buffer);
	    output_line[counter] = output;
	    /* Once we have 80 characters output the string and reset the counter*/
	    if ( counter == 80 )
	      {
		fprintf(stdout, "%s\n", output_line);
		counter = 0;
		continue;
	      }

	    ++counter;
	  }
	while (output != '\0');
}

void *CrunchStringFunc(void *args)
{
  if (args == NULL)
    return;

  ThreadInit *Init = args;

  char lastChar;
  char currentChar;

  /* Run until the null character is found from input buffer */
  do
    {
      currentChar = remoove(Init->input_buffer);
      
      //If the current char is not a star then deposit it into output
      if (currentChar != '*')
	{
	  if (lastChar == '*')  //If previous char was a star we have to add that back into buffer
	    deposit(Init->output_buffer, '*');

	  deposit(Init->output_buffer, currentChar);
	  lastChar = currentChar;
	}
      else   //If the current char is a star
	{
	  if (lastChar == '*')  //And the last char is a star
	    {
	      deposit(Init->output_buffer, '^');  //Then output a caret
	      lastChar = '^';   //Set last char to ^
	    }
	  else
	      lastChar = currentChar;
	}
    }
  while (currentChar != '\0');
}

void *RemoveCarriageFunc(void *args)
{
  if (args == NULL) //Arg check
    return;

  ThreadInit *Init = args;

  /* Run until a null value is pulled through the input buffer*/
	char read;
	do
	{
	  //Read a value from the input buffer
	  read = remoove(Init->input_buffer);
	  //Replace all carriage returns with a space
	  if (read == '\n')
	    read = ' ';
	  //Deposit character back into output buffer
	  deposit(Init->output_buffer, read);
	}
	while (read != '\0');
}

void *InputThreadFunc(void *args)
{
  //Check the argument ptr coming in
  if (args == NULL)
    return;

	/* Convert argument pointer into ThreadInit*/
	ThreadInit *Init = args;

	char input;
	/* Read every character from stream until end of file*/
	while ((input = getchar()) != EOF)
	  {
	    deposit(Init->output_buffer, input);
	  }
	
	/* Pass down a null value to signal threads should quit*/
	deposit(Init->output_buffer, '\0');
}
