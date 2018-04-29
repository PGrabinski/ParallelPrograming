#include<stdio.h>
#include<gsl/gsl_rng.h>
#define N 5000 //number of random walkers
#define T 500 //number of steps
#define L 80//size of square lattice
#define S 0.1 //start area size(part of lattice)
#define H
#define W
double R1=0,R2=0,D1,D2,p=0;
int t1,t2,help=0;

typedef struct{
    int y;
    int x;
}particle;

typedef struct{
    int R;
    int G;
    int B;
}RGB;



void Skipline(FILE *fp){
    for(; fgetc(fp) != '\n';);
}

void Read(FILE *w,RGB o[][W],Lattice[][W]){
    skipline(w);
    skipline(w);
    skipline(w);
    skipline(w);
    for(i=0;i<H;i++){
    	for(j=0;j<W;j++){
    	    fscanf(w,"%d %d %d ", &o[i][j].R,&o[i][j].G,&o[i][j].B);
    	}
    }
    for(i=0;i<H;i++){
    	for(j=0;j<W;j++){
    	    fscanf(w,"%d %d %d ", &o[i][j].R,&o[i][j].G,&o[i][j].B);
    	}
    }
    
}

double RW2(gsl_rng *q,int u){//Random Walk in Porosity Media
    particle rw;
    int a,b,p,i,px=0,lx=0,gy=0,dy=0;
    double r;
    a=H/2-H*S*gsl_rng_uniform(q);
    b=W/2-W*S*gsl_rng_uniform(q);
    rw.x=a;//pionowa
    rw.y=b;//pozioma
    if(Lattice[a][b]==1)
        return 0;
    for(i=0;i<u;i++){
    	p=((int)(3.99*gsl_rng_uniform(q)));
            switch(p){
                case 0:
                    if(rw.x+2-px*H+lx*H>H){
                        if(Lattice[rw.x-(px+1)*H+lx*H+1][rw.y-gy*W+dy*W]==1){
                            continue;
                        }
                        else{
                            px++;
                            continue;
                        }
                    }
                    if(Lattice[rw.x-px*H+lx*H+1][rw.y-gy*W+dy*W]==1) break;
                    rw.x++;
                    break;
                case 1:
                    if(rw.x-1-px*H+lx*H<0){
                        if(Lattice[rw.x-(px)*H+(lx+1)*H-1][rw.y-gy*W+dy*W]==1){
                            continue;
                        }
                        else{
                            lx++;
                            continue;
                        }
                    }
                    if(Lattice[rw.x-1-px*H+lx*H][rw.y-gy*W+dy*W]==1) break;
                    rw.x--;
                    break;
                case 2:
                    if(rw.y+2-gy*W+dy*W>W){
                        if(Lattice[rw.x-(px)*H+lx*H][rw.y-(gy+1)*W+dy*W+1]==1){
                            continue;
                        }
                        else{
                            gy++;
                            continue;
                        }
                    }
                    if(Lattice[rw.x-px*H+lx*H][rw.y+1-gy*W+dy*W]==1) break;
                    rw.y++;
                    break;
                case 3:
                    if(rw.y-1-gy*W+dy*W<0){
                        if(Lattice[rw.x-(px)*H+lx*H][rw.y-(gy)*W+(dy+1)*W-1]==1){
                            continue;
                        }
                        else{
                            dy++;
                            continue;
                        }
                    }
                    if(Lattice[rw.x-px*H+lx*H][rw.y-1-gy*W+dy*W]==1) break;
                    rw.y--;
                    break;
            }
    }
    r=(rw.x-a)*(rw.x-a)+(rw.y-b)*(rw.y-b);
        t2++;
        return r;
}

double RW1(gsl_rng *q,int u){//Random Walk in Clean Lattice
    particle rw;
    int a,b,p,i,px=0,lx=0,gy=0,dy=0;
    double r;
    a=L/2-H*S*gsl_rng_uniform(q);
    b=L/2-W*S*gsl_rng_uniform(q);
    rw.x=a;
    rw.y=b;
    for(i=0;i<u;i++){
	p=((int)(3.99*gsl_rng_uniform(q)));
        switch(p){
            case 0:
                if(rw.x+2-px*H+lx*H>H){
                    px++;
                    continue;
                }
                rw.x++;
                break;
            case 1:
                if(rw.x-1-px*H+lx*H<0){
                    lx++;
                    continue;
                }
                rw.x--;
                break;
            case 2:
                if(rw.y+2-gy*W+dy*W>W){
                    gy++;
                    continue;
                }
                rw.y++;
                break;
            case 3:
                if(rw.y-1-gy*W+dy*W<0){
                    dy++;
                    continue;
                }
                rw.y--;
                break;
        }
    }
    r=(rw.x-a)*(rw.x-a)+(rw.y-b)*(rw.y-b);
        t1++;
        return r;
}


int main(int argc, char **argv){
    int i,c;
    
    gsl_rng_env_setup();
    gsl_rng *r=gsl_rng_alloc(gsl_rng_mt19937);
    
    RGB o[H][W];
    int Lattice[H][W];
    
    FILE *w=fopen(".ppm","r");
    Read(w,o,Lattice);
    for(c=1;c<T;c++){
    	t1=0;
    	t2=0;
    	for(i=0;i<N;i++)
            R1+=RW1(r,c);
    	R1=R1/t1;
    	D1=R1/(4*T);
    	for(i=0;i<N;i++){
    	    R2+=RW2(r,c);
    	}
    	R2=R2/t2;
    	D2=R2/(4*T);
    	printf("%f %d %f\n",R1,c,R2);
    }
    p=p/((float) L*L);
    printf("%f\n",p);
    return 0;
}
