/*------------------------------------------------------*/
/* Prog    : ProgDemoN.c                                */
/* Auteur  : Mercedes Gauthier, Nicolas Hurtubise       */
/* Date    :                                            */
/* version :                                            */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo5.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/      
/*------------------------------------------------*/

#define NAME_IMG_OUT1 "A-segmentation" 
#define NAME_IMG_OUT2 "A-histogramme" 

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc, char *argv[])
{
    int i,j;
    int seuilMV;
    int length,width;

    float** y;  /* image a segmenter */
    float** x;  /* champ d'etiquettes a estimer */
    float*  VectHist;

 
 	if(argc<2){
		printf("Usage :\n\t TpIFT6150-4-A image_a_segmenter\n\n");
		return 0;
	}
 
    printf("Entrez un seuil: ");
    scanf("%d",&seuilMV);
 
    /* Ouvrir l'image d'entree */
    y = LoadImagePgm(argv[argc - 1], &length, &width);
    x = fmatrix_allocate_2d(length, width);
    
    /* Seuil */
    for(i=0; i<length; i++)
        for(j=0; j<width; j++) {
            x[i][j] = (y[i][j] > seuilMV) * 255;
        }

    /* Sauvegarde du champ d'etiquettes et de l'histogramme de l'image d'entree */
    
    SaveImagePgm(NAME_IMG_OUT1, x, length, width);

    float* histogramme = malloc(sizeof(float) * GREY_LEVEL);
    compute_histo(y, length, width, histogramme);
    SaveHistoPgm(NAME_IMG_OUT2, histogramme);
    
    /* Liberer la memoire des images */
    free_fmatrix_2d(x);
    free_fmatrix_2d(y);
    /*Commande systeme: visualisation de Ingout.pgm*/
    system("display A-segmentation.pgm&"); 
    system("display A-histogramme.pgm&");
  
    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
