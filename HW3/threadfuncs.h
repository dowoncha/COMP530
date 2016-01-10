/**
 * Dowon Cha
 * COMP 530
 * I pledge I did not copy anyone's code
 * This header is for the functions that are called by the main thread to do a bit of work on
 * the input buffers and output a changed charaacter
 */

#include "buffer.h"

#ifndef THREAD_FUNCS_H
#define THREAD_FUNCS_H

/* This struct is to pass in the input and output buffers to each thread*/
typedef struct
{
   BoundedBuffer *input_buffer;
   BoundedBuffer *output_buffer;
} ThreadInit;

/* This will output 80 character lines from an input buffer*/
void *OutputFunc(void *args);

/* This  will convert all 2 stars into a caret */
void *CrunchStringFunc(void *args);

/* This will convert all carriage returns into a space*/
void *RemoveCarriageFunc(void *args);

/* This will read input from the user until EOF is called*/
void *InputThreadFunc(void *args);

#endif
