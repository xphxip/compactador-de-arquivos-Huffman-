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
        printf("1. Compactar um arquivo\n");
        printf("2. Descompactar um arquivo\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao digitando o numero: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            return 1;
        }
        
        // Limpar o buffer do teclado para ler strings depois
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        
        if (opcao == 1) {
            char inputFile[256];
            char outputFile[256] = "arquivo.huff"; // padrão
            
            printf("\nDigite o nome ou caminho do arquivo para compactar (ex: imagem.png, video.mp4, texto.txt):\n> ");
            scanf(" %255[^\n]", inputFile);
            
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            printf("Digite o nome do arquivo de saida (ou pressione ENTER para usar o padrao 'arquivo.huff'):\n> ");
            char temp[256] = "";
            fgets(temp, sizeof(temp), stdin);
            if (temp[0] != '\n' && temp[0] != '\0') {
                temp[strcspn(temp, "\n")] = '\0';
                strcpy(outputFile, temp);
            }
            
            printf("\nIniciando compactacao do arquivo '%s'...\n", inputFile);
            compressFile(inputFile, outputFile);
            printf("Compactacao concluida! Arquivo '%s' gerado.\n", outputFile);
        } 
        else if (opcao == 2) {
            char inputFile[256];
            char outputFile[256];
            
            printf("\nDigite o nome ou caminho do arquivo .huff para descompactar:\n> ");
            scanf(" %255[^\n]", inputFile);
            
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            printf("Digite o nome que o arquivo tera ao ser restaurado (ex: imagem_recuperada.png):\n> ");
            scanf(" %255[^\n]", outputFile);
            
            printf("\nIniciando descompactacao de '%s' para '%s'...\n", inputFile, outputFile);
            decompressFile(inputFile, outputFile);
            printf("Descompactacao concluida! O arquivo foi restaurado para '%s'.\n", outputFile);
            
            // Apaga o arquivo *.huff do disco apos criar o outro
            if (remove(inputFile) == 0) {
                printf("\n -> Arquivo comprimido '%s' foi apagado do seu HD com sucesso!\n", inputFile);
            } else {
                printf("\n -> Nao foi possivel apagar o arquivo '%s'.\n", inputFile);
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
