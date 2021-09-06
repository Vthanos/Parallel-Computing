#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void display(char *str);


int main (int argc, char **argv)
{
    pid_t pid;
    sem_t *sem1; // First semaphore
    sem_t *sem2; // Second semaphore

    // create, initialize semaphores
    sem1 = sem_open("/semaphore1", O_CREAT|O_EXCL,  0644, 0);
    sem2 = sem_open("/semaphore2", O_CREAT|O_EXCL,  0644, 1);

    pid = fork();
    if (pid >0)
    {
        sem_wait(sem2);
        display("good morning ");
        sem_post(sem1);
        wait(NULL);
        display("a nice day \n\n");  
        
        
        
    }
    else if (pid==0)
    {
        
        sem_wait(sem1);
        display("sir\n");
        display("i wish you, "); 
        sem_post(sem2);
    }
    else
    {
        printf("Fork error. \n");
    }
    
    sem_close(sem1);
    sem_unlink("/semaphore1");
    sem_close(sem2);
    sem_unlink("/semaphore2");
    exit(0);
    
}
void display(char *str)
{
    char *p;
    for (p=str; *p; p++)
    {
        write(1, p, 1);// writes 1 byte from the buffer pointed to
                            // by p to the standard output 1
        usleep(100); //sleep in milliseconds
    }
}
