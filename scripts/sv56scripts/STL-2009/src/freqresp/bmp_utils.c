/* ..............................................................................................31/Mar/2005*/
/*																										v1.0*/

/*=============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

  DESCRIPTION : 
	This file contains functions used for the frequency response measure tool.

  FUNCTIONS :
	Global (have prototype in bmp_utils.h)
		draw_linesdB(...)	:	draw input vectors in an image
		sav_bmp(...)		:	save an image to a bitmap file

  HISTORY :
	31.Mar.05	v1.0	First Beta version (STL2005)
	Dec.09		v1.0.1  Header correction (STL2009)
  AUTHORS :
	Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* define a bitmap structure */
typedef struct				
{
	char sign[3];				/* signature BM	*/
	unsigned long size;			/* size of the image */
	unsigned long res;			/* reserved zone */
	unsigned long offset;		/* offset where image begins */
	unsigned long info;			/* Info zone size */
	unsigned long height;		/* height of the image*/
	unsigned long width ;		/* width of the image*/
	unsigned short nbLayers;	/* number of layers*/
	unsigned short bppixel;		/* number of bits per pixel*/
	unsigned long comp;			/* compression format*/
	unsigned long tim;			/* size of the image in bytes*/
	unsigned long Hres;			/* horizontal resolution*/
	unsigned long Vres;			/* vertical resolution*/
	unsigned long nbco;			/* number of colors*/
	unsigned long impco;		/* number of important colors*/
} bitmap;


static void swapShort(unsigned short *sp) {
	unsigned char *cp, t;
	cp= (unsigned char *)sp;
	t = cp[0]; 
	cp[0] = cp[1]; 
	cp[1] = t;
}

static void swapLong(unsigned long *lp) {
	unsigned char *cp, t;
	cp= (unsigned char *)lp;
	t = cp[0]; 
	cp[0] = cp[3]; 
	cp[3] = t;
	t = cp[1]; 
	cp[1] = cp[2]; 
	cp[2] = t;
}

static void swapHeader(bitmap* bmp) {
	swapLong(&bmp->res);
	swapLong(&bmp->size); 
	swapLong(&bmp->offset);
	swapLong(&bmp->info);
	swapLong(&bmp->height);
	swapLong(&bmp->width);
	swapLong(&bmp->comp);
	swapLong(&bmp->tim);
	swapLong(&bmp->Hres); 
	swapLong(&bmp->Vres);
	swapLong(&bmp->nbco);
	swapLong(&bmp->impco);
	swapShort(&bmp->nbLayers);
	swapShort(&bmp->bppixel);
}

void sav_bmp (int width, int height,char* image,char * bmpFileName, int little_endian) 
{
	bitmap* bmp ;
	FILE *fp ;

	int i , j ;
	/* background color */
	unsigned char R0=255, V0=255, B0=255;
	/* axes color */
	unsigned char R1=0, V1=0, B1=0;
	/* curve1 color */
	unsigned char R2=255, V2=0, B2=0;
	/* curve2 color */
	unsigned char R3=0, V3=0, B3=255;

	/* allocate memory for the bitmap definition */
	bmp=(bitmap*)malloc(sizeof(bitmap));
	
	/* define the bitmap format */
	bmp->sign[0] = 'B' ;
	bmp->sign[1] = 'M' ;
	bmp->res = 0 ;
	bmp->offset = 54 ;
	bmp->info = 40 ;
	bmp->height = width ;
	bmp->width = height ;
	bmp->nbLayers = 1 ;
	bmp->bppixel = 24 ;
	bmp->comp = 0 ;
	bmp->Hres = 2834 ; 
	bmp->Vres = 2834 ;
	bmp->nbco = 0 ;
	bmp->impco = 0 ;
	bmp->tim = (bmp->width * bmp->height * 3);
	bmp->size = bmp->tim + 54 ; 

	/* swap bytes if big-endian platform */
	if (little_endian==0)
		swapHeader(bmp);

	/* open the bitmap file to create */
	fp = fopen(bmpFileName,"wb");
	if (fp==NULL) {
		fprintf(stderr,"Error: can't open bitmap file!\n");
		exit(-1);
	}
	
	/* write the bitmap header */
	fwrite(bmp->sign,2,1,fp) ;
	fwrite(&bmp->res,4,1,fp) ;
	fwrite(&bmp->size,4,1,fp) ;
	fwrite(&bmp->offset,4,1,fp) ;
	fwrite(&bmp->info,4,1,fp) ;
	fwrite(&bmp->height,4,1,fp) ;
	fwrite(&bmp->width,4,1,fp) ;
	fwrite(&bmp->nbLayers,2,1,fp) ;
	fwrite(&bmp->bppixel,2,1,fp) ;
	fwrite(&bmp->comp,4,1,fp) ;
	fwrite(&bmp->tim,4,1,fp) ;
	fwrite(&bmp->Hres,4,1,fp) ;
	fwrite(&bmp->Vres,4,1,fp) ;
	fwrite(&bmp->nbco,4,1,fp) ;
	fwrite(&bmp->impco,4,1,fp) ;

	if (little_endian==0) {
		swapLong(&bmp->height);
		swapLong(&bmp->width);
	}

	/* write the image */
	for(i=0;i<(int)bmp->height ; i++)
		for(j=0 ; j<(int)bmp->width ; j++ )
		{
			if(image[i*bmp->width+j]==1) {
				fwrite(&B1,1,1,fp) ;	
				fwrite(&V1,1,1,fp) ;
				fwrite(&R1,1,1,fp) ;
			} else if(image[i*bmp->width+j]==2) {
				fwrite(&B2,1,1,fp) ;	
				fwrite(&V2,1,1,fp) ;
				fwrite(&R2,1,1,fp) ;
			}
			else if(image[i*bmp->width+j]==3) {
				fwrite(&B3,1,1,fp) ;	
				fwrite(&V3,1,1,fp) ;
				fwrite(&R3,1,1,fp) ;
			} else {
				fwrite(&B0,1,1,fp) ;	
				fwrite(&V0,1,1,fp) ;
				fwrite(&R0,1,1,fp) ;
			}
		}
	
	/* close bitmap file */
	fclose(fp) ;
}


void draw_linesdB(char* image, float* avg1PowSp, int lgth, int im_wdth, int im_hght, int border, float absMax, float absMin, float ordMax, float ordMin,int n) {
	int i, j;
	int x, y, x1, x2, y1, y2, tmp;
	double b, alpha;

	for(i=(long) absMin;i<(long) absMax;i++) {	/* loop over the avgPowSp vector */
		y1=(int) ((im_hght-border/2)*(10*log10(avg1PowSp[i])-ordMin)/ (ordMax-ordMin) + border/2);
		y2=(int) ((im_hght-border/2)*(10*log10(avg1PowSp[i+1])-ordMin)/ (ordMax-ordMin) + border/2);
		x1=(int) ( im_wdth*(i-absMin)/ (double) (absMax - absMin) + border/2 );
		x2=(int) ( im_wdth*(i+1-absMin)/(double) (absMax- absMin) + border/2 );
		alpha=(y2 - y1) / (double) (x2 - x1);
		b = y2 - alpha*x2;
		if(abs((int) alpha)<=1.0) {
			if(x1>=im_wdth) x1=im_wdth-1;
			if(x1<0) x1=0;
			if(x2>=im_wdth) x2=im_wdth-1;
			if(x2<0) x2=0;
			if(x1>x2) {
				tmp=x2;
				x2=x1;
				x1=tmp;
			}
			for(j=x1;j<=x2;j++) {
				y = (int) ( alpha * j + b );
				if(y>=im_hght) y=im_hght-1;
				if(y<0)	y=0;
				image[y*im_wdth+j]=n;
			}
		} else {
			if(y1>=im_hght) y1=im_hght-1;
			if(y1<0) y1=0;
			if(y2>=im_hght) y2=im_hght-1;
			if(y2<0) y2=0;
			if(y1>y2) {
				tmp=y2;
				y2=y1;
				y1=tmp;
			}
			for(j=y1;j<=y2;j++) {
				x = (int) ((j - b) / alpha );
				if(x>=im_wdth) x=im_wdth-1;
				if(x<0)	x=0;
				image[j*im_wdth+x]=n;
			}
		}		
	}
}
