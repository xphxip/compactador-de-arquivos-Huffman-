#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>

// Nó da Árvore de Huffman
typedef struct HuffmanNode {
    unsigned char character;
    unsigned int frequency;
    struct HuffmanNode *left, *right;
} HuffmanNode;

// Min-Heap (Fila de Prioridade Baseada em Array)
typedef struct MinHeap {
    unsigned int size;
    unsigned int capacity;
    HuffmanNode **array;
} MinHeap;

// Funções do nó
HuffmanNode* createNode(unsigned char character, unsigned int frequency);

// Funções da Min-Heap
MinHeap* createMinHeap(unsigned int capacity);
void swapNodes(HuffmanNode **a, HuffmanNode **b);
void minHeapify(MinHeap *minHeap, int idx);
int isSizeOne(MinHeap *minHeap);
HuffmanNode* extractMin(MinHeap *minHeap);
void insertMinHeap(MinHeap *minHeap, HuffmanNode *node);
void buildMinHeap(MinHeap *minHeap);

#endif // STRUCTURES_H
