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

/* Library and custom includes*/
#include "st.h"
#include "buffer.h"
#include "threadfuncs.h"

#define INPUT_BUFFER_SIZE 200
#define PROCESS_BUFFER_SIZE 200
#define OUTPUT_BUFFER_SIZE 200

int main(int argc, char const *argv[])
{
   /* Declare buffers*/
   BoundedBuffer *input_buffer, *process_buffer, *output_buffer;  
   init_buffer(&input_buffer, INPUT_BUFFER_SIZE);
   init_buffer(&process_buffer, PROCESS_BUFFER_SIZE);
   init_buffer(&output_buffer, OUTPUT_BUFFER_SIZE);
         
   /* Initialize the ST library*/
   st_init();

   /* Input and Output buffer arguments to pass to each thread*/
   ThreadInit InputArgs = { 0, input_buffer };
   ThreadInit RemoveCarriageArgs = {input_buffer, process_buffer};
   ThreadInit CrunchArgs = {process_buffer, output_buffer};
   ThreadInit OutputArgs = {output_buffer, NULL};

   /* Create thread to read input*/
   if (st_thread_create(InputThreadFunc, &InputArgs, 0, 0) == NULL)
     {
       perror("Failed to create Input Thread Func\n");
       exit(EXIT_FAILURE);
    }
    
    /* Create thread to remove all carriage returns and conver them into spaces*/
   if (st_thread_create(RemoveCarriageFunc, &RemoveCarriageArgs, 0, 0) == NULL)
   {
	   perror("Failed to create first process thread\n");
	   exit(EXIT_FAILURE);
   }
   
   /* Convert two consecutive stars into a ^ or do nothing*/
   if (st_thread_create(CrunchStringFunc, &CrunchArgs, 0, 0) == NULL)
   {
        perror("Failed to create crunch string thread\n");
		exit(EXIT_FAILURE);
   }
   
   /* Output all the converted input values*/
   if (st_thread_create(OutputFunc, &OutputArgs, 0, 0) == NULL)
     {
       perror("Failed to create output task\n");
       exit(EXIT_FAILURE);
     }
     
   st_thread_exit(NULL);

   /* Free up bounded buffer memory*/
   delete(&input_buffer);
   delete(&process_buffer);
   delete(&output_buffer);
   return 0;
}

