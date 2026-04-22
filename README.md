# Compactador e Descompactador de Arquivos (Algoritmo de Huffman)

Este projeto é uma implementação em C de um compactador e descompactador de arquivos utilizando o **Algoritmo de Huffman**. Seu principal objetivo é comprimir arquivos de texto de forma a economizar espaço no disco rígido sem perder nenhuma informação no processo (Lossless Compression). A compressão é feita através de manipulação em nível de **bits**, convertendo os caracteres normais em códigos binários menores com base na sua frequência.

Neste documento, você vai encontrar a explicação de **cada arquivo, estrutura e função** envolvida nos códigos deste projeto. Nada passará em branco!

---

## 🚀 Como Executar o Projeto

Antes de entender as engrenagens, o código pode ser compilado e executado de duas formas principais, graças ao sistema presente no arquivo `main.c`.

1. **Via Makefile (Sugerido)**
   - No terminal, digite `make` para compilar todo o código e gerar um executável multiplataforma chamado `compressor.exe` (no Windows) ou `compressor` (no Linux/macOS).
   - Você pode limpar os executáveis a qualquer momento utilizando `make clean` em qualquer um dos dois sistemas.

2. **Interface pelo VS Code / Modo Interativo**
   - Caso você apenas rode o código principal no seu editor ou sem nenhum argumento extra no terminal, ele abrirá um **Menu Interativo**.
   - No menu, digite `1` para compactar o arquivo padrão `texto.txt` para `arquivo_secreto.huff`.
   - Digite `2` para descompactar o arquivo `.huff` de volta para um formato legível (`texto_recuperado.txt`). *(Obs: Após recuperar, ele deleta o .huff por padrão para economizar espaço e limpar rastros!)*

3. **Modo Terminal (Linha de Comando Exata)**
   - O projeto suporta compactação de quaisquer arquivos via terminal se usar as flags de argumento *(Lembre-se de omitir o `.exe` caso esteja utilizando Linux ou macOS)*:
   - Para compactar: `./compressor.exe -c [arquivo original] [arquivo .huff de saida]`
   - Para descompactar: `./compressor.exe -d [arquivo .huff de entrada] [arquivo txt restaurado]`

---

## 🧩 Dissecando a Estrutura (Arquivo por Arquivo)

Abaixo está o guia detalhado e exato sobre como as estruturas se comunicam. Tudo foi separado de modo a seguir o princípio de "responsabilidade única".

### 1. `structures.h` e `structures.c` (A Fundação de Dados)
Estes arquivos preparam o terreno definindo os "Tipos de Dados" que serão injetados e manipulados não só para armazenar as letras, mas criar a estrutura organizacional da Árvore e da Fila (MinHeap).

**O que cada coisa faz:**
- `struct HuffmanNode`: É o "bloco" da nossa Árvore. Ele contém:
  - `character`: O próprio caractere que está salvando (Ex: 'A').
  - `frequency`: Quantas vezes esse caractere apareceu no texto.
  - `left` e `right`: Ponteiros direcionando para quem está em seus ramos (Filho da Esquerda e Filho da Direita).
- `struct MinHeap`: É a Fila de Prioridade em formato de "array". Seu objetivo é estar sempre organizada evidenciando na primeira posição (`array[0]`) o nó com a **menor frequência** existente.
- `createNode()`: Apenas uma função facilitadora (Construtor) que aloca memória do PC (`malloc`) para dar vida e retornar um novo nó individual do Huffman com sua letra e sua contagem.
- `createMinHeap()`: Instância a Fila na memória definindo seu tamanho (teto de capacidade máxima possível).
- `swapNodes()`: Função ajudante simples. Dado dois nós `A` e `B`, ele inverte os nós. Auxilia na organização da Fila.
- `minHeapify()`: O coração que mantém a Fila de Prioridades operando. Se um nó for colocado em um lugar errado, ela joga esse nó para as profundezas e traz os menores valores pro topo usando métodos de comparação matemáticas (`2*idx+1` e `+2` acessam galhos descendentes virtualmente).
- `isSizeOne()`: Verifica rapidamente se a Fila chegou ao seu tamanho igual a 1 (Significando que a nossa Grande Árvore de Huffman acabou de ser construída inteiramente na Fila!).
- `extractMin()`: Como num guichê de banco. Retira o "primeiro" item do topo da Fila de Prioridade (que tem a menor frequência) e logo invoca o `minHeapify` para a fila rapidamente ser repuxada e colocar o segundo menor item no topo!
- `insertMinHeap()`: O oposto da função acima. Entra um novo nó avulso na última casa da Fila, e então um `while` pega esse nó e vai galgando ele pro topo até que ele pare sob um nó menor que ele próprio.
- `buildMinHeap()`: Prepara uma primeira bateria "forçada" chamando exaustivamente o `minHeapify` desde os internos profundos até o topo da árvore crua em questão, a fim de garantir que a ordem global inicial seja 100% perfeita.

---

### 2. `huffman.h` e `huffman.c` (O Motor Matemático de Huffman)
Essa é a verdadeira alma algorítmica deste projeto. Ele se vale das peças moldadas nas `structures` para de fato criar as regras da Compressão!

**O que cada coisa faz:**
- `calculateFrequencies()`: Lê o arquivo original no seu formato físico com "rb" (Read Binary) e usa cada byte lido como índice de zero a 255. Ou seja, se ler uma letar "B" (código 66) ele soma +1 no espaço 66 da array (`frequencies[c]++`). Isso criará a contagem precisa de todas as letras do texto!
- `isLeaf()`: Testa rapidamente se um Nó é uma das pontas extremas (As Folhas) da Árvore de Huffman, ou seja: Não ter absolutamente nenhum filho da esquerda nem da direita (`!(root->left) && !(root->right)`).
- `buildHuffmanTree()`: Aqui operamos o algoritmo que cria a árvore final! Ele:
  1. Cria nós para todos os caracteres e injeta tudo na `MinHeap`. Reorganiza tudo com `buildMinHeap()`.
  2. Inicia o loop "enquanto o tamanho não for 1".
  3. Extraí (`extractMin()`) os DOIS nós com os menores números.
  4. Amarra os dois nós a um "Nó Pai Falso" que ganha o símbolo fictício `$` agindo como conector somando a frequência de ambos!
  5. Insere de novo o nó Falso gigante na fila. Repete o loop unindo ramos até sobrar 1 Grande e Massivo Nó (A raiz central conectando todos os menores!).
- `generateCodes()`: É o Desbravador! Ele desce da Raiz Suprema até varrer todas as pontas da Árvore simultaneamente de forma recursiva. Sempre que desce pela Esquerda salva o símbolo `"0"`, ao descer pela Direita salva `"1"`. Bateu numa "Folha" (Letra sem filhos)? Copie a sequência de rotas de `0`s e `1`s e guarde no `dictionary[]` no espaço exato pertencente a aquela letra alcançada. Assim nasceu a língua de sinais privada dessa sua Árvore de Huffman.
- `freeTree()`: O Lixeiro da Máquina. Varre todo as conexões ativas na Memória RAM criadas para a Árvore de Huffman e deleta recursivamente todos os vestígios dela via o comando `free(root)`.

---

### 3. `compress.h` e `compress.c` (A Máquina de Escrever BITS)
Aqui é onde toda as ideias teóricas viram código sujo criando o arquivo minimizado de saída (`.huff`).

**O que cada coisa faz:**
- `compressFile()`: Controlador principal da etapa de compressão em cascata:
  1. Invoca os `calculateFrequencies()`, `buildHuffmanTree()`, e o desbravador `generateCodes()`, criando um array de Códigos Binários String (`dictionary`).
  2. **`fwrite(frequencies, ... out)`**: O salva-vidas! Escreve literalmente o quadro inteiro (Tabela) bruto de frequências originais no **início** do arquivo de saída `.huff`. Sem isto seria biologicamente impossível descompactar este arquivo no futuro depois.
  3. **O Operador de BITS `(buffer << 1)`**: Você não pode salvar dados bit a bit em um HD normalmente (A menor escala gravável do computador é de 8 bits "1 byte"). Esta aba reinicia a varrer o arquivo texto lendo letra a letra de novo: ele converte as letras do seu texto original buscando a string de sinais no dicionario gerado. Usando matemática física em C, empurra o caractere `'1'` e `'0'` usando Deslocamento de Bits "Shift `< < 1`" junto com Bitwise OR "`|`" em um único Char vazio (O Buffer).
  4. Quando o `bitCount` diz que o contador bateu a capacidade de `8`, isso significa que agrupamos um Byte limpo! Ele finalmente joga esse Byte enxuto no arquivo via `fputc(buffer, out)` e reinicia do zero pra compor os próximos OITOS BITS.
  5. **Padding Fill (`<< 8 - bitCount`)**: E se sobrar apenas um caractere cuja tradução custou só "3 BITS"? Não dá pra salvar fragmentos no computador... A solução: Ele preenche o lado do espaço vazio dos últimos 5 Bits restantes arrastando tudo com 0s zerados inofensivos só pra encher o caminhãozinho padrão de '1 byte' da máquina garantindo envio seguro desses finalmentes do texto sem causar corrompimentos. Limpa e remove todas as alocações da memória após isso.

---

### 4. `decompress.h` e `decompress.c` (A Leitura e Reconstrução)
Dado um Arquivo com dezenas de Bits empacotados num `.huff`, essa biblioteca faz o "Processo Matemático Reverso" para trazer o texto dos mortos.

**O que cada coisa faz:**
- `decompressFile()`: O Maestro da Restauramento de Arquivos em 4 Passos:
  1. **`fread(frequencies, ... in)`**: Logo ao abrir o arquivo compactado ele suga todo o Cabeçalho Inicial e readquire imediatamente TODA A TABELA ORIGINAL de frequências daquelas palavras daquela época.
  2. Em posse das frequências antigas, ele ressucita a árvore chamando `buildHuffmanTree()`. O algoritmo do PC não sabe que são velhos dados, mas a Matemática perfeita assegura que exatamente os mesmíssimos ramos dos anos anteriores se conectem milimetricamente como antes.
  3. **A Salvação ("Padding Handler" do `totalChars`)**: Para ele saber quando DEVE parar de ler o documento empacotado ignorando o "Lixo Extra do Caminhão" do tópico passado supracitado (O Padding de "0s"), ele soma todas as frequências originais do passo anterior num total (`totalChars`).
  4. **Navegador Bit-Level `((c >> i) & 1)`**: Aqui está a Descompactação Máster. Ele chupa os milhares de Bytes do `.huff` e de frente pra trás destila bit à bit forçando o descolamento pra direita (Bitwise `>>`). Achou o Bit **0**? Clica na Seta para **Esquerda** (`current->left`). O próximo Bit é **1**? Desça para a seta a **Direita** (`current->right`). Repita esta navegação cega até esbarrar numa parede sem saídas laterais (Uma "Folha/Nó da arvore final"). Lá estará guardado exatamente o caractere original.
  5. Escreva-o (`fputc`), aumente um de contagem, volte à base inicial (Raiz Suprema), e desça as setas baseadas nos próximos bits até encontrar o total estrito limitador gerado pelo `totalChars`! FIM. Seu texto re-nasce magicamente dos mortos!

---

### 5. `main.c` e `Makefile` (A Interface Controladora)
- `main.c`: A "Sala do Servidor" do seu compactador. Ele cuida de fazer a interface que junta todos os passos em torno da decisão do Usuário, impedindo que o código rode erroneamente sozinho. Possui a checagem manual de `argc` para perceber não só caso você executou por via de um botão play, como também gerou um sistema `printf/scanf` interativo para lidar com as remoções em file system C (apagar o `.huff` da máquina via `remove()` quando é restaurado para ficar de acordo em manter o ambiente clean e poupar o disco do Usuário).
- `Makefile`: A Receita de Bolo do Compilador nativo (`gcc`). Instruções declarativas dizendo que "o arquivo MAIN precisa das cabeças de todos os .H e seus C's". Assim, a automação sabe conectar todo esse labirinto numa única linha de comando sem precisos mil comandos.
