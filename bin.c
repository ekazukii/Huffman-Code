/**
 * @file bin.c
 * @brief Contient les fonctions permettant la gestion des caractères en binaire
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "main.h"
#include "arbre.h"
#include "bin.h"

/**
 * @brief Convertit un caractère en son écriture en binaire
 * Retourne un tableau de huit caractères correspondant à son écriture en binaire
 * Chaque caractère du tableau est soit '0' soit '1'
 * @param c 
 * @param binArr 
 */
void toBinary(char c, char binArr[8]) {
    for (int i = 7; i >= 0; --i) {
        binArr[i] = ((c & (1 << (7 - i))) ? '1' : '0' );
    }
}

void printBitArray(dicoElem el) {
    for (int i = 0; i < el.bitNumber; i++) {
        printf("%d ", el.bitArray[i]);
    }
    printf("\n");
}

/**
 * @brief Créer un fichier ->temporaire<- convertissant chaque caractères du fichier compressé par
 * huffman en un fichier texte contenant l'écriture en binaire de chaque caractères. Cette fonction sert
 * à simplifier la decompression du fichier. 
 * 
 * @param encoded Le fichier compressé
 * @param binfile Le fichier texte l'écriture en binaire
 */
void createBinFile(FILE* encoded, FILE* binfile) {

    unsigned char ch;
    while(!feof(encoded)) {
        ch = fgetc(encoded);

        char binArr[8];
        toBinary(ch, binArr);
        for (int i = 0; i < 8; i++) {
            fputc(binArr[i], binfile);
        }
    }
}

/**
 * @brief Parcoure le fichier binaire precedement créé et decode ce fichier 
 * à partir de l'abre de Huffman.
 * @param A Arbre de codage huffman
 * @param fd Le fichier de sortie qui contiendra le texte decompressé
 */
void decodeFromBinFile(PArbre A, FILE* fd) {
    FILE* fichier = NULL;
    fichier = fopen("bin.txt", "r");

    unsigned char ch;

    PArbre tmp = A;
    while(!feof(fichier)) {
        //printf("Called ");
        ch = fgetc(fichier);

        if(ch == '1') tmp = tmp->fd;
        if(ch == '0') tmp = tmp->fg;

        if(tmp->fd == NULL && tmp->fg == NULL) {
            //printf("RESET \n");
            fputc(tmp->el.lettre, fd);
            tmp = A;
        }
    }

    fclose(fichier);
}

/**
 * @brief ceci est la fonction qui permet d'encoder le fichier
 * 
 * @param dico dictionnaire contenant le codage de chaque lettres 
 * @param fichier fichier d'entrée
 * @param fo fichier de sortie 
 */
void encodeWrite(dicoElem dico[256], FILE* fichier, FILE* fo) {
    unsigned char ch;
    int shift = 0;
    int tmpShift = 0;
    char outChar = 0;
    while(!feof(fichier)) {
        ch = fgetc(fichier);
    //while((ch = fgetc(fichier)) != EOF) {
        char bitArray[256];
        copyArray(dico[ch].bitArray, bitArray);
        for (int i = 0; i < dico[ch].bitNumber; i++) {
            if(shift == 8) {
                fputc(outChar, fo);
                outChar = 0;
                shift = 0;
            }
            if(bitArray[i] == 1) {
                outChar = outChar | (1 << (7 - shift));
            }
            shift++;
        }
        //shift = 0;
    }
}