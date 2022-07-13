#include "mpi.h"  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 
int main1(int argc, char *argv[])
{
	int self, size;
	MPI_Status s;
	//equivalents  
	float equs[][10] = { { 1,2,6,3,5,23,14,6,21,14 },
	{ 2,8,6,10,21,3,14,21,15,23 },
	{ 6,6,9,21,14,23,6,54,1,14 },
	{ 21,1,2,32,2,36,21,4,2,15 },
	{ 2,23,54,3,32,11,12,1,44,1 },
	{ 21,12,14,1,23,24,14,21,12,11 },
	{ 1,24,3,36,6,21,4,5,12,21 },
	{ 21,41,45,12,32,21,41,12,4,6 },
	{ 6,12,32,21,14,21,4,5,6,21 },
	{ 21,12,41,8,18,32,16,14,7,17 }
	};
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &self);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	float *equivalent = (float*)malloc((size)*sizeof(float));
	float *recv = (float*)malloc((size)*sizeof(float));
	float *x = (float*)malloc((size)*sizeof(float));
	float B[10] = { 2,4,6,2,41,21,21,4,6,27 };
	float temp = 0.0;
	int marked = 0;
	int i, j, k;
	int flag = 0;
	//distribute  
	if (0 == self)
	{
		for (i = 0; i<size; i++)
		{
			equivalent[i] = equs[0][i];
		}
		for (j = 1; j<size; j++)
		{
			MPI_Send(equs[j], size, MPI_FLOAT, j, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(equivalent, size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &s);
	}
	//pivot  
	for (i = 0; i<size - 1; i++)
	{
		if (self == i)
		{
			for (k = 0; k<size; k++)
			{
				recv[k] = equivalent[k];
			}
			marked = 1;
		}
		MPI_Bcast(recv, size, MPI_FLOAT, i, MPI_COMM_WORLD);
		if (0 == marked)
		{
			float ratio = equivalent[i] / recv[i];
			for (j = i; j<size; j++)
			{
				equivalent[j] -= recv[j] * ratio;
			}
		}
	}

	//backtracking
	for (i = size - 1; i >= 0; i--)
	{
		if (self == i)
		{
			j = size - i - 1;
			k = size - 1;
			while (j--)
			{
				temp += x[k] * equivalent[k];
				k--;
			}
			x[i] = (B[i] - temp) / equivalent[i];
		}
		MPI_Bcast(x, size, MPI_FLOAT, i, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	printf("The equivalent on %d is: %.2f %.2f %.2f ...\n", self, equivalent[0], equivalent[1], equivalent[2]);
	MPI_Barrier(MPI_COMM_WORLD);
	if (size - 1 == self)
	{
		if (equivalent[size - 1] == 0)
		{
			printf("This matix is unsolvable!\n");
			flag = 1;
		}
		else
		{
			printf("This matix is solvable!\n");
		}
	}
	if (flag)
	{
		free(recv);
		free(equivalent);
		free(x);
		MPI_Finalize();
		return 0;
	}
	if (0 == self)
	{
		printf("The matix B is: %.2f %.2f %.2f ...\n", B[0], B[1], B[2]);
		printf(" The anwer is :\n");
		for (i = 0; i<size; i++)
		{
			printf("%.2f ", x[i]);
		}
	}
	putchar('\n');
	free(recv);
	free(equivalent);
	free(x);
	MPI_Finalize();
	return 0;
}
