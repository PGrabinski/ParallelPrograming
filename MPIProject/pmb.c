#include<stdio.h>
#include<mpi.h>
#include<sys/time.h>
#define W 1000
#define H 1000
#define T 300

/*pixel*/
typedef struct{
  int R;
  int G;
  int B;
}RGB;

RGB o[H][W];

void Mandelbrot(int Start,int Total){
    int i,j,t,WMax;
    if((int)((Start+1)*W/(float)(Total))>W) WMax=W;
    else WMax=(int)((Start+1)*W/(float)(Total));
    for(i=0;i<H;i++){
        for(j=(int)(Start*W/(float)(Total));j<WMax;j++){
            float x0=j/(float)(W)*3.5-2.5,y0=i/(float)(H)*2-1,x=0,y=0,xt=0;
            t=0;
    	while(x*x+y*y<4 && t<T){
                xt=x*x-y*y+x0;
        	y=2*x*y+y0;
        	x=xt;
                t++;
            }
            if(t/(float)(T)<1/(float)(3)){
                o[i][j].R=(int)(t/(float)(T)*3*255);
            }
            else{
                if(t/(float)(T)<2/(float)(3)){
                     o[i][j].R=255;
                     o[i][j].G=(int)(t/(float)(T)*3/2*255);
                }
                else{
                    o[i][j].R=255;
                    o[i][j].G=255;
                    o[i][j].B=(int)(t/(float)(T)*255);
                }
            }
        }
    }
}


int main(int argc, char *argv[]){
    struct timeval startCPU,stopCPU;
    int n_N;
    gettimeofday(&startCPU,NULL);
    MPI_Init(&argc, &argv);
    int rank,totproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &totproc);
    Mandelbrot(rank,totproc);
    if(rank==0) n_N=totproc;
    MPI_Finalize();
    if(rank==0){
        gettimeofday(&stopCPU,NULL);
        long int d_TotTimeCPU=(stopCPU.tv_sec-startCPU.tv_sec)*1000000+(stopCPU.tv_usec-startCPU.tv_usec);
        printf("%d %ld\n",n_N,d_TotTimeCPU);
    }
    return 0;
}
