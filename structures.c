#include "structures.h"

// Criação de um novo nó da árvore de Huffman
HuffmanNode* createNode(unsigned char character, unsigned int frequency) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (!node) {
        fprintf(stderr, "Erro de alocação de memória para o nó.\n");
        exit(1);
    }
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Criação da Fila de Prioridade Min-Heap
MinHeap* createMinHeap(unsigned int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!minHeap) {
        fprintf(stderr, "Erro de alocação de memória para a Min-Heap.\n");
        exit(1);
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)malloc(minHeap->capacity * sizeof(HuffmanNode*));
    if (!minHeap->array) {
        fprintf(stderr, "Erro de alocação de memória para o array da Min-Heap.\n");
        exit(1);
    }
    return minHeap;
}

// Troca dois nós da fila
void swapNodes(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Mantém a propriedade de min-heap
void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Verifica se o tamanho do Heap é 1
int isSizeOne(MinHeap *minHeap) {
    return (minHeap->size == 1);
}

// Extrai o nó com menor frequência da Min-Heap
HuffmanNode* extractMin(MinHeap *minHeap) {
    if (minHeap->size == 0) return NULL;
    
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    
    minHeapify(minHeap, 0);
    return temp;
}

// Insere um novo nó na Min-Heap
void insertMinHeap(MinHeap *minHeap, HuffmanNode *node) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    // Sobe o elemento na árvore até sua posição correta
    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// Constrói a MinHeap
void buildMinHeap(MinHeap *minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
