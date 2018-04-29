#include<mpi.h>
#include<stdio.h>

int main(int argc, char* argv[]){
    int myrank;
    int send,recv=0;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    send=myrank;
    MPI_Reduce(&send,&recv,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(myrank==0) printf("%d\n",recv);
    MPI_Finalize();
    return 0;
}
