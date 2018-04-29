#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main (int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    printf("Hello world!\n");
    int j,size;
    MPI_Status status;
    double p=3.14;
    MPI_Comm_rank(MPI_COMM_WORLD, &j);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(j==0){
            printf("The size is: %d\n", size);
            MPI_Send(&p, 1, MPI_DOUBLE, 1, 20,MPI_COMM_WORLD);
    }
    if(j==1){
            MPI_Recv(&p, 1, MPI_DOUBLE, 0, 20,MPI_COMM_WORLD,&status);
            printf("Received number: %f\n", p);
    }
    MPI_Finalize();
    return 0;
}
