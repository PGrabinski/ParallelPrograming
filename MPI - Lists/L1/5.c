#include<stdio.h>
#include<mpi.h>
#include<gsl/gsl_rng.h>
#include<math.h>

int main(int argc,char *argv[]){
    MPI_Init(&argc,&argv);
    const int n_N=1000;
    const int n_K=5;
    gsl_rng_env_setup();
    gsl_rng *r=gsl_rng_alloc(gsl_rng_mt19937);
    int n_myrank;
    int n_totproc;
    MPI_Comm_size(MPI_COMM_WORLD,&n_totproc);
    MPI_Comm_rank(MPI_COMM_WORLD,&n_myrank);
    double d_numbers[n_N];
    int i,j;
    int n_maxi;
    if(n_myrank==0){
        for(i=0;i<n_N;i++)
            d_numbers[i]=gsl_rng_uniform(r);
    }
        
    if((int)((n_myrank+1)*n_N/(double)(n_totproc))>n_N) n_maxi=n_N;
    else n_maxi=(int)(n_myrank*n_N/(double)(n_totproc));
    for(j=0;j<n_K;j++)
        for(i=(int)(n_myrank*n_N/(double)(n_totproc));i<n_maxi;i++)
            d_numbers[i]=sqrt(d_numbers[i]);            
    MPI_Finalize();
    return 0;
}
