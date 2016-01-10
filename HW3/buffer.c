#include <assert.h>
#include <stdlib.h>

#include "buffer.h"

void init_buffer(BoundedBuffer **buffer, const int buffer_size)
{
	assert(buffer != 0);
	assert(buffer_size > 0);

	/* Allocate memory for the BoundedBuffer according to size*/
	*buffer = (BoundedBuffer*)malloc(sizeof(BoundedBuffer));
	/* Allocate memory for the actual char buffer array*/
	(*buffer)->buffer = (char*)malloc(sizeof(char) * buffer_size);	
	(*buffer)->buffer_size = buffer_size;
	
	/* Allocate memory for the 3 semaphores*/
	semaphore *full = (semaphore*)malloc(sizeof(semaphore));
	semaphore *empty = (semaphore*)malloc(sizeof(semaphore));
	semaphore *mutex = (semaphore*)malloc(sizeof(semaphore));
	
	/* Create each semaphore for the pointers*/
	createSem(full, 0);
	createSem(empty, buffer_size);
	createSem(mutex, 1);

	/* Assign each pointer in buffer to the created semaphores*/
	(*buffer)->full = full;
	(*buffer)->empty = empty;
	(*buffer)->mutex = mutex;
}

void delete(BoundedBuffer** buffer)
{
  assert(buffer != NULL);

  /* Free up all buffers used by the program*/
  free((*buffer)->buffer);
  free((*buffer)->full);
  free((*buffer)->empty);
  free((*buffer)->mutex);
  free((*buffer));
  
  buffer = 0;
}

void deposit(BoundedBuffer *buffer, const char c)
{
	assert(buffer != 0);

	/* Lock the empty buffer and mutual exclusion */
	down(buffer->empty);
	down(buffer->mutex);

	/* Input the char into the buffer*/
	(buffer->buffer)[buffer->nextIn] = c;
	/* Increment buffer nextIn counter, bounded by buffer_size*/
	buffer->nextIn = (buffer->nextIn + 1) % buffer->buffer_size;

	/* Unlock the full buffer and mutual exclusion*/
	up(buffer->mutex);
	up(buffer->full);
}

char remoove(BoundedBuffer* buffer)
{
	assert(buffer != 0);
  
	char removed_char;

	/* Lock the full buffer and mutual exclusion */
	down(buffer->full);
	down(buffer->mutex);

	/* Get a character from the buffer*/
	removed_char = (buffer->buffer)[buffer->nextOut];
	/* Increment the nextOut in the buffer*/
	buffer->nextOut = (buffer->nextOut + 1) % buffer->buffer_size;
	/* Unlock the buffer and mutual exclusion*/
	up(buffer->mutex);
	up(buffer->empty);

	return removed_char;
}
