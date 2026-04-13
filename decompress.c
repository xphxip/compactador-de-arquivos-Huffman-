#include "decompress.h"

void decompressFile(const char* inputFile, const char* outputFile) {
    FILE* in = fopen(inputFile, "rb");
    if (!in) {
        fprintf(stderr, "Erro ao abrir o arquivo compactado: %s\n", inputFile);
        exit(1);
    }
    
    unsigned int frequencies[256];
    
    // Lê o cabeçalho (array de frequências)
    size_t readElements = fread(frequencies, sizeof(unsigned int), 256, in);
    if (readElements != 256) {
        fprintf(stderr, "Erro ao ler o cabeçalho. Arquivo corrompido ou formato inválido.\n");
        fclose(in);
        exit(1);
    }
    
    // Total de caracteres originais (ajuda a ignorar os "zeros de padding" no final do arquivo)
    long long totalChars = 0;
    for (int i = 0; i < 256; i++) {
        totalChars += frequencies[i];
    }
    
    // Constrói a árvore de Huffman idêntica à que foi usada na compactação
    HuffmanNode* root = buildHuffmanTree(frequencies);
    if (!root) {
        printf("Árvore de Huffman não pôde ser construída. Arquivo vazio?\n");
        fclose(in);
        return;
    }
    
    FILE* out = fopen(outputFile, "wb");
    if (!out) {
        fprintf(stderr, "Erro ao criar arquivo de saída.\n");
        fclose(in);
        freeTree(root);
        exit(1);
    }
    
    HuffmanNode* current = root;
    int c;
    long long decodedChars = 0;
    
    // Lê byte a byte do arquivo compactado (ignora cabeçalho)
    while (decodedChars < totalChars && (c = fgetc(in)) != EOF) {
        // Extrai cada bit do byte lido (da esquerda para a direita)
        for (int i = 7; i >= 0 && decodedChars < totalChars; i--) {
            int bit = (c >> i) & 1;
            
            // Navegação na Árvore
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            
            // Verifica se chegou em uma folha
            if (current->left == NULL && current->right == NULL) {
                fputc(current->character, out);
                decodedChars++;
                current = root; // Reinicia a navegação para o próximo caractere
            }
        }
    }
    
    fclose(in);
    fclose(out);
    freeTree(root);
}
