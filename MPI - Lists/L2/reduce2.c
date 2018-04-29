#include<mpi.h>
#include<stdio.h>

int main(int argc, char* argv[]){
    
    MPI_Init(&argc,&argv);
    int n_myrank,n_totproc;
    MPI_Comm_rank(MPI_COMM_WORLD,&n_myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&n_totproc);
    int send[n_totproc];
    int recv=0;
    send[n_myrank]=n_myrank;
    MPI_Reduce(&send[n_myrank],&recv,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(n_myrank==0) printf("%d\n",recv);
    MPI_Finalize();
    return 0;
}
