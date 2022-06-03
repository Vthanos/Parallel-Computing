#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#define _NUM_THREADS 4 
int main()
{
  int n, r = 0, t;
  int my_rank, comm_sz;
  comm_sz=20;
  int num, rem , reverse_num, temp,start,end;
  start=0;
  end=200000;
  omp_set_num_threads(_NUM_THREADS);
  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    
  printf("Enter a number to check if it's a palindrome or not\n");
  scanf("%d", &n);
  if (my_rank== 0){

    int nums= malloc(sizeof(int)*comm_sz);
  }
  MPI_Gather(&n, 1, MPI_INT, nums, 1, MPI_INT, 0, MPI_COMM_WORLD);





  t = n;
   #pragma omp parallel for shared(num,end,revers_num) private(rem,temp)
   for(num=start;num<=end;num++){
       if (num%comm_sz != my_rank) continue;
        temp=num;
        reverse_num=0;
        while(temp){
         rem=temp%10;
         temp=temp/10;
         reverse_num=reverse_num*10+rem;
      }
      if(num==reverse_num)
         printf("%d ",num);
   }

 
   MPI_Finalize();
  if (n == r)
    printf("%d is a palindrome number.\n", n);
  else
    printf("%d isn't a palindrome number.\n", n);

  return 0;
}
