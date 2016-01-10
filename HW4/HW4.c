/**
 * Dowon Cha
 * COMP 530
 * I pledge i did not copy this code
 * This program will read in input from the user/file, convert all carriage returns into a space
 * Convert 2 consecutive stars into a ^, and then output 80 character lines to the user.
 */

/* Standard includes*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void OutputFunc(int PipeRead)
{
	/* Open the read pipe as a File stream to read from*/
	FILE *instream;
	if ((instream = fdopen(PipeRead, "r")) == NULL)
	{
		printf("Output func stream error\n");
		exit(EXIT_FAILURE);
	}

	char output_line[80];
	int counter = 0;
	//Set the last character of every line to the new line character
	char output;	
	do
	  {
	    output = getc(instream);
	    output_line[counter++] = output;
	    /* Once we have 80 characters output the string and reset the counter*/
	    if ( counter == 81 )
	      {
			printf("%s\n", output_line);
			counter = 0;
			continue;
	      }
	  }
	while (output != '\0');
	
	fclose(instream);
	exit(EXIT_SUCCESS);
}

/* This function will crunch 2 stars into a ^ */
/* If a star is read and the 2nd one is not a star then we have to remember to put 1st star back in*/
void CrunchStringFunc(int PipeRead, int PipeWrite)
{
	assert(PipeRead != 0);
	assert(PipeWrite != 0);
	
	/* Open up pipe file descriptors as file streamers to read and write from*/
	FILE *instream, *outstream;
	instream = fdopen (PipeRead, "r");
	outstream = fdopen (PipeWrite, "w");

	if (instream == NULL || outstream == NULL)
	{
		printf("Crunch string function stream error\n");
		exit(EXIT_FAILURE);
	}

	char lastChar;
	char currentChar;

  /* Run until the null character is found from input buffer */
  do
    {
      /* Get the current char in the stream*/
      currentChar = getc(instream);
      
      //If the current char is not a star then deposit it into output
      if (currentChar != '*')
		{
			if (lastChar == '*')  //If previous char was a star we have to add that back into buffer
				putc('*', outstream);

			putc(currentChar, outstream);
			lastChar = currentChar;
		}
      else   //If the current char is a star
		{
			if (lastChar == '*')  //And the last char is a star
			{
				putc('^', outstream);  //Then output a caret
				lastChar = '^';   //Set last char to ^
			}	
			else
				lastChar = currentChar;
		}
    }
  while (currentChar != '\0');
  
  fclose(instream);
  fclose(outstream);
  exit(EXIT_SUCCESS);
}

/* This function will remove a carriage return and turn it into a space*/
void RemoveCarriageFunc(int PipeRead, int PipeWrite)
{
	assert(PipeRead != 0);
	assert(PipeWrite != 0);
	
	/* Open up pipe descriptors as file streams*/
	FILE *instream, *outstream;
	instream = fdopen (PipeRead, "r");
	outstream = fdopen (PipeWrite, "w");

	if (instream == NULL || outstream == NULL)
	{
		printf("Remove carriage stream null\n");
		exit(EXIT_FAILURE);
	}

  /* Run until a null value is pulled through the input buffer*/
	char read;
	do
	{
	  //Read a value from the input buffer
	  read = getc(instream);
	  //Replace all carriage returns with a space
	  if (read == '\n')
	    read = ' ';
	  //Deposit character back into output buffer
		putc(read, outstream);
	}
	while (read != '\0');
	
	fclose(instream);
	fclose(outstream);
	exit(EXIT_SUCCESS);
}

void InputThreadFunc(int PipeWrite)
{

  /* Open up file descriptor as a file stream and check for errors*/
  FILE *outstream;
	if ((outstream = fdopen(PipeWrite, "w")) == NULL)
	{
		printf("Cannot open Input thread output stream\n");
		exit(EXIT_FAILURE);
	}
	
	char *line = NULL;     //The line to be read from stdin
	size_t len = 0;        //Length of the line determined by getline
	ssize_t read;          //hold the return value
	/* Read every line from stdin until end of file*/
	while ((read = getline(&line, &len, stdin)) != -1) {
		fprintf(outstream, "%s", line);
	}
	
	/* Pass down a null value to signal threads should quit*/
	putc('\0', outstream);
	
	free(line);
	fclose(outstream);
	exit(EXIT_SUCCESS);	
}

int main(int argc, char const *argv[])
{
	pid_t InputThread, ProcessThread1, ProcessThread2, OutputThread;

	/* Input and Output buffer arguments to pass to each thread*/
	int InputPipe[2], ProcessPipe[2], OutputPipe[2];

	if (pipe(InputPipe) || pipe(ProcessPipe) || pipe(OutputPipe)) { 
		perror("Pipe init failed\n");
		return EXIT_FAILURE;
	}
	
	switch(InputThread = fork())
	{
		case 0:			// Child process
			close(InputPipe[0]);	//Close read side of pipe
			InputThreadFunc(InputPipe[1]);
		default:
			switch (ProcessThread1 = fork())
			{
				case 0:
					close(InputPipe[1]);	//Close write side of input pipe
					close(ProcessPipe[0]);	//Close read side of process pipe
					RemoveCarriageFunc(InputPipe[0], ProcessPipe[1]);
				default:
					switch (ProcessThread2 = fork())
					{
						case 0:
							close(ProcessPipe[1]); //Close write side of process pipe
							close(OutputPipe[0]);	//Close read end of output pipe
							CrunchStringFunc(ProcessPipe[0], OutputPipe[1]);
						default:
							switch(OutputThread = fork())
							{
								case 0:
									close(OutputPipe[1]);	//Close write side of output pipe
									OutputFunc(OutputPipe[0]);
								default:
									wait(NULL);
							}
					}
			}
	}
	
   return 0;
}
