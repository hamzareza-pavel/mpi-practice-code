#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

#define N 8

int main(int argc, char* argv[])
{
	int i, group_size, my_rank, j;
	MPI_Status status;
	MPI_Init (&argc, &argv) ;
	MPI_Comm_size (MPI_COMM_WORLD, &group_size) ; 
	MPI_Comm_rank (MPI_COMM_WORLD, &my_rank) ;
	double start = 0,end = 0;
	int* payload = NULL;
	int* tmp = NULL;

	
	if ( my_rank == 0 )
	{
		start = MPI_Wtime();
		payload = (int*) malloc(N * sizeof(int));
		for(j = 0; j<N; j++)
		{
			payload[j] = rand();
		}
	}
	
	MPI_Bcast(&payload, N, MPI_INTEGER, 0, MPI_COMM_WORLD);
	
	if(my_rank == 0)
	{	
		end = MPI_Wtime();
		printf("\nTime taken to broadcast payload of size %d is %0.16lf\n", N, end - start);
	}

	MPI_Finalize() ; 
}