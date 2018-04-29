#include<stdio.h>
#include<mpi.h>

int main(int argc,char **argv){
    int myrank;
    MPI_Status status;
    double a[10],b[10];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    if(myrank==0){
        //recive&send
        MPI_Send(&a,10,MPI_DOUBLE,1,17,MPI_COMM_WORLD);
        MPI_Recv(&b,10,MPI_DOUBLE,1,19,MPI_COMM_WORLD,&status);
        }
    else if(myrank==1){
        //recive&send
        MPI_Send(&b,10,MPI_DOUBLE,0,19,MPI_COMM_WORLD);
        MPI_Recv(&a,10,MPI_DOUBLE,0,17,MPI_COMM_WORLD,&status);
        }
    MPI_Finalize();
    return 0;
}
