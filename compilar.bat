@echo off
setlocal

REM Script gerado para ajudar a usar o compilador nativo ou buscar o do Code::Blocks automaticamente
echo =======================================================
echo          COMPILADOR AJUDANTE (VS CODE / CMD)
echo =======================================================

REM Tenta adicionar os caminhos mais comuns do Code::Blocks temporariamente no Terminal se não estiver achando o gcc
set "PATH=C:\Program Files\CodeBlocks\MinGW\bin;C:\Program Files (x86)\CodeBlocks\MinGW\bin;C:\MinGW\bin;%PATH%"

echo Verificando compilador...
gcc --version >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
    echo [ERRO] O compilador GCC nao foi encontrado no seu computador!
    echo Certifique-se de que voce instalou o MinGW ou que o Code::Blocks esta dentro de localizacoes padroes.
    exit /b 1
)

echo [OK] GCC Encontrado. Compilando o arquivo executavel 'compressor.exe'...
gcc main.c compress.c decompress.c huffman.c structures.c -O2 -Wall -o compressor.exe

IF %ERRORLEVEL% EQU 0 (
    echo [SUCESSO] Compilacao concluida!
    echo Para abrir o Menu pelo terminal, apenas digite:
    echo .\compressor.exe
    echo.
) ELSE (
    echo [ERRO] Ocorreu uma falha ao tentar compilar os arquivos C!!
)
endlocal
