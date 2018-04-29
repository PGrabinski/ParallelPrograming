#include<stdio.h>
#include<mpi.h>
#include<math.h>
#include<gsl/gsl_rng.h>

double d_Avarage(int n_myrank,double d_numbers[], int n_L,int n_N){
    int i;
    double d_ava=0;
    for(i=n_myrank*n_L;i<(n_myrank+1)*n_L;i++){
        d_ava+=d_numbers[i];
    }
    d_ava=d_ava/(double)(n_N);
    return d_ava;
}

double d_SqAvarage(int n_myrank,double d_numbers[],int n_L,int n_N){
    int i;
    for(i=n_myrank*n_L;i<(n_myrank+1)*n_L;i++){
        d_numbers[i]=d_numbers[i]*d_numbers[i];
    }
    return d_Avarage(n_myrank,d_numbers,n_L,n_N);
}
        
int main(int argc,char *argv[]){
    MPI_Init(&argc,&argv); 
    const int n_L=10;
    gsl_rng_env_setup();
    gsl_rng *r=gsl_rng_alloc(gsl_rng_mt19937);
    
    int i;
    int n_N;
    int n_myrank;
    int n_totproc;
    double d_ava=0;
    double d_sqava=0;
    double d_disp=0;
    double d_avapart=0;
    double d_sqavapart=0;
    MPI_Comm_size(MPI_COMM_WORLD,&n_totproc);
    MPI_Comm_rank(MPI_COMM_WORLD,&n_myrank);
    n_N=n_L*n_totproc;
    double d_numbers[n_N];
    MPI_Comm_rank(MPI_COMM_WORLD,&n_myrank);
    for(i=0;i<n_N;i++){
        d_numbers[i]=i;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    d_avapart=d_Avarage(n_myrank,d_numbers,n_L,n_N);
    d_sqavapart=d_SqAvarage(n_myrank,d_numbers,n_L,n_N);
    MPI_Reduce(&d_avapart,&d_ava,sizeof(double),MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Reduce(&d_sqavapart,&d_sqava,sizeof(double),MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD,&n_myrank);
    printf("Got the parts! %d\n",n_myrank);
    if(n_myrank==0){
        d_disp=sqrt(d_sqava-d_ava*d_ava);
        printf("%f %f %f\n",d_ava,d_sqava,d_disp);
    }
    MPI_Finalize();
    return 0;
}
