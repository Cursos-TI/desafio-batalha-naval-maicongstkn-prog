#include <stdio.h>
#include <stdlib.h>

#define TAM 10

// Mostrar o tabuleiro
void mostrarTabuleiro(char tab[TAM][TAM], int oculto) {
    printf("\n   ");
    for (int i = 0; i < TAM; i++) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            if (oculto && tab[i][j] == 'N') printf("~ ");
            else printf("%c ", tab[i][j]);
        }
        printf("\n");
    }
}

// Inicializar tabuleiro
void inicializarTabuleiro(char tab[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tab[i][j] = '~';
}

// Colocar navios simples
void colocarNavios(char tab[TAM][TAM]) {
    // Horizontal
    for (int i = 2; i < 5; i++) tab[1][i] = 'N';
    // Vertical
    for (int i = 3; i < 6; i++) tab[i][7] = 'N';
    // Diagonal 1
    for (int i = 0; i < 3; i++) tab[6 + i][i] = 'N';
    // Diagonal 2
    for (int i = 0; i < 3; i++) tab[8 - i][5 + i] = 'N';
}

// Contar navios restantes
int naviosRestantes(char tab[TAM][TAM]) {
    int cont = 0;
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if (tab[i][j] == 'N') cont++;
    return cont;
}

// Ataque simples
void atacar(char tab[TAM][TAM], int linha, int coluna) {
    if (tab[linha][coluna] == 'N') {
        printf(">>> ACERTOU um navio!\n");
        tab[linha][coluna] = 'X';
    } else if (tab[linha][coluna] == '~') {
        printf(">>> Água.\n");
        tab[linha][coluna] = 'O';
    } else {
        printf(">>> Essa posição já foi atacada!\n");
    }
}

// Habilidade Cruz
void habilidadeCruz(char tab[TAM][TAM], int linha, int coluna) {
    for (int i = 0; i < TAM; i++) {
        if (i != coluna) atacar(tab, linha, i);
    }
    for (int i = 0; i < TAM; i++) atacar(tab, i, coluna);
}

// Habilidade Octaedro (X)
void habilidadeOctaedro(char tab[TAM][TAM], int linha, int coluna) {
    int dx[4] = {-1, -1, 1, 1};
    int dy[4] = {-1, 1, -1, 1};
    atacar(tab, linha, coluna);
    for (int i = 0; i < 4; i++) {
        int nx = linha + dx[i];
        int ny = coluna + dy[i];
        if (nx >= 0 && nx < TAM && ny >= 0 && ny < TAM)
            atacar(tab, nx, ny);
    }
}

// Habilidade Cone (3 posições abaixo)
void habilidadeCone(char tab[TAM][TAM], int linha, int coluna) {
    for (int i = 0; i < 3; i++) {
        int nx = linha + i;
        if (nx >= 0 && nx < TAM)
            atacar(tab, nx, coluna);
    }
}

int main() {
    int round = 1;

    while (1) {  // loop infinito para reiniciar rodadas
        char tab1[TAM][TAM], tab2[TAM][TAM];
        inicializarTabuleiro(tab1);
        inicializarTabuleiro(tab2);
        colocarNavios(tab1);
        colocarNavios(tab2);

        int jogador = 1;
        int linha, coluna, opcao;

        printf("\n=== BATALHA NAVAL – NIVEL MESTRE ===\n");
        printf("Rodada %d\n", round);
        printf("Dois jogadores, quatro navios cada.\n");

        while (naviosRestantes(tab1) > 0 && naviosRestantes(tab2) > 0) {
            char (*tabAlvo)[TAM] = (jogador == 1 ? tab2 : tab1);
            printf("\n--- Vez do Jogador %d ---\n", jogador);
            mostrarTabuleiro(tabAlvo, 1);

            printf("Escolha o tipo de ataque:\n");
            printf("1 - Ataque normal\n");
            printf("2 - Habilidade Cruz\n");
            printf("3 - Habilidade Octaedro\n");
            printf("4 - Habilidade Cone\n");
            printf("Opcao: ");
            if (scanf("%d", &opcao) != 1) {
                printf("Entrada invalida!\n");
                return 1;
            }

            printf("Escolha a posicao (linha coluna): ");
            if (scanf("%d %d", &linha, &coluna) != 2) {
                printf("Entrada invalida!\n");
                return 1;
            }

            if (linha < 0 || linha >= TAM || coluna < 0 || coluna >= TAM) {
                printf("Posicao fora do tabuleiro!\n");
                continue;
            }

            switch (opcao) {
                case 1: atacar(tabAlvo, linha, coluna); break;
                case 2: habilidadeCruz(tabAlvo, linha, coluna); break;
                case 3: habilidadeOctaedro(tabAlvo, linha, coluna); break;
                case 4: habilidadeCone(tabAlvo, linha, coluna); break;
                default: printf("Opcao invalida!\n"); continue;
            }

            jogador = (jogador == 1 ? 2 : 1);
        }

        if (naviosRestantes(tab1) == 0)
            printf("\n>>> Jogador 2 venceu a rodada %d!\n", round);
        else
            printf("\n>>> Jogador 1 venceu a rodada %d!\n", round);

        round++;  // próxima rodada
        printf("\n=== Nova partida iniciando... ===\n");
    }

    return 0;
}
