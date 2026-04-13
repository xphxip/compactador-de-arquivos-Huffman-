#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "huffman.h"

// Função principal de descompactação
// Recebe o arquivo .huff e gera o arquivo de destino restaurado
void decompressFile(const char* inputFile, const char* outputFile);

#endif // DECOMPRESS_H
