#include <stdio.h>
#include "mpi.h"
#include <math.h>

int main(int argc, char* argv[])
{
  int n, rank, procs, i, rc;
  double mypi,pi,h,sum,x,a;
  int exit = 0;
  int tag = 1;
  MPI_Status stat;
  double start_time = 0;
  double end_time = 0;
  double accuracy = 0; 

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  while(!exit){
    if (rank == 0)
    {

      int n1;
      printf("Enter the number of intervals:(-1 to exit)\n ");
      scanf("%d", &n1);

      for(i = 1; i< procs;i++)
      {
          rc = MPI_Send(&n1, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
      }
      start_time = MPI_Wtime();
      n = n1;
    }

    if(rank != 0)
      rc = MPI_Recv(&n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);

   if(n == -1)
      break;

    h = 1.0/(double)n;
    sum = 0.0;
    for (i = rank; i <= n; i+=procs)
    {
      x = h * ((double)i  - 0.5);
      sum += 4.0/(1.0 + x*x);
    }
    mypi = h * sum;
    pi = mypi;
    if(rank != 0)
    {
      rc = MPI_Send(&mypi, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }

    if(rank == 0)
    {
      for(i = 1; i<procs; i++)
      {
        rc = MPI_Recv(&mypi, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &stat);
        pi+=mypi;
      }
      end_time = MPI_Wtime();
      accuracy = (fabs(pi - M_PI)/M_PI) * 100;
      printf("value of PI is %0.12lf accuracy is %0.5lf time taken %0.8lf \n", pi, accuracy, end_time - start_time);
    }
  }
  MPI_Finalize();
}
