#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define ONE  1
typedef sem_t Semaphore;
Semaphore *mutex;

int main()
{
    int i;
    int t;
    key_t shmkey;
    int shmid;
    pid_t pid;
    int*p;
    unsigned int n;
    unsigned int value;
    int *nSeconds;
    srand(time(NULL));
   
    
   
    /* Below we initiate our shared memory segmentith the shmget function  (1)  */
    /*  The shmkey varible holds the path and the value of our unique key (2) */
    shmkey = ftok("/dev/null",5);
    shmid = shmget(shmkey,sizeof(int),0644|IPC_CREAT);
    
    
    /*  The if statement checks if the shared memory has been initiated      */
    /* otherwise it returns the error "shmget"   (3)   */
    
    
    
    if (shmid<0)
    {
        perror("shmget\n");
        exit(1);
        
    }
    /******************************************************/
    /* Initialization of the shared memory segment to the p named variable (4) */
    p = (int*)shmat(shmid,NULL,0);
    
    *p=0;

    /******************************************************/

    printf("How mamy children do you want to fork?\n");
    printf("Fork count:");
    scanf("%u", &n);

    /******************************************************/

    /*  The variable mutex (mutual-exclusion) holds the named semaphore "pSem"
    with permisions 0644 and value of 1 (5)   */

    mutex = sem_open("pSem",O_CREAT|O_EXCL,0644,ONE);

    /*      nSeconds holds a random number from srand and its used to determine for how long the child will exist in critical region (6)   */

    nSeconds = (int*)malloc(n*sizeof(int));
    
    for(i=0;i<n;i++)nSeconds[i]=rand()%4 + 1;
    /******************************************************/

    for(i=0;i<n;i++)
    {
        pid = fork();
        if (pid<0)
        {
            sem_unlink("pSem");
            sem_close(mutex);

            printf("Fork error.\n");

        }
        else if (pid==0)
            break;
    }

    /******************************************************/
    /******************PARENT PROCESS************************************/
    /******************************************************/
    if(pid>0)
    {
        while (pid = waitpid (-1,NULL,0))
        {
            if(errno == ECHILD)
            break;
        }

    printf("Parent:All children have excited.\n");
    printf("*p=%d\n\n\n", *p);

    /* When all the childen procecess will terminate shmdt is called to 
    detach the shared memory segment.  
    The shmctl with IPC_RMID is used to remove the shared memory segment (7)  */
    shmdt(p);
    shmctl(shmid,IPC_RMID,0);
    /*  sem_unlink   destroy the pSem named semaphore 
    sem_close closes the mutec semaphore. (8)  */
    sem_unlink("pSem");
    sem_close(mutex);

    }

    /******************************************************/
    /******************CHILD PROCESS************************************/
    /******************************************************/

    else
    {
        sem_wait(mutex);

        printf("Child(%d) exists in critical region\n",i);
        printf("Waiting %d seconds.\n",nSeconds[i]);
        t = *p;
        t++;
        sleep(nSeconds[i]);
        *p = t;
        printf("Increasing *p\n");
        printf("Child(%d)exists the critical region\n",i);
        printf("new value of *p=%d.\n\n\n",*p);
        sem_post(mutex);

    }
    exit(0);
    


}







