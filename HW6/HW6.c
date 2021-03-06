/**
 * Copyright 2015, Dowon Cha
 * COMP 530
 * I pledge i did not copy this code
 * This program will read in input from the user/file
 * Convert all carriage returns into a space
 * Convert 2 consecutive stars into a ^, and then output 80 character lines to the user.
 */

/* Standard includes*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

/* Custom headers*/
#include "buffer.h"

#define LINE_LENGTH 80

/**
 *  Struct to hold the input and output buffer. Pass this into child process functions
 */
typedef struct ProcessInit
{
  BoundedBuffer *input_buffer;
  BoundedBuffer *output_buffer;
} ProcessInit;

/**
 *  Fork a child process using the input function.
 *  Pass forked function args
 */
pid_t forkChild(void (*function)(void *), void *args);

/**
 *  Waits for all children processes to finish. If a child exits
 *  abnormally, kill all children processes
 */
void waitForChildren(pid_t *childpids, int n);

/**
 *  Function will read in from stdin and output to a buffer.
 *  Expects args of type ProcessInit, input_buffer of args should be NULL.
 */
void InputFunc(void* args);

/**
 *  Function will convert all carriage returns into a space.
 *  Expects args of type Processnit with both input and output buffers set.
 */
void RemoveCarriageFunc(void* args);

/**
 *  Function will crunch 2 * into a ^, otherwise just passes char into output buffer.
 *  Expects args of type ProcessInit with both input and output buffers set.
 */
void CrunchFunc(void* args);

/**
 *  Function will output LINE_LENGTH lines to stdout. 
 *  Exepcted args of type ProcessInit. ProcessInit->output_buffer should be NULL.
 */
void OutputFunc(void* args);

int main(int argc, char const *argv[])
{
  // Declare buffers
  BoundedBuffer *input_buffer, *process_buffer, *output_buffer; 
  
  // Create memory mapped buffers
  input_buffer = create_buffer();
  process_buffer = create_buffer();
  output_buffer = create_buffer();

  // Create process argument structs to pass buffers
  ProcessInit InputInit = { NULL, input_buffer };
  ProcessInit RCInit = { input_buffer, process_buffer };
  ProcessInit CrunchInit = {process_buffer, output_buffer };
  ProcessInit OutputInit = {output_buffer, NULL };
      
  pid_t childpids[4];

  //Fork a child for each process
  childpids[0] = forkChild(InputFunc, &InputInit);
  childpids[1] = forkChild(RemoveCarriageFunc, &RCInit);
  childpids[2] = forkChild(CrunchFunc, &CrunchInit);
  childpids[3] = forkChild(OutputFunc, &OutputInit);

  waitForChildren(childpids, 4);
  
  destroy_buffer(input_buffer);
  destroy_buffer(process_buffer);
  destroy_buffer(output_buffer);
  
  exit(EXIT_SUCCESS);
}

pid_t forkChild(void (*function)(void *), void *args)
{
  pid_t childpid;
  switch (childpid = fork())
  {
    case -1:
      perror("Fork error");
      exit(EXIT_FAILURE);
    case 0:
      (*function)(args);
    default:
      return childpid;
  }
}

void waitForChildren(pid_t* childpids, int n)
{
  int status;
  int i;

  //Run until a child exits with a bad status
  while (-1 < wait(&status)) 
  {
    if (!WIFEXITED(status))  //Watch all child processes
    {
      for (i = 0; i < n; ++i) //Kill all children if one child fails
      {
	  kill(childpids[i], SIGKILL);
      }
      break;
    }
  }
}

void InputFunc(void *args)
{
  assert(args != NULL);
  
  ProcessInit *init = args;
  char c;

  // Read every character from stream until end of file
  while (1)
  {
    c = getchar();
    deposit(init->output_buffer, c);
    
    if (c == EOF) {
      break;
    }
  }

  _Exit(0);
}

void RemoveCarriageFunc(void *args)
{
  assert(args != NULL);

  ProcessInit *init = args;
  char c;
  
  // Run until EOF is passed through buffer
  while (1)
  {
    /*Read values from the input buffer
     *then replace all carriage returns with a space
     *Deposit back into output buffer */
    c = consume(init->input_buffer);

    if (c == '\n') {
      c = ' ';
    }
    deposit(init->output_buffer, c);

    if (c == EOF) {
      break;
    }
  }

  _Exit(0);
}

void CrunchFunc(void *args)
{
  assert(args != NULL);

  ProcessInit *init = args;
  char c;
  char previous;

  while (1)
  {
    c = consume(init->input_buffer);
    
    // If consumed character is not a star then deposit and set previous char to current.
    //   Additionally, if previous character was a star also deposit a star.
    if (c != '*')
    {
      if (previous == '*') {
	deposit(init->output_buffer, '*');
      }

      deposit(init->output_buffer, c);
      previous = c;
    }
    else if (c == '*' && previous == '*')  //If current and previous char are stars deposit a ^
    {
      deposit(init->output_buffer, '^');
      previous = '^';
    }
    else if (c == '*' && previous != '*')  //If current is a * but previous is not then set previous do not deposit
    {
      previous = '*';
    }
    
    if (c == EOF)
      break;
  }

  _Exit(0);
}

void OutputFunc(void *args)
{
  assert(args != NULL);
  
  ProcessInit *init = args;
  
  //Declare a line buffer of size LINE_LENGTH + 1 for \0 character
  char line[LINE_LENGTH + 1];
  line[LINE_LENGTH] = '\0';
  int i = 0;
  char c;
  
  // Run until the null character is received from the buffer
  while (1)
  {
    c = consume(init->input_buffer);
    
    if (c == EOF)
      break;
    
    line[i] = c;
    ++i;

    /* Once we have 80 characters output the string and reset the counter*/
    if ( i == LINE_LENGTH)
    {
      printf("%s\n", line);
      line[LINE_LENGTH] = '\0';
      i = 0;
    }
  }

  _Exit(0);
}
