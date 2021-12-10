# Projet compression Huffman en C

## Compilation

`gcc arbre.c bin.c main.c -o huffman`

## Execution

Compression de lorem.txt -> `./huffman -c lorem.txt lorem.hfzip lorem.histo` \n
Decompression de lorem.hfzip -> `./huffman -d lorem.hfzip lorem2.txt lorem.histo`
