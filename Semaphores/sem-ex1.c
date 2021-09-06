

#include <stdio.h>     /* printf()	*/
#include <stdlib.h>    /* exit(), malloc(), free() */
#include <sys/types.h> /* key_t, sem_t, pid_t     */
#include <sys/shm.h>   /* shmat(), IPC_RMID       */
#include <errno.h>     /* errno, ECHILD	*/
#include <semaphore.h> /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>  
#include <wait.h> 
#include <unistd.h>  /* O_CREAT, O_EXEC         */

int main()
{
    key_t shmkey; /*	shared memory	key	*/
    int shmid;    /*	shared memory	id	*/
    pid_t pid;    /*	fork pid		*/
    int *p;
    int i;       /* shared variable	*/
                  /*shared */

    /********************************************************/

    /* Initialize a shared variable in shared memory	*/
    /* valid directory name and a number	*/ 
    shmkey = ftok("/dev/null", 5);
    shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT);

    /* shared memory error check */ 
    if (shmid < 0)
    {
        perror("shmget\n");
        exit(1);
    }

    /********************************************************/

    /********************************************************/

    /* attach p to shared memory */
    p = (int *)shmat(shmid, NULL, 0);
    *p = 0;
    printf(" Initial value of shared variable: %d\n\n", *p);
    /********************************************************/
    
    
    
    
    pid = fork();
        if (pid < 0)
    {
        printf("Fork error.\n");
        return (-1);
    }
    while (*p < 6)
    {
         if (pid > 0)
    {
        /* wait for all children to exit */
        while (pid = waitpid(-1, NULL, 0)) // The program should hang in
        // the loop until there are no more children
        {
            if (errno == ECHILD) // it is set when there are no
                // childrents
                break;
        }

        printf("  Value of shared variable = %d\n", *p);

        /* shared memory detach */ shmdt(p);
        shmctl(shmid, IPC_RMID, 0);
    }

    /******************************************************/
    /******************	CHILD PROCESS	*****************/
    /******************************************************/ 
    else
    {
        
        
        
           printf("the value of the shared memmory is %d\n",++(*p));
        
        

    }
    }
    
    /******************************************************/
    /******************	PARENT PROCESS	****************/
    /******************************************************/ 
   
        
    
    
    

    

exit (0);
}
