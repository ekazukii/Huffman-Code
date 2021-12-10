/**
 * @file arbre.c
 * @brief Contient les fonctions de gestion des arbre binaires
 * 
 */
#define MALLOC(x)((x* ) malloc(sizeof(x)))
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "arbre.h"
#include "bin.h"

// Creation d'un noeud de valeur x et de suivant suiv,retourne le noeud cree
PtrListe creerNoeud(PArbre x,  PtrListe suiv) {
    PtrListe ptr;
    if ((ptr= MALLOC(Liste)) == NULL){
        fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
        exit(1);
    }
    ptr->el= x;
    ptr->suiv= suiv;
    return ptr;
}

/**
 * @brief Insert un élément de manière croissante dans un liste
 * 
 * @param tete Pointeur vers une liste chainée
 * @param val Pointeur vers l'abre à inserer
 * @return PtrListe Pointeur vers la nouvelle liste
 */
PtrListe insertList(PtrListe tete, PArbre val) {
    PtrListe p = creerNoeud(val, NULL);

    if(tete == NULL) {
        return p;
    } else {
        if(tete->suiv == NULL) {
            if(tete->el->el.occurences < val->el.occurences) {
                tete->suiv = p;
                return tete;
            } else {
                p->suiv = tete;
                return p;
            }
        } else {
            PtrListe l = tete;
            while(l->suiv != NULL && l->suiv->el->el.occurences < val->el.occurences) {
                l = l->suiv;
            }

            if(tete->el->el.occurences > val->el.occurences) {
                p->suiv = tete;
                tete = p;
                return p;
            } else {
                if(l->suiv != NULL) {
                    p->suiv = l->suiv;
                }

                l->suiv = p;
            }

            return tete;
        }
    }
}

/**
 * @brief Créer un arbre binaire contenant aucun fils
 * 
 * @param e Le nombre d'occurences du caractère
 * @param c Le caractère à inserer dans l'abre binaire
 * @return PArbre 
 */
PArbre creerArbreBin(int e, char c) {
    PArbre a;
    if((a = malloc(sizeof(struct arbre))) == NULL) {
        fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
        exit(1);
    }

    a->el.occurences = e;
    a->el.lettre = c;
    a->fg = NULL;
    a->fd = NULL;
    return a;
}

/**
 * @brief Retire le premier élément de la liste et retourne la nouvelle liste
 * Renvoie le premier élément de la liste à travers le paramètre A
 * 
 * @param pile Pointeur vers la liste chainée
 * @param A POINTEUR vers un arbre binaire
 * @return PtrListe 
 */
PtrListe depile(PtrListe pile, PArbre A) {
    PtrListe ptr;
    if (!pile) return NULL;
    ptr = pile;
    *A = *pile->el;
    pile = pile->suiv;  
    // la tete de pile passe au suivant
    free(ptr); // liberation de la tete de pile
    return pile;
}

/**
 * @brief Créer l'abre binaire de Huffman à partir d'une liste d'arbre binaire
 * 
 * @param liste Pointeur vers une liste chainée trié par ordre croissant
 * @return PArbre Pointeur vers l'abre de Huffman
 */
PArbre creerArbreHuffman(PtrListe liste) {
    PtrListe ptr;
    if(liste->suiv != NULL) {
        PArbre FG = malloc(sizeof(struct arbre));
        liste = depile(liste, FG);

        PArbre FD = malloc(sizeof(struct arbre));
        liste = depile(liste, FD);

        PArbre NEW = creerArbreBin(FG->el.occurences + FD->el.occurences, EOF);
        NEW->fg = FG;
        NEW->fd = FD;

        liste = insertList(liste, NEW);

        //AffListe(liste);
        return creerArbreHuffman(liste);
        //return NULL;
    }
    return liste->el;
}

/**
 * @brief Copie un tableau de 256 caractères dans un autre tableau
 * 
 * @param bitArray Tableau d'entrée de 256 char
 * @param newArr Nouveau tableau contenant 256 char
 * @return char* Nouveau tableau contenant 256 char
 */
char* copyArray(char bitArray[256], char newArr[256]) {
    for (int i = 0; i < 256; i++) {
        newArr[i] = bitArray[i];
    }
    return newArr;
}

void parcoursDico(dicoElem dico[256]) {
    for (int i = 0; i < 256; i++) {
        if(dico[i].bitNumber != 0){
            printf("%c", (unsigned char) i);
            printBitArray(dico[i]);
        }
    } 
}

void parcoursArbre(PArbre A) {
    if(A->fg != NULL) {
        printf("0 ");
        parcoursArbre(A->fg);
    } else {
        printf("%c\n", A->el.lettre);
    }

    if(A->fd != NULL) {
        printf("1 ");
        parcoursArbre(A->fd);
    } else {
        printf("%c\n", A->el.lettre);
    }
}

/**
 * @brief Créer un tableau associant à chaque caractère son codage huffman associé
 * 
 * @todo CHANGER LE NOM DE LA FCT
 * @param dico Le tableau
 * @param A L'arbre de huffman
 * @param bitArray 
 * @param h
 */
void createDico(dicoElem dico[256], PArbre A, char bitArray[256], int h) {
    if(A == NULL) return;
    if(A->fd == NULL && A->fg == NULL) {
    //if(A->el.lettre != EOF) {
        for (int i = 0; i < h; i++) {
            dico[A->el.lettre].bitArray[i] = bitArray[i];
        }
    
        dico[A->el.lettre].bitNumber = h;
    }


    char lArr[256]; 
    copyArray(bitArray, lArr);
    char rArr[256]; 
    copyArray(bitArray, rArr);

    lArr[h] = 0;
    rArr[h] = 1;

    createDico(dico, A->fg, lArr, h+1);
    createDico(dico, A->fd, rArr, h+1);
}
