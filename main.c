
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
//#include<SDL2/SDL_image.h>
//#include<SDL2/SDl_timer.h>
#include "nn1.h"


#undef main
#define arrSize(x) sizeof(x) / sizeof(x[0]) 





int main()
{
	int height,width,chanels;
    //uint8_t *img = stbi_load("golden_forest_by_sandara_de88blz-pre.jpg",&width,&height,&chanels,0);  
    printf("\nInput name of a image: ");
    char str[300] ;
    gets(str);
    
    uint8_t *img = stbi_load(str,&width,&height,&chanels,0);
    	if(img==NULL)
        {printf("ERROR LOADING"); return -1;}
    uint32_t imgSize = width*height*chanels;
   
    
    float *imgTr = (float*)calloc(imgSize,sizeof(float));



	printf("\nimgSize: %d PIXELS chanel %d",imgSize,chanels);
    uint32_t pretrain = 0;
    uint32_t numOfTime = 0;
	printf("\nDo you want to pretrain image? (Recomended for larger images 1 if YES)   "); 
	scanf("%u", &pretrain);
	if(pretrain == 1)
	{
		printf("\nHow much iterations:  ");
		scanf("%u", &numOfTime); 
	}
    //NN
    	NN nn;
    	nn.Nin = imgSize;
    	nn.Nout = imgSize;
    	allocNN(&nn);
    	init(&nn);
    	
    	for(size_t i = 0; i < imgSize;i++){
    		nn.input[i] = (float)img[i]  / 255.0f;
    		nn.corect[i] = (float)img[i] / 255.0f;
		}
		//forward(&nn);
		if(pretrain==1){
		
		for(size_t i = 0; i  < numOfTime;i++)
	    {
			back(&nn);
			forward(&nn);
			printf("\nTRAINING ITERATION %d\n",i+1);
		}
		}
		for(size_t i = 0;i < imgSize;i++){
			imgTr[i] = (uint8_t)(nn.out[i]*255);		
		}
    //
         
	// Renderern
	SDL_Window *window = NULL;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(width, height,SDL_RENDERER_ACCELERATED,&window,&renderer);

	while(1){
	
     forward(&nn);
     back(&nn);
		for(size_t i = 0;i < imgSize;i++){
			imgTr[i] = (uint8_t)(nn.out[i]*255.0f);
		}

	{
			
	SDL_RenderClear(renderer);
	
	size_t i = 0; 
	for(size_t y = 0; y < height;y++)
	for(size_t x = 0; x < width;x++)
	{
		
		SDL_SetRenderDrawColor(renderer,imgTr[i++],imgTr[i++],imgTr[i++],0);
		SDL_RenderDrawPoint(renderer,x,y);
		//system("pause");
	}
	
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderPresent(renderer);
	}		
		SDL_Event event;
		if(SDL_PollEvent(&event)){
			
		}
		
	}
	

	return 0;
}
