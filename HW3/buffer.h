/**
 * Dowon Cha
 * COMP 530
 * I pledge i did not copy this code
 * BoundedBuffer struct holds an array of chars. Uses semaphores and a mutex semaphores
 * to guarantee synchronization and data sharing locks
 */

#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include "st.h"
#include "semaphore.h"

/* BoundedBuffer struct that holds an array of chars*/
typedef struct
{
  char *buffer;
  semaphore *full, *empty, *mutex;
  int nextIn, nextOut;
  int buffer_size;
} BoundedBuffer;

/* This function will initialize a buffer and return it */
void init_buffer(BoundedBuffer **buffer, const int buffer_size);

/* Delete's a buffer after it is done being used */
void delete(BoundedBuffer **buffer);

/* Deposit into the buffer the input char */
void deposit(BoundedBuffer *buffer, const char c); 

/* Remove a char form the buffer and output it*/
char remoove(BoundedBuffer *buffer);

#endif
