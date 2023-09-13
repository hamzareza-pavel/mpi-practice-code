#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
  int a;
  int b;
  int world_size,rank,rc,tag =1;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Status stat;
  int multresult = 0;
  double start_time = 0.0;
  double end_time = 0.0;

  MPI_Init(&argc, &argv);
  start_time = MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Get_processor_name(processor_name, &name_len);

  if(rank == 0)
  {

    a=10;
    int rcvd;
    printf("Processor name is %s\n", processor_name);
    printf("MPI world size is %d\n", world_size);
    for(int i = 1; i<world_size; i++)
    {
      for(int j =0;j<50;j++)
      {
          rc = MPI_Send(&a, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
      }
    }
  }
  else
  {
    b = 20;
    rc = MPI_Recv(&a, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
    multresult = a*b;
    rc = MPI_Send(&multresult, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  //accumulating result in prcessor 0

  if(rank == 0)
  {
    int sum = 0;
    for (int i = 1; i<world_size; i++)
    {
      for(int j =0;j<50;j++)
      {
         rc = MPI_Recv(&multresult, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &stat);
      }
      sum+=multresult;
      printf("Result from processor %d is %d \n", i, multresult);
    }

    printf("Sum of received number is %d\n",sum);

  }
  end_time = MPI_Wtime();
  if(rank ==0 )
  {
    printf("elapsed time for MPI ops %lf\n", end_time - start_time);
  }


  MPI_Finalize();
}
