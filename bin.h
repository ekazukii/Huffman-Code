#ifndef BIN_H_INCLUDED
#define BIN_H_INCLUDED

void thisisatest();
void printbincharpad(char c);
void printbinchar(char c);
void encodeWrite(dicoElem dico[256], FILE* fichier, FILE* fo);
void printEncode();
void decodeTEST(PArbre A);
void decodeFromBinFile(PArbre A, FILE* fd);
void createBinFile(FILE* encoded, FILE* binfile);
void printBitArray(dicoElem el);

#endif