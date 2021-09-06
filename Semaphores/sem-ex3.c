#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
    int i;
    char *p;
    sem_t *sem1; // First semaphore
    sem_t *sem2; // Second semaphore

    // create, initialize semaphores
    sem1 = sem_open("/semaphore1", O_CREAT,  0644, 0);
    sem2 = sem_open("/semaphore2", O_CREAT,  0644, 1);

    if (fork()) // Parent process
    {
        for (i = 0; i < 10; i++)
        {
            sem_wait(sem2); // Lock the semaphore
            for (p = "ab"; *p; p++)
            {
                write(1, p, 1);
                //usleep(100);
            }
            sem_post(sem1); // Release the semaphore lock
        }
        wait(NULL);
    }
    else // Child process
    {
        for (i = 0; i < 10; i++)
        {
            sem_wait(sem1); // Lock the semaphore
            for (p = "cd\n"; *p; p++)
            {
                write(1, p, 1);
                //usleep(100);
            }
            sem_post(sem2); // Release the semaphore lock
        }
    }

    // Close the Semaphores
    sem_close(sem1);
    sem_unlink("/semaphore1");
    sem_close(sem2);
    sem_unlink("/semaphore2");
    return 0;
}