#include "huffman.h"
#include <string.h>

void calculateFrequencies(const char* filename, unsigned int* frequencies) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo para leitura de frequências: %s\n", filename);
        exit(1);
    }
    
    // Inicializa as frequências
    for (int i = 0; i < 256; i++) {
        frequencies[i] = 0;
    }
    
    int c;
    while ((c = fgetc(file)) != EOF) {
        frequencies[c]++;
    }
    
    fclose(file);
}

// Verifica se é uma folha (não tem filhos left/right)
int isLeaf(HuffmanNode* root) {
    return !(root->left) && !(root->right);
}

HuffmanNode* buildHuffmanTree(unsigned int* frequencies) {
    HuffmanNode *left, *right, *top;

    // Conta quantos caracteres tem freq > 0
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) size++;
    }

    if (size == 0) return NULL; // Arquivo vazio

    MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            minHeap->array[minHeap->size] = createNode((unsigned char)i, frequencies[i]);
            minHeap->size++;
        }
    }

    buildMinHeap(minHeap);

    // Se houver apenas 1 caractere no arquivo, a árvore da raiz não será construída corretamente no loop
    // Tratamos adicionando um nó falso para ele não ser filho direto da raiz sem código
    if (minHeap->size == 1) {
        left = extractMin(minHeap);
        top = createNode('$', left->frequency);
        top->left = left;
        insertMinHeap(minHeap, top);
    }

    // Constrói a árvore até sobrar apenas 1 nó na Heap
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Nó interno, caractere pode ser qualquer um (ex. '$')
        top = createNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // O último nó é a raiz
    HuffmanNode* root = extractMin(minHeap);
    
    // Libera a estrutura minHeap 
    free(minHeap->array);
    free(minHeap);
    
    return root;
}

void generateCodes(HuffmanNode* root, int arr[], int top, char** dictionary) {
    if (!root) return;

    if (root->left) {
        arr[top] = 0;
        generateCodes(root->left, arr, top + 1, dictionary);
    }

    if (root->right) {
        arr[top] = 1;
        generateCodes(root->right, arr, top + 1, dictionary);
    }

    // Se é folha, copia o código acumulado
    if (isLeaf(root)) {
        dictionary[root->character] = (char*)malloc((top + 1) * sizeof(char));
        if (dictionary[root->character]) {
            for (int i = 0; i < top; i++) {
                dictionary[root->character][i] = arr[i] ? '1' : '0';
            }
            dictionary[root->character][top] = '\0';
        }
    }
}

void freeTree(HuffmanNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
