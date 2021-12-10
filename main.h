#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

struct element {
    unsigned char lettre;
    int occurences;
};

struct arbre {
    struct element el;
    struct arbre *fg;
    struct arbre *fd;
};
typedef struct arbre* PArbre;

typedef struct lst {
    PArbre el;
    struct lst* suiv;
} Liste;

typedef Liste* PtrListe;

typedef struct dicoElem {
    char bitNumber;
    char bitArray[256];
} dicoElem;


void creerHisto(FILE* fp);
int main();

#endif