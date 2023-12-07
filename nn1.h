#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdint.h>
#include<assert.h>

//#define Nin  2395872
//#define Nout 2395872
#define Nneuron 1
#define lr 1
#define SEED   32


size_t Nin,Nout;


typedef struct NN
{

    float *weightso;
    float *weightsi;
    float *neuron;   //Midle storage  
    float biasi;
    float biaso;
    float *input;
    float *out;
    float *corect;
    size_t Nin;
    size_t Nout;
}NN;

void allocNN(NN *nn)
{
	Nin = nn->Nin;
	Nout = nn->Nout;
	nn->corect = (float*)calloc(Nout,sizeof(float));
	nn->out = (float*)calloc(Nout,sizeof(float));
	nn->input = (float*)calloc(Nin,sizeof(float));
	nn->neuron = (float*)calloc(Nneuron,sizeof(float));
	nn->weightso = (float*)calloc(Nout*Nneuron,sizeof(float));
	nn->weightsi = (float*)calloc(Nout*Nneuron,sizeof(float));
}


float RandF(void)
{

    //return rand()%255;
    //if(rand()%2==0)
        return -1.0f*rand()/((float)RAND_MAX);
    //else 
        //return 1.0f*rand()/((float)RAND_MAX);
   
}

void init(NN *nn)
{
    	srand(time(0));
	  	  //srand(32);
    for (size_t i = 0; i < Nin; i++)
        nn->input[i]=0.0f;

    for (size_t i = 0; i < Nout; i++)
        nn->out[i]=0.0f;

       for (size_t i = 0; i < Nout; i++)
        nn->corect[i]=0.0f;
    
    for (size_t i = 0; i < Nin*Nneuron; i++)
        nn->weightsi[i]=RandF();

    for (size_t i = 0; i < Nin*Nneuron; i++)
        nn->weightso[i]=RandF();

        nn->biasi=0.0f;
        nn->biaso=0.0f;

    for (size_t i = 0; i < Nneuron; i++)
        nn->neuron[i]=0.0f;

}

float activation(uint8_t x)
{
    //if(x > 0) return (float)(x%255); else return 0;
    //return 255.0f/(1.0f+exp(-1/50*x));
    return 1.0f/(1.0f+exp(-1*x));
    //return tanhf(x);
    ///return sin(x);
    //return fabsf(x);
} 
void forward(NN *nn)   //done
{

    int prescaler=0;
    for (size_t i = 0; i < Nneuron; i++)  //weights    ulaz
    {
        size_t j = 0;
        nn->neuron[i]=0.0f;
        for (j = 0; j < Nin ; j++)  // od prijasnjeg
        nn->neuron[i]+=(nn->weightsi[j+prescaler]*nn->input[j]);
        //nn->neuron[i]+=nn->biasi[i];
        nn->neuron[i]=activation(nn->neuron[i]+nn->biasi);
        prescaler+=j;
        //printf("in %d\n",prescaler);

    }
    prescaler=0;
     for (size_t i = 0; i < Nout; i++)  //weights    ulaz
    {
        size_t j=0;
        nn->out[i]=0.0f;
        for (j = 0; j < Nneuron ; j++)  // od prijasnjeg
            nn->out[i]+=(nn->weightso[j+prescaler]*nn->neuron[j]);
        //nn->out[i]+=nn->biaso[i];
        nn->out[i]=activation(nn->out[i]+nn->biaso);
        prescaler+=j;
        //printf("out %d\n",prescaler);
    }

}
 


void back(NN *nn)
{
      float sum=0.0f,sumb=0.0f;
      int prescaler_in=0,prescaler_out=0;
      for (size_t i = 0; i < Nneuron; i++)//go thru neurons
      {
          size_t j = 0;
          for (j = 0; j < Nout; j++)   //adjustment in out layer
          {
              const float derror=nn->out[j]-nn->corect[j];
              const float dsigmoid=nn->out[j]*(1.0f-nn->out[j]);
              //float dsigmoid;
			  //if(nn->out[j] > 0) dsigmoid = 1; else  dsigmoid = 0;
              sum+=lr*derror*dsigmoid*nn->weightso[j+prescaler_in];
              sumb+=lr*derror*dsigmoid*nn->biaso;
              nn->weightso[j+prescaler_in]-=lr*derror*dsigmoid*nn->neuron[i];
              //nn->biaso-=lr*derror*dsigmoid*nn->neuron[i];

          }
          size_t k = 0;
          for (k = 0; k < Nin; k++)  //adjustment in input layer
          {
            const float hv=nn->neuron[i]*(1.0f-nn->neuron[i]);
             //float hv;
             //if(nn->neuron[i] > 0 ) hv=1; else hv=0;
             nn->weightsi[k+prescaler_out]-=lr*sum*hv*nn->input[k];
             //nn->biasi-=lr*sumb*hv*nn->input[k];
          }
          prescaler_in+=j;
          prescaler_out+=k;       
      }
      
}

void print(NN *nn)
{
    forward(nn);
    printf("\n");
    for (size_t i = 0; i < Nout*Nneuron; i++)
        printf("wo %f ",nn->weightso[i]);
    printf("\n");
    for (size_t i = 0; i < Nin*Nneuron; i++)
        printf("wi %f ",nn->weightsi[i]);
    printf("\n");

	printf("biasi %f biaso %f\n",nn->biasi,nn->biaso);

}

void printout(NN *nn)
{
   for(size_t i = 0; i < 100; i++)
	   printf("%d \n",nn->out[i] );
	   system("pause");
}



