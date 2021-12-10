# Projet compression Huffman en C

Implemente l'algorithme de Huffman pour compresser et decompresser des fichiers de tout types sans pertes

## Compilation

`gcc arbre.c bin.c main.c -o huffman`

## Execution

Compression de hp.txt -> `./huffman -c hp.txt hp.hfzip hp.histo` \n
Decompression de hp.hfzip -> `./huffman -d hp.hfzip hp2.txt hp.histo`\n

Compression de img.jpg -> `./huffman -c img.jpg img.hfzip img.histo` \n
Decompression de img.jpg -> `./huffman -d img.hfzip img2.jpg img.histo`\n
