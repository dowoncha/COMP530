/**
 *  \file buffer.h
 * \author Dowon Cha
 * COMP 530
 * 
 * I pledge i did not copy this code
 * BoundedBuffer struct holds an array of chars. Uses semaphores and a mutex semaphores to guarantee process safety.
 * Initialize the buffer with alloc_buffer. Remember to delete_buffer after usage. 
 */

#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <sys/mman.h>

#define BUFFER_SIZE 200

/**
 * BoundedBuffer Struct that holds an array of chars and semaphores for processes to use as shared memory
 */

typedef struct BoundedBuffer
{
  char buffer[BUFFER_SIZE];  //Buffer that holds the characters
  sem_t *full, *empty;      //Semaphore name to keep track of full buffer
  int nextIn, nextOut;       //Keep track of counter for reads and writes
} BoundedBuffer;

BoundedBuffer* create_buffer();

void destroy_buffer(BoundedBuffer *buffer);

void deposit(BoundedBuffer *buffer, const char c); 

char consume(BoundedBuffer *buffer);

#endif
