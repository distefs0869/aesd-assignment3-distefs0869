#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    thread_data_t* thread_data_p = (thread_data_t*)thread_param;
    int rc = 0;
    
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    //thread_data_p->thread_complete_success = (0 == (rc = pthread_mutex_init(thread_data_p->mutex, NULL)));
    sleep(thread_data_p->wait_to_obtain_secs);
    thread_data_p->thread_complete_success = (0 == (rc = pthread_mutex_lock(thread_data_p->mutex)));
    if (!thread_data_p->thread_complete_success)
    {
       printf("ERROR- Failed to lock thread mutex: %d\r\n", rc);
    }
    else
    {
        sleep(thread_data_p->wait_to_release_secs);
        thread_data_p->thread_complete_success = (0 == (rc = pthread_mutex_unlock(thread_data_p->mutex)));
        if (!thread_data_p->thread_complete_success)
        {
           printf("ERROR- Failed to unlock thread mutex: %d\r\n", rc);
        }
    }
    
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
     bool retVal = false;
     int rc = 0;
     thread_data_t* thread_data_p = (thread_data_t*)malloc(sizeof(thread_data_t));
     
     retVal = (thread_data_p != NULL);
     if (retVal)
     {
         thread_data_p->mutex = mutex;
         thread_data_p->wait_to_obtain_secs = (float)wait_to_obtain_ms / 1000.0;
         thread_data_p->wait_to_release_secs = (float)wait_to_release_ms / 1000.0;

         rc = pthread_create(thread, NULL, threadfunc, (void*)thread_data_p);
         retVal = (rc == 0);
         if (!retVal)
         {
            printf("ERROR- Falied to create thread: %d\r\n", rc);
         }
     }
     else
     {
        printf("ERROR- malloc failed to allocate memory.\r\n");
     }
     
    return (retVal);
}

