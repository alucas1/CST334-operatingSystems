/* 
* Name: Alberto Lucas
* Date: 04/13/2021
* Title: Lab6 – step 2
* Description: This program shows how semaphores can be used to solve the
* consumer/producer multithread problem. Two semaphores (empty, full) are used
* to track the state of the buffer, and one (mutex) is used to maintain mutual
* exclusion in critical sections of code. The buffer is filled with the letters of
* the alphabet, and the consumer reads the letters. 
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFF_SIZE 4 // defines buffer size
#define NTHREADS 1  // num of threads to create (producer and consumer)
#define NINPUT 12   // amount of input that will be produced/consumed

sem_t mutex;
sem_t empty;
sem_t full;

char buffer[BUFF_SIZE]; // main buffer
int fill = 0;           // Tracks where to fill buffer
int use = 0;            // Tracks where to consume from buffer
int numletter = 0;      // Tracks which letter to fill the buffer with next


// Puts a char into the buffer
void put(char letter)
{
    buffer[fill] = letter;
    fill = (fill + 1) % BUFF_SIZE;
}

// Gets a char from the buffer
char get()
{
    char tmp = buffer[use];
    use = (use + 1) % BUFF_SIZE;
    return tmp;
}

// Function for producer thread. Uses put() in a safe
// manner (using semaphores) to put items in a buffer.
void *producer(void *arg)
{
    int i;
    for (i = 0; i < NINPUT; i++)
    {
        sem_wait(&empty);    // waits until buffer has room or is empty
        sem_wait(&mutex);    // gets lock to access buffer
        put('a' + numletter); // writes char to buffer
        printf("\t\t\t\tPthread %d: put %c\n", (int)pthread_self(),'a' + numletter);
        numletter = (numletter + 1) % 26;
        sem_post(&mutex);    // release lock
        sem_post(&full);     // signal that a value has been added
    }
}

// Function for consumer thread. Uses get() in a safe
// manner (using semaphores) to get items from a buffer.
void *consumer(void *arg)
{
    int i;
    for (i = 0; i < NINPUT; i++)
    {
        sem_wait(&full);  // waits until buffer is filled
        sem_wait(&mutex); // gets lock to access buffer
        char tmp = get(); // get char
        printf("Cthread %d: got %c\n", (int)pthread_self(), tmp);
        sem_post(&mutex); // release lock
        sem_post(&empty); // signal that a value has been consumed   
    }
}

int main()
{
    // Initialize locks
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFF_SIZE);
    sem_init(&full, 0, 0);

    // Define pthread identifiers for consumer/producer threads
    pthread_t threadsC[NTHREADS];
    pthread_t threadsP[NTHREADS];

    // create producer/consumer threads
    int i;
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_create(&threadsP[i], NULL, producer, NULL);
        pthread_create(&threadsC[i], NULL, consumer, NULL);
    }

    // Waits for consumer/producer threads to return
    for (i = 0; i < NTHREADS; i++)
    {
        pthread_join(threadsP[i], NULL);
        pthread_join(threadsC[i], NULL);
    }
    printf("\nMain thread done - all threads returned");
    printf("\nBuffer size: %d", BUFF_SIZE);
    printf("\nConsumer threads: %d", NTHREADS);
    printf("\nProducer threads: %d", NTHREADS);
    printf("\nTotal input: %d\n", NINPUT);

    // release semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
