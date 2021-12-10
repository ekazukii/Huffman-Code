#ifndef ARBRE_H_INCLUDED
#define ARBRE_H_INCLUDED

PtrListe creerNoeud(PArbre x,  PtrListe suiv);
PtrListe insertList(PtrListe tete, PArbre val);
PtrListe Empile(PtrListe pile, PArbre x);
PArbre creerArbreBin(int e, char c);
PtrListe depile(PtrListe pile, PArbre A);
void AffListe(PtrListe liste);
PArbre setFG(PArbre arbre, int fils, char c);
PArbre setFD(PArbre arbre, int fils, char c);
PArbre ajouterFils(PArbre arbre, int fg, char fgc, int fd, char fdc);
PArbre creerArbreHuffman(PtrListe liste);
char* copyArray(char bitArray[8], char newArr[8]);
void createDico(dicoElem dico[256], PArbre A, char bitArray[8], int h);
void parcoursDico(dicoElem dico[256]);
void parcoursArbre(PArbre A);

#endif