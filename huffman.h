#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "structures.h"

// Calcula as frequências de cada byte em um arquivo e armazena em 'frequencies'
// 'frequencies' deve ser um array de tamanho 256.
void calculateFrequencies(const char* filename, unsigned int* frequencies);

// Constrói a Árvore de Huffman usando um array de frequências.
// Retorna a raiz da árvore construída.
HuffmanNode* buildHuffmanTree(unsigned int* frequencies);

// Percorre a árvore e gera o código para cada caractere.
// 'dictionary' é um array de 256 ponteiros para strings alocadas.
void generateCodes(HuffmanNode* root, int arr[], int top, char** dictionary);

// Libera a memória alocada para os nós da Árvore
void freeTree(HuffmanNode* root);

#endif // HUFFMAN_H
