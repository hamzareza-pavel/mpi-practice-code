#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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

			for (i=1; i<group_size; i++)
				MPI_Send(&payload, N, MPI_INT, i, i, MPI_COMM_WORLD) ; 
			for ( i = 1 ; i<group_size ; i++)
			{
				MPI_Recv(&tmp, N, MPI_INT, i, i, MPI_COMM_WORLD, &status) ;
				printf("Received array from process %d is \n",i);
				for(j = 0; j< N ; j++)
				{
					printf("%d ",tmp[j]);
				}
			}
			end = MPI_Wtime();

			printf("\nAverage Time taken to send and receive payload of size %d is %0.16lf\n", N, (end - start)/2);
			
	}
	else { /* if my_rank != 0 */
 			MPI_Recv(&payload, N, MPI_INT, 0, my_rank, MPI_COMM_WORLD, &status) ;
 		    MPI_Send(&payload, N, MPI_INT, 0, my_rank, MPI_COMM_WORLD);
		 }
	MPI_Finalize() ; 
}