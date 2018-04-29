#include<stdio.h>
#include<cuda.h>
#include<cuda_runtime.h>
#include<assert.h>
#include<gsl/gsl_rng.h>
#include<time.h>

#define CHECK_CUDA(x)           \
{                               \
  cudaError_t err = x;          \
  if (err != cudaSuccess)       \
  {                             \
     printf("!!! CUDA ERROR: \"%s\" at file %s, line %d !!!\n", cudaGetErrorString(err), __FILE__, __LINE__);\
     exit(1);                   \
  }                             \
}

void CreateUniverse(gsl_rng * rng_r, int n_N, int n_Size, float4 *Galaxy_h){
    int i;
    for(i=0;i<n_N;i++){
        Galaxy_h[i].x=gsl_rng_uniform(rng_r)*n_Size-n_Size*0.5;
        Galaxy_h[i].y=gsl_rng_uniform(rng_r)*n_Size-n_Size*0.5;
        Galaxy_h[i].z=gsl_rng_uniform(rng_r)*n_Size-n_Size*0.5;
    }
}

//Kernel
__global__ void PotentialOnDevice(float4 *Galaxy_d, int n_N){
    int idx = blockIdx.y  * gridDim.x * blockDim.x + blockIdx.x*blockDim.x + threadIdx.x;
    if(idx<n_N){
        float4 Star=Galaxy_d[idx];
        Star.w=1.0/sqrt(Star.x*Star.x+Star.y*Star.y+Star.z*Star.z);
        Galaxy_d[idx].w=Star.w;
    }
}

//CPU
void PotentialOnHost(float4 *Galaxy_h, int n_N){
    int i;
    for(i=0;i<n_N;i++)
        Galaxy_h[i].w=1/sqrt(Galaxy_h[i].x*Galaxy_h[i].x+Galaxy_h[i].y*Galaxy_h[i].y+Galaxy_h[i].z*Galaxy_h[i].z);
}


int main(int argc, char* argv[]){
    if (argc != 3)
    {
      printf ("error: expected 2 arguments usage\n");
      return -1;
    }

    //Number of objects
    const int n_N=atoi(argv[1]);

    //Size of Galaxy
    const int n_Size=atoi(argv[2]);

    gsl_rng_env_setup();
    gsl_rng *rng_r=gsl_rng_alloc(gsl_rng_mt19937);

    float4 *Galaxy_h;     //pointers to host memory
    float4 *Galaxy_d;     //pointers to device memory

    //allocate arrays on host
    Galaxy_h = new float4[n_N];

    //allocate arrays on device
    CHECK_CUDA(cudaMalloc((void **) &Galaxy_d, sizeof(float4)*n_N));

    //Generate objects coord.
    CreateUniverse(rng_r,n_N,n_Size,Galaxy_h);

    CHECK_CUDA(cudaMemcpy(Galaxy_d, Galaxy_h, sizeof(float4)*n_N, cudaMemcpyDefault));

    //KernelSize
    dim3 blockSize = 512;
    dim3 gridSize (1,1,1);
    const int max_block_size = 65535;
    int nBlocks = n_N/blockSize.x + (n_N%blockSize.x == 0 ? 0 : 1);
    gridSize.y = 1 + nBlocks/max_block_size;
    gridSize.x = (nBlocks > max_block_size) ? max_block_size : nBlocks;


    //GPU Potential
    struct timespec startGPU,stopGPU;
    clock_gettime(CLOCK_MONOTONIC, &startGPU);
    PotentialOnDevice <<< gridSize, blockSize >>> (Galaxy_d, n_N);
    cudaDeviceSynchronize();
    clock_gettime(CLOCK_MONOTONIC, &stopGPU);
    CHECK_CUDA(cudaMemcpy(Galaxy_h, Galaxy_d, sizeof(float4)*n_N, cudaMemcpyDefault));
    long int d_TotTimeGPU=(stopGPU.tv_sec-startGPU.tv_sec)*1000000000+stopGPU.tv_nsec-startGPU.tv_nsec;

    //CPU Potential
    struct timespec startCPU,stopCPU;
    clock_gettime(CLOCK_MONOTONIC, &startCPU);
    PotentialOnHost(Galaxy_h, n_N);
    clock_gettime(CLOCK_MONOTONIC, &stopCPU);
    long int d_TotTimeCPU=(stopCPU.tv_sec-startCPU.tv_sec)*1000000000+(stopCPU.tv_nsec-startCPU.tv_nsec);
    printf("%d %d %ld %ld\n",n_Size,n_N,d_TotTimeCPU,d_TotTimeGPU);

    cudaFree(Galaxy_d);
    return 0;
}
