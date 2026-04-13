#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compress.h"
#include "decompress.h"

void showHelp() {
    printf("Uso: compressor.exe -c [arquivo original] [arquivo comprimido .huff]\n");
    printf("     compressor.exe -d [arquivo .huff] [arquivo restaurado]\n");
}

int main(int argc, char* argv[]) {
    // Se o programa for executado sem argumentos (como ao clicar no "Run" do VS Code)
    if (argc == 1) {
        int opcao = 0;
        printf("--- MENU DO COMPACTADOR HUFFMAN ---\n");
        printf("1. Compactar texto.txt para arquivo_secreto.huff\n");
        printf("2. Descompactar arquivo_secreto.huff para texto_recuperado.txt\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao digitando o numero: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            return 1;
        }
        
        if (opcao == 1) {
            printf("\nIniciando compactacao...\n");
            compressFile("c:\\Users\\leand\\Desktop\\compactador de arquivo em C\\texto.txt", "c:\\Users\\leand\\Desktop\\compactador de arquivo em C\\arquivo_secreto.huff");
            printf("Compactacao concluida! Arquivo '.huff' gerado.\n");
        } 
        else if (opcao == 2) {
            printf("\nIniciando descompactacao...\n");
            decompressFile("c:\\Users\\leand\\Desktop\\compactador de arquivo em C\\arquivo_secreto.huff", "c:\\Users\\leand\\Desktop\\compactador de arquivo em C\\texto_recuperado.txt");
            printf("Descompactacao concluida! O arquivo foi restaurado.\n");
            
            // Apaga o arquivo *.huff do disco apos criar o txt
            if (remove("c:\\Users\\leand\\Desktop\\compactador de arquivo em C\\arquivo_secreto.huff") == 0) {
                printf("\n -> Arquivo comprimido 'arquivo_secreto.huff' foi apagado do seu HD com sucesso!\n");
            } else {
                printf("\n -> Nao foi possivel apagar o arquivo '.huff'.\n");
            }
        } 
        else {
            printf("Saindo...\n");
        }
        
        return 0;
    }

    if (argc != 4) {
        showHelp();
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        printf("Iniciando compactacao...\n");
        compressFile(argv[2], argv[3]);
        printf("Compactacao concluida com sucesso em '%s'.\n", argv[3]);
    } 
    else if (strcmp(argv[1], "-d") == 0) {
        printf("Iniciando descompactacao...\n");
        decompressFile(argv[2], argv[3]);
        printf("Descompactacao concluida com sucesso em '%s'.\n", argv[3]);
    } 
    else {
        printf("Opcao invalida: %s\n", argv[1]);
        showHelp();
        return 1;
    }

    return 0;
}
