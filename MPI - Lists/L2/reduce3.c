#include<mpi.h>
#include<stdio.h>

void Sq(int *in, double *inout, int *len, MPI_Datatype *dtype){
    int i;
    for(i=0;i<*len;i++) 
        inout[i]+=pow((double)(in[i]),2);
}

int main(int argc, char* argv[]){
    
    MPI_Init(&argc,&argv);
    int n_myrank,n_totproc;
    MPI_Comm_rank(MPI_COMM_WORLD,&n_myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&n_totproc);
    double send[n_totproc];
    send[n_myrank]=n_myrank;
    double recv;
    MPI_Op op;
    MPI_Op_create((MPI_User_function*)Sq,1,&op);
    MPI_Reduce(&send[n_myrank],&recv,1,MPI_DOUBLE,op,0,MPI_COMM_WORLD);
    if(n_myrank==0) printf("%f\n",recv);
    MPI_Finalize();
    return 0;
}
