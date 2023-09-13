#include <stdio.h>
#include "mpi.h"
#include <math.h>


double func(double x)
{
    return 4.0/(1.0 + x*x);;
}

double getPartialRes(double ll, double ul, double h, int intervals, int rank, int procs)
{
  int n = intervals,i;
  h = (ul - ll) / (double)n;
  double x, fx;
  double partialRes = 0;
  for (i = rank ; i <= n; i += procs)
      {
          x = ll + i * h;
          fx = func(x);
          if (i == 0 || i == n)
              partialRes += fx;
          else if (i % 2 != 0)
              partialRes += 4 * fx;
          else
              partialRes += 2 * fx;
      }
      return partialRes;
}

int main(int argc, char* argv[])
{
    MPI_Status stat;
    int procs, rank, rc, i, j, exit = 0;
    int n = 0;
    int n2 = 0;
    int tag = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double pi = 0, tmp = 0;
    double ll = 0;
    double ul = 1;
    double h = 0;
    double start_time = 0;
    double end_time = 0;
    double accuracy = 0; 


    while(!exit)
    {
      if (rank == 0)
      {
        printf("Enter the Number of Intervals: (-1 to exit)\n ");
        scanf("%d", &n2);
        start_time = MPI_Wtime();
        for(i = 1; i< procs;i++)
        {
           rc = MPI_Send(&n2, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
        }

        n = n2;
        

        if(n == -1)
          break;

        
        h = (ul - ll) / (double)n;
        double res = getPartialRes(ll, ul, h, n, rank, procs);
        for(j = 1; j< procs; j++)
        {
          rc = MPI_Recv(&tmp, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD, &stat);
          res+=tmp;

        }
        
        pi = res * (h / 3);
        end_time = MPI_Wtime();
        accuracy = (fabs(pi - M_PI)/M_PI) * 100;
        printf("value of PI is %0.12lf accuracy is %0.16lf time taken %0.16lf \n", pi, accuracy, end_time - start_time);
      }

     else
     {
      
      rc = MPI_Recv(&n, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
      if(n == -1)
        break;
      h = (ul - ll) / (double)n;
      double partialRes = getPartialRes(ll, ul, h, n, rank, procs);
      
      rc = MPI_Send(&partialRes, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
      
      }

    }

    MPI_Finalize();
}
