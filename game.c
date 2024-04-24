/*

This simple program simulates a rock-paper-scissors game of two threads.

Depending on which thread won the previous round, it determines 
which thread will start the next one.

 * Functions and variables:
 * 
 * pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; // Condition variable for thread 0
 * pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER; // Condition variable for thread 1
 * pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Mutex lock for synchronization
 * 
 * typedef struct // Structure to hold thread data
 * {
 *     int threadNumb; // Thread number
 *     int choice;     // Choice made by the thread (0: Paper, 1: Rock, 2: Scissors)
 * } ThreadData;
 * 
 * void *threadsGame(void *arg); // Function representing the game logic for each thread
 * int main(); // Main function controlling the game flow
 * 
 * Main logic:
 * 
 * - Two threads are created to play the rock-paper-scissors game.
 * - Each thread randomly chooses between paper, rock, or scissors.
 * - The game consists of multiple rounds, where each round has a winner or ends in a draw.
 * - Threads take turns making choices, controlled by condition variables (cond1 and cond2) and mutex (lock).
 * - After each round, the winner or draw is determined based on the choices made by the threads.
 * - The game continues for a fixed number of rounds (here, 10 rounds).
 * - At the end of the game, the scores are compared, and the winner (or draw) is announced.

*********************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int threadNumb;
    int choice;
} ThreadData;

void *threadsGame(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    
    while (1)
    {
        pthread_mutex_lock(&lock);
        int choice = rand() % 3;
        data->choice = choice;
        printf(" %d : ", data->threadNumb);
        switch (choice)
        {
        case 0:
            printf("Paper\n");
            break;

        case 1:
            printf("Rock\n");
            break;

        case 2:
            printf("Scissors\n");
            break;
        }

        if (data->threadNumb == 0)
            pthread_cond_wait(&cond1, &lock);
        
        else if (data->threadNumb == 1)
            pthread_cond_wait(&cond2, &lock);
        
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main()
{
    pthread_t tid1;
    pthread_t tid2;

    
    ThreadData data1 = {0, 0};
    ThreadData data2 = {1, 0};

    int threadNumb[2] = {0, 1};

    srand(time(NULL));

    printf("Round 1\n");
    pthread_create(&tid1, NULL, threadsGame, (void *)&data1);
    pthread_create(&tid2, NULL, threadsGame, (void *)&data2);
    sleep(2);
    printf("\n");

    int j = 2,  count0 = 0, count1 = 0;

    while (j <= 10)
    {
        printf("Round %d\n", j);

        if (data1.choice == data2.choice)
        {
            printf("Draw in previous round, thread 0 starts next\n");
            pthread_cond_signal(&cond1);
            sleep(2);
            pthread_cond_signal(&cond2);
            sleep(2);
          
        }
        else if ((data1.choice == 0 && data2.choice == 1) || (data1.choice == 1 && data2.choice == 2) || (data1.choice == 2 && data2.choice == 0))
        {
            printf("Thread 0 won previous round, thread 1 starts next\n");
            count0++;
            pthread_cond_signal(&cond2);
            sleep(2);
            pthread_cond_signal(&cond1);
            sleep(2);
        }
        else if ((data2.choice == 0 && data1.choice == 1) || (data2.choice == 1 && data1.choice == 2) || (data2.choice == 2 && data1.choice == 0))
        {
            printf("Thread 1 won previous round, thread 0 starts next\n");
            count1++;
            pthread_cond_signal(&cond1);
            sleep(2);
            pthread_cond_signal(&cond2);
            sleep(2);
        }
        printf("\n");
        j++;
    }
    if(count1 == count0)
        printf("Draw, result %d to %d\n", count1, count0);
    else if(count1 > count0)
        printf("Thread 1 has won with a score: %d to %d\n",count1, count0);
    else
        printf("Thread 0 has won with a score: %d to %d\n",count0, count1);

    return EXIT_SUCCESS;
}
