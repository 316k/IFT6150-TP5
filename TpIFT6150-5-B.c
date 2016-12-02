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
#include <time.h>

#include "FonctionDemo5.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/      
/*------------------------------------------------*/
#define NAME_IMG_OUT "B-segmentation"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc, char *argv[])
{
    int i,j;
    int seuilMV;
    float mean0,mean1,var0,var1; /* parametres statistiques des deux classes : a estimer  */
    int length,width;

    float** y;  /* image a segmenter */
    float** x;  /* champs d'etiquettes a estimer */
    float*  VectHist;
	
 	if(argc<2){
		printf("Usage :\n\t TpIFT6150-4-B image_a_segmenter\n\n");
		return 0;
	}
	
    /* Ouvrir l'image d'entree */
    y = LoadImagePgm(argv[argc - 1], &length, &width);
    x = fmatrix_allocate_2d(length, width);

	/* Lancer l'algorithme des K-Moyennes */

    // Centres initialisés au hasard
    float next_mean0 = -1, next_mean1 = -1;

    float* histogramme = malloc(sizeof(float) * GREY_LEVEL);
    compute_histo(y, length, width, histogramme);

    srand(time(NULL));
    mean0 = randomize() * 255;

    // On ne doit pas avoir deux centres égaux
    while((mean1 = randomize() * 255) == mean0);

    float tmp;
    
    if(mean0 > mean1) {
        // Échange les valeurs pour assurer que mean0 == centre de la classe des pixels noirs
        tmp = mean0;
        mean0 = mean1;
        mean1 = tmp;
    }

    int convergence = 0, debut_classe_1, iteration = 0;
    float somme0, somme1, total0, total1, new_mean0, new_mean1;
    
    while(!convergence) {

        somme0 = 0;
        somme1 = 0;
        total0 = 0;
        total1 = 0;
        
        debut_classe_1 = (mean1 - mean0) / 2.0 + mean0;

        // Classe 0
        for(i=0; i<debut_classe_1; i++) {
            somme0 += i*histogramme[i];
            total0 += histogramme[i];
        }

        // Classe 1
        for(i=debut_classe_1; i<=GREY_LEVEL; i++) {
            somme1 += i*histogramme[i];
            total1 += histogramme[i];
        }

        new_mean0 = somme0/total0;
        new_mean1 = somme1/total1;

        printf("%d : %f %f\n", iteration, new_mean0, new_mean1);
        iteration++;
        
        convergence = new_mean0 == mean0 && new_mean1 == mean1;

        mean0 = new_mean0;
        mean1 = new_mean1;
    }

    total0 = total1 = 0;

    // Classe 0
    for(i=0; i<debut_classe_1; i++) {
        somme0 += histogramme[i] * SQUARE(i - mean0);
        total0 += histogramme[i];
    }
    var0 = somme0 / total0;

    // Classe 1
    for(i=debut_classe_1; i<=GREY_LEVEL; i++) {
        somme1 += histogramme[i] * SQUARE(i - mean1);
        total1 += histogramme[i];
    }
    var1 = somme1 / total1;
    
    for(i=0; i<length; i++)
        for(j=0; j<width; j++) {
            x[i][j] = (y[i][j] > debut_classe_1) * 255;
        }

    printf("%f %f\n", var0, var1);
    
    /* Sauvegarde du champ d'etiquettes */
    SaveImagePgm(NAME_IMG_OUT, x, length, width);
    
    /* Liberer la memoire des images */
    free_fmatrix_2d(x);
    free_fmatrix_2d(y);

    /*Commande systeme: visualisation de Ingout.pgm*/
    system("display B-segmentation.pgm&"); 
  
    /*retour sans probleme*/ 
    printf("\n C'est fini ... \n\n\n");
    return 0; 	 
}
