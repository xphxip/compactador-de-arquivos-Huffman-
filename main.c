#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

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
            char arquivoOriginal[256];
            char arquivoHuff[512];
            
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("(Pasta atual: %s)\n", cwd);
            }
            printf("Digite o nome ou caminho do arquivo para compactar (ex: imagem.png, musica.mp3, video.mp4):\n> ");
            scanf(" %255[^\n]", arquivoOriginal);
            
            // Remover possiveis espaços ou \r ao final (comum no Windows)
            size_t len = strlen(arquivoOriginal);
            while (len > 0 && (arquivoOriginal[len-1] == ' ' || arquivoOriginal[len-1] == '\r' || arquivoOriginal[len-1] == '\t')) {
                arquivoOriginal[--len] = '\0';
            }
            
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            // O arquivo de saida sera o proprio nome do arquivo + ".huff" (nome unico)
            snprintf(arquivoHuff, sizeof(arquivoHuff), "%s.huff", arquivoOriginal);
            
            printf("\nIniciando compactacao do arquivo '%s'...\n", arquivoOriginal);
            compressFile(arquivoOriginal, arquivoHuff);
            printf("Compactacao concluida! Arquivo '%s' gerado automaticamente.\n", arquivoHuff);
            
            // Atendendo ao seu pedido: apagar o arquivo original (mp3/txt/mp4) apos compactar!
            if (remove(arquivoOriginal) == 0) {
                printf(" -> Arquivo original '%s' apagado do disco! Agora so resta o '.huff'.\n", arquivoOriginal);
            } else {
                printf(" -> Aviso: Nao foi possivel apagar o arquivo original '%s'.\n", arquivoOriginal);
            }
        } 
        else if (opcao == 2) {
            char arquivoHuff[256];
            char fileType[32];
            char arquivoRestaurado[512];
            
            printf("\nDigite o nome ou caminho do arquivo .huff para descompactar:\n> ");
            scanf(" %255[^\n]", arquivoHuff);
            
            // Remover possiveis espaços ou \r ao final
            size_t lenH = strlen(arquivoHuff);
            while (lenH > 0 && (arquivoHuff[lenH-1] == ' ' || arquivoHuff[lenH-1] == '\r' || arquivoHuff[lenH-1] == '\t')) {
                arquivoHuff[--lenH] = '\0';
            }
            
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            printf("Qual e o TIPO original desse arquivo? (ex: mp3, png, txt, mp4):\n> ");
            scanf(" %31[^\n]", fileType);
            
            while ((ch = getchar()) != '\n' && ch != EOF);
            
            // Tratar caso o usuario digite com ponto (ex: ".mp3" ao inves de "mp3")
            char* ext = fileType;
            if (ext[0] == '.') ext++;
            
            // Pegar o nome base removendo o ".huff"
            char baseName[256];
            strcpy(baseName, arquivoHuff);
            char* ptrHuff = strstr(baseName, ".huff");
            if (ptrHuff != NULL) {
                *ptrHuff = '\0';
            }
            
            // Remover a extensão do nome base para evitar duplicação
            char* ptrDot = strrchr(baseName, '.');
            if (ptrDot != NULL) {
                *ptrDot = '\0';
            }
            
            // Gerar nome unico final de forma automatizada
            snprintf(arquivoRestaurado, sizeof(arquivoRestaurado), "%s_recuperado.%s", baseName, ext);
            
            printf("\nIniciando descompactacao de '%s' para '%s'...\n", arquivoHuff, arquivoRestaurado);
            decompressFile(arquivoHuff, arquivoRestaurado);
            printf("Descompactacao concluida! O arquivo foi restaurado para '%s'.\n", arquivoRestaurado);
            
            // Apaga o arquivo *.huff do disco apos criar o outro
            if (remove(arquivoHuff) == 0) {
                printf("\n -> Arquivo comprimido '%s' apagado do seu HD com sucesso!\n", arquivoHuff);
            } else {
                printf("\n -> Aviso: Nao foi possivel apagar o arquivo comprimido '%s'.\n", arquivoHuff);
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
