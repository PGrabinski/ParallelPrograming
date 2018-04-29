#include<stdio.h>
#include<math.h>

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
    const int n_L=40;
    
    int i;
    int n_N;
    int n_myrank=0;
    int n_totproc=1;
    double d_ava=0;
    double d_sqava=0;
    double d_disp=0;
    n_N=n_L*n_totproc;
    double d_numbers[n_N];
    for(i=0;i<n_N;i++){
        d_numbers[i]=i;
    }
    d_ava=d_Avarage(n_myrank,d_numbers,n_L,n_N);
    d_sqava=d_SqAvarage(n_myrank,d_numbers,n_L,n_N);
    if(n_myrank==0){
        d_disp=sqrt(d_sqava-d_ava*d_ava);
        printf("%f %f %f\n",d_ava,d_sqava,d_disp);
    }
    return 0;
}
