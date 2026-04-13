#ifndef COMPRESS_H
#define COMPRESS_H

#include "huffman.h"

// Função principal de compactação
// Recebe o arquivo original e o nome do arquivo de destino
void compressFile(const char* inputFile, const char* outputFile);

#endif // COMPRESS_H
