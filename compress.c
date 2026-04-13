#include "compress.h"
#include <string.h>

void compressFile(const char* inputFile, const char* outputFile) {
    unsigned int frequencies[256] = {0};

    // Passo 1: Calcular frequências
    calculateFrequencies(inputFile, frequencies);

    // Passo 2: Construir árvore
    HuffmanNode* root = buildHuffmanTree(frequencies);
    if (!root) {
        printf("Arquivo de entrada vazio ou erro na leitura.\n");
        return;
    }

    // Passo 3: Gerar dicionário (códigos)
    char* dictionary[256] = {NULL};
    int arr[256];
    generateCodes(root, arr, 0, dictionary);

    // Passo 4: Abrir arquivos
    FILE* in = fopen(inputFile, "rb");
    FILE* out = fopen(outputFile, "wb");

    if (!in || !out) {
        fprintf(stderr, "Erro ao abrir arquivos de entrada/saida.\n");
        exit(1);
    }

    // Escreve a tabela de frequência no cabeçalho
    fwrite(frequencies, sizeof(unsigned int), 256, out);

    // Lógica de empacotamento
    unsigned char buffer = 0;
    int bitCount = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        char* code = dictionary[c];

        // Empacota bit a bit
        for (int i = 0; code[i] != '\0'; i++) {
            // Desloca o buffer para esquerda e insere 1 se for '1'
            buffer = (buffer << 1) | (code[i] == '1' ? 1 : 0);
            bitCount++;

            // Se encheu o byte (8 bits), escreve no arquivo
            if (bitCount == 8) {
                fputc(buffer, out);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Escreve os bits restantes preenchendo com zeros à direita
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        fputc(buffer, out);
    }

    fclose(in);
    fclose(out);

    // Liberar memória gerada
    for (int i = 0; i < 256; i++) {
        if (dictionary[i]) free(dictionary[i]);
    }
    freeTree(root);
}
