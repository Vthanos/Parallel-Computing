/* File: mpi_omp_pi.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

void Usage(char *prog_name);
double OMP_CalcPI(double a, double b, int n);
double CalcPI(double left_endpt, double right_endpt, int trap_count,double base_len);

/* Function we’re integrating */
double f_pi(double x);
int main(int argc, char **argv)
{
    int my_rank, comm_sz, ierr;
    int n, local_n;
    int mpi_n, mpi_loc_n;
    double mpi_a, mpi_b, mpi_h;
    double mpi_loc_a, mpi_loc_b;
    double mpi_global_pi;
    double omp_global_pi;
    int thread_count;
    struct timeval tvalStart, tvalStop;
    struct timeval tvalTmp, tvalElap;
    double Telap;

    gettimeofday(&tvalStart, NULL);
    /* Let the system do what it needs to start up MPI */
    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    
    if (argc != 3)
    {
        if (my_rank == 0)
            Usage(argv[0]);
        MPI_Abort(MPI_COMM_WORLD, ierr);
    }
    if (argc != 3)
        Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    n = atoi(argv[2]);
    local_n = n / comm_sz; /* So is the number of trapezoids */
    /* separate mpi and omp vars */
    mpi_n = atoi(argv[2]);
    mpi_loc_n = mpi_n / comm_sz; /* number of trapezoids on node */
    /* Length of each process’ interval of integration = local_n*h. */
    /* interval starts/ends at: */
    mpi_a = 0.0;
    mpi_b = 1.0;
    mpi_h = (mpi_b - mpi_a) / mpi_n;
    mpi_loc_a = mpi_a + my_rank * mpi_loc_n * mpi_h;
    mpi_loc_b = mpi_loc_a + mpi_loc_n * mpi_h;
    printf("P[%d]: mpi_n=%d, mpi_h,a,b=[%.8f,%.8f,%.8f], mpi_loc_n,a,b=[%d,%.8f,%.8f]\n",my_rank, mpi_n, mpi_h, mpi_a, mpi_b, mpi_loc_n, mpi_loc_a, mpi_loc_b);
    omp_global_pi = 0.0;
    #pragma omp parallel num_threads(thread_count) \
    reduction(+                                \
    : omp_global_pi)
    omp_global_pi += OMP_CalcPI(mpi_loc_a, mpi_loc_b, mpi_loc_n);
    /* Add up the integrals calculated by each process */
    MPI_Reduce(&omp_global_pi, &mpi_global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    gettimeofday(&tvalStop, NULL);
    Telap = (double)((tvalStop.tv_sec - tvalStart.tv_sec) * 1.0E6 + tvalStop.tv_usec - tvalStart.tv_usec) / 1.0E6;
    /* Print the result */
    if (my_rank == 0)
    {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf(" Estimated value of pi = %.14f\n", mpi_global_pi);
        printf(" Reference value of pi = %.14f\n", 4.0 * atan(1.0));
        printf(" Esitmate Error of pi = %.15e\n", fabs(mpi_global_pi - 4.0 * atan(1.0)));
        printf(" Telapsed in seconds: %e seconds\n", Telap);
    }
    /* Shut down MPI */
    MPI_Finalize();
    return 0;
} /* main */
/*------------------------------------------------------------------
 * Function: Usage
 * Purpose: Print a message explaining how to run the program
 * In arg: prog_name
 */
void Usage(char *prog_name)
{
    fprintf(stderr, "usage: %s <thread_count> <n>\n", prog_name);     /* Change */
    fprintf(stderr, " thread_count is the number of threads >= 1\n"); /* Change */
    fprintf(stderr, " n is the number of terms and should be >= 1\n");
    fprintf(stderr, " n ’%’ thread_count == 0 \n");
    exit(0);
} /* Usage */
/* Function: OMP_CalcPI */
double OMP_CalcPI(double a, double b, int n)
{
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    h = (b - a) / n;
    local_n = n / thread_count;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    my_result = CalcPI(local_a, local_b, local_n, h);
    return my_result;
} /* OMP_CalcPI */
/*------------------------------------------------------------------*/
double CalcPI( double left_endpt /* in */,
double right_endpt /* in */,
int trap_count /* in */,
double base_len /* in */)
{
    double estimate, x;
    int i;
    estimate = (f_pi(left_endpt) + f_pi(right_endpt)) / 2.0;
    for (i = 1; i <= trap_count - 1; i++)
    {
        x = left_endpt + i * base_len;
        estimate += f_pi(x);
    }
    estimate = estimate * base_len;
    return estimate;
} /* CalcPI */
/*------------------------------------------------------------------*/
 * Function: f_pi */
double f_pi(double x /* in */)
{
    return 4.0 / (1 + x * x);
} /* f_pi */