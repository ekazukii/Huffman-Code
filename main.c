/**
 * @file main.c
 * @brief Fichier contenant toutes les fonctions principales et celles liées à l'histogramme 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "main.h"
#include "bin.h"
#include "arbre.h"

PtrListe pile= NULL, lst= NULL; 
dicoElem dico[256];
PArbre TA;
struct element *histogramme;

const int N = 256;

/**
 * @brief Parcoure le fichier et créer un histogramme (tableau) assosiant à chaque charactère 
 * le nombre d'occurences de celui ci dans le fichier source
 * 
 * @param fp Fichier source à parcourir 
 */
void creerHisto(FILE* fp) {
    histogramme = malloc(256 * sizeof(struct element));
    unsigned char ch;
    //while((ch = fgetc(fp)) !=EOF ) {
    while(!feof(fp)) {
        ch = fgetc(fp);
        if(feof(fp)) return;
        histogramme[ch].lettre = ch;
        histogramme[ch].occurences += 1;
        //printf("%c - %d - %d\n\n", histogramme[ch].lettre, histogramme[ch].occurences, ch);
    }
}

/**
 * @brief Sauvegarde l'histogramme dans un fichier texte
 * 
 * @param fichier
 */
void saveHisto(FILE* fichier) {
    for (int i = 0; i < 256; i++) {
        if(histogramme[i].occurences > 0) {
            fprintf(fichier, "%c%d\n", histogramme[i].lettre, histogramme[i].occurences);
        }
    }
}

/**
 * @brief Lit le fichier et retrouve l'histogramme sauvegardé
 * 
 * @param histof 
 */
void readHisto(FILE* histof) {
    histogramme = malloc(256 * sizeof(struct element));

    unsigned char ch;
    int test = 0;
    while(!feof(histof)) {
        ch = fgetc(histof);
        if(feof(histof)) return;

        fscanf(histof, "%d", &test);
        fgetc(histof); // We skip the breakline char
        histogramme[ch].lettre = ch;
        histogramme[ch].occurences = test;
    }
}

/**
 * @brief Convertit l'histogramme en une liste chainée d'arbre 
 * Chaque arbre correspond à un élément de l'histogramme
 * Les abres sont ajoutés par ordre croissant dans la liste
 *  
 * @param histo L'histogramme
 * @param liste Une liste chainée vide
 * @return PtrListe Un pointeur sur la liste
 */
PtrListe convertToArbreList(struct element *histo, PtrListe liste) {
    for (int i = 0; i < N; i++) {
        if(histo[i].occurences > 0) {
            PArbre A = creerArbreBin(histo[i].occurences, histo[i].lettre);
            liste = insertList(liste, A);
        }
    }

    return liste;
}

/**
 * @brief Affiche un message d'erreur lors de l'ouverture d'un fichier
 * 
 * @return int Le code d'erreur
 */
int fileError() {
    printf("Erreur: problème lors de l'ouverture d'un fichier\n");
    return -1;
}

/**
 * @brief Compresse un fichier texte et sauvegarde son histogramme associé
 * 
 * @param inputfile Le nom du fichier à compresser
 * @param outfile Le nom du fichier de sortie 
 * @param histofile Le nom du fichier ou l'histogramme sera sauvegardé
 * @return int 
 */
int encode(char *inputfile, char *outfile, char *histofile) {
    printf("Compression de %s\n", inputfile);
    // On créer l'histogramme en parcourant le fichier à encoder
    FILE* fichier = fopen(inputfile, "r");
    if(fichier == NULL) return fileError();
    creerHisto(fichier);

    // On recupere la taille du fichier
    int sizeIn = ftell(fichier);

    // On remet le fichier à sa position initiale
    fseek(fichier, 0, SEEK_SET);

    // On sauvegarde l'histogramme dans un fichier
    FILE* histof = fopen(histofile, "w");
    if(histof == NULL) return fileError();
    saveHisto(histof);
    fclose(histof);

    // On convertit l'histogramme en une liste chainée d'arbre binaire
    pile = convertToArbreList(histogramme, pile);

    // On créer l'arbre binanire permettant de trouver le codage huffman
    PArbre t = malloc(sizeof(struct arbre));
    t = creerArbreHuffman(pile);

    // Créer le dictionnaire associant tout les caractères à son codage huffman
    char bitArray[256];
    createDico(dico, t, bitArray, 0);
    
    // On encode le fichier en utilisant le dictionnaire
    FILE* outf = fopen(outfile, "w");
    if(outf == NULL) return fileError();
    encodeWrite(dico, fichier, outf);

    // On recupere la taille du fichier
    int sizeOut = ftell(outf);
    printf("Compression de %lf%% \n", ((double) sizeOut / sizeIn) * 100);

    fclose(outf);
    fclose(fichier);

    return 1;
}

/**
 * @brief Decompresse un fichier texte à partir de son histogramme
 * 
 * @param inputfile Le nom du fichier à decompresser
 * @param outfile Le nom du fichier de sortie (le fichier décodé) 
 * @param histofile Le nom du fichier de l'histogramme
 * @return int 
 */
int decode(char *compressedfile, char *outfile, char *histofile) {
    printf("Compression de %s\n", compressedfile);
    // On lit l'histogramme depuis le fichier 
    FILE* histof = fopen(histofile, "r");
    if(histof == NULL) return fileError();
    readHisto(histof);
    fclose(histof);

    // On convertit l'histogramme en une liste chainée d'arbre binaire
    pile = convertToArbreList(histogramme, pile);

    // On créer l'arbre binanire permettant de trouver le codage huffman
    PArbre t = malloc(sizeof(struct arbre));
    t = creerArbreHuffman(pile);

    FILE* fichier = fopen(compressedfile, "r");
    FILE* fd = fopen("bin.txt", "w");

    if(fichier == NULL || fd == NULL) return fileError();

    // On créer le fichier temporaire qui convertit le fichier "encodé" en un fichier contenant seulement les char '1' et '0'
    createBinFile(fichier, fd);

    fclose(fichier);
    fclose(fd);

    // On decode le fichier binaire temporaire
    FILE* outf = fopen(outfile, "w");
    if(outf == NULL) return fileError();
    decodeFromBinFile(t, outf);
    fclose(outf);

    // On supprime le fichier temporaire
    remove("bin.txt");
    return 1;
}

/**
 * @brief Affiche l'aide des options du programme
 * 
 * @return int 
 */
int help() {
    printf("Compresse le fichier en utilisant l'algorithme de huffman\n\n");
    printf("Syntaxe: huffman [-c|d|h]\n\n");
    printf("Options:\n");
    printf("  c source.txt out.hfzip histo.data - Compresse le fichier\n");
    printf("  d source.txt out.hfzip histo.data - Decompresse le fichier\n");
    printf("  h - Affiche l'aide\n\n");
    printf("Exemples:\n");
    printf("  ./huffman -c lorem.txt compressed.hfzip compressed.data\n");
    printf("  ./huffman -d compressed.hfzip decoded.txt compressed.data\n");
    
    return -1;
}

/**
 * @brief Affiche un message d'erreur lors du mauvaise utilisations des paramètres du programme
 * 
 * @return int 
 */
int badParams() {
    printf("Erreur: Mauvais parametres\n");
    printf("Utilisez huffman -h pour plus d'information");
    
    return -1;
}


/**
 * @brief Fonction main, elle gère les paramètres 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) {
    clock_t before = clock();

    char* command = argv[1];

    switch (command[1]) {
    case 'c':
        if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) return badParams();
        encode(argv[2], argv[3], argv[4]);
        break;
    case 'd':
        if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) return badParams();
        decode(argv[2], argv[3], argv[4]);
        break;
    case 'h':
        return help();
        break;
    default:
        return badParams();
        break;
    }

    clock_t difference = clock() - before;
    int msec = difference * 1000 / CLOCKS_PER_SEC;

    printf("Fini en %d secondes %d millisecondes\n", msec/1000, msec%1000);
}