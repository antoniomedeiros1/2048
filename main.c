#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {

    int valor; // multiplos de 2
    int x; //inteiro entre 0 e 3, indicando a coluna
    int y; //inteiro entre 0 e 3, indicando a linha
    int fundiu;

} Bloco;

char minusc(char c){

    if (c >= 'A' && c <= 'Z'){
        c = ('A' - c) + 'a';
    }
    return c;
}

void atualiza(int grade[4][4], Bloco blocos[16]){

    int i, j;

    for (i = 0; i < 16; i++){
        if (blocos[i].valor != 0){
            grade[blocos[i].y][blocos[i].x] = blocos[i].valor;
        }
    }

}

void display(int grade[4][4]){
    //Imprime no terminal a 'tela' do jogo
    int i, j;
    printf("\n");
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            switch (grade[i][j]){

                case    2: printf("\033[0;31m"); break;
                case    4: printf("\033[0;32m"); break;
                case    8: printf("\033[0;33m"); break;
                case   16: printf("\033[0;34m"); break;
                case   32: printf("\033[1;34m"); break;
                case   64: printf("\033[0;35m"); break;
                case  128: printf("\033[1;35m"); break;
                case  256: printf("\033[0;36m"); break;
                case  512: printf("\033[1;36m"); break;
                case 1024: printf("\033[1;31m"); break;
                case 2048: printf("\033[1;32m"); break;

            }
            printf("%4d", grade[i][j]);
            printf("\033[0m");
        }

        printf("\n\n");
    }
    //printf("w - cima || s - baixo || a - esquerda || d - direita\n\n");
}

int conta_blocos(Bloco blocos[16]){
    //conta quantos blocos do vetor tem valor diferente de 0
    int i, cont = 0;

    for (i = 0; i < 16; i++){
        if (blocos[i].valor != 0){
            cont++;
        }
    }
    return cont;
}

void zera(Bloco blocos[16], int x, int y){
    //essa funcao transforma o valor do bloco na linha y coluna x em 0
    //eh usada quando dois blocos de mesmo valor se juntam
    int i;

    for (i = 0; i < 16; i++){
        if(blocos[i].x == x && blocos[i].y == y){
            //printf("(%d, %d, %d)", blocos[i].x, blocos[i].y, blocos[i].valor);
            blocos[i].x = 0;
            blocos[i].y = 0;
            blocos[i].valor = 0;
            //printf("-> (%d, %d, %d)\n", blocos[i].x, blocos[i].y, blocos[i].valor);
            break;
        }
    }
}

int ja_fundiu(Bloco blocos[16], int x, int y){
    //essa funcao retorna o int fundiu do bloco na linha y coluna x
    //eh usada quando dois blocos de mesmo valor se juntam, para saber se o outro bloco ja fundiu na mesma rodada
    int i;

    for (i = 0; i < 16; i++){
        if(blocos[i].x == x && blocos[i].y == y){
            if (blocos[i].valor == 0){
                return 0;
            }
            return (blocos[i].fundiu);
        }
    }
    return 0;
}

int pode_mover(int grade[4][4], Bloco bloco, char dir){
    //verifica se o bloco pode mover na direção indicada pelo jogador
    int b = 1;

    if (bloco.valor == 0){
        return 0;
    }

    switch (dir){

        case 'w':
            if (bloco.y == 0 || (grade[bloco.y - 1][bloco.x] != 0 && grade[bloco.y - 1][bloco.x] != bloco.valor ) ||
            (grade[bloco.y - 1][bloco.x] == bloco.valor && bloco.fundiu)){
                b = 0;
            }
            break;

        case 's':
            if (bloco.y == 3 || (grade[bloco.y + 1][bloco.x] != 0 && grade[bloco.y + 1][bloco.x] != bloco.valor) ||
            (grade[bloco.y + 1][bloco.x] == bloco.valor && bloco.fundiu)){
                b = 0;
            }
            break;

        case 'a':
            if (bloco.x == 0 || (grade[bloco.y][bloco.x - 1] != 0 && grade[bloco.y][bloco.x - 1] != bloco.valor) ||
            (grade[bloco.y][bloco.x - 1] == bloco.valor && bloco.fundiu)){
                b = 0;
            }
            break;

        case 'd':
            if (bloco.x == 3 || (grade[bloco.y][bloco.x + 1] != 0 && grade[bloco.y][bloco.x + 1] != bloco.valor) ||
            (grade[bloco.y][bloco.x - 1] == bloco.valor && bloco.fundiu)){
                b = 0;
            }
            break;
        }
        return b;
}

void move(int grade[4][4], Bloco bloco[16], char dir, int i){

    if (pode_mover(grade, bloco[i], dir)){

        switch (dir){

            case 'w':
                if (grade[bloco[i].y - 1][bloco[i].x] == bloco[i]. valor){
                    if (bloco[i].fundiu || ja_fundiu(bloco, bloco[i].x, bloco[i].y - 1)){
                        bloco[i].fundiu = 1;
                        break;
                    }
                    bloco[i].valor *= 2;
                    zera(bloco, bloco[i].x, bloco[i].y - 1);
                    bloco[i].fundiu = 1;
                }
                grade[bloco[i].y][bloco[i].x] = 0;
                bloco[i].y--;
                break;

            case 's':
                if (grade[bloco[i].y + 1][bloco[i].x] == bloco[i]. valor){
                    if (bloco[i].fundiu || ja_fundiu(bloco, bloco[i].x, bloco[i].y + 1)){
                        bloco[i].fundiu = 1;
                        break;
                    }
                    bloco[i].valor *= 2;
                    zera(bloco, bloco[i].x, bloco[i].y + 1);
                    bloco[i].fundiu = 1;
                }
                grade[bloco[i].y][bloco[i].x] = 0;
                bloco[i].y++;
                break;

            case 'a':
                if (grade[bloco[i].y][bloco[i].x - 1] == bloco[i]. valor){
                    if (bloco[i].fundiu || ja_fundiu(bloco, bloco[i].x - 1, bloco[i].y)){
                        bloco[i].fundiu = 1;
                        break;
                    }
                    bloco[i].valor *= 2;
                    zera(bloco, bloco[i].x - 1, bloco[i].y);
                    bloco[i].fundiu = 1;
                }
                grade[bloco[i].y][bloco[i].x] = 0;
                bloco[i].x--;
                break;

            case 'd':
                if (grade[bloco[i].y][bloco[i].x + 1] == bloco[i]. valor){
                    if (bloco[i].fundiu || ja_fundiu(bloco, bloco[i].x + 1, bloco[i].y)){
                        bloco[i].fundiu = 1;
                        break;
                    }
                    bloco[i].valor *= 2;
                    zera(bloco, bloco[i].x + 1, bloco[i].y);
                    bloco[i].fundiu = 1;
                }
                grade[bloco[i].y][bloco[i].x] = 0;
                bloco[i].x++;
                break;
            }
    }
}

void add_bloco(int grade[4][4], Bloco blocos[16], int n){
    //adiciona mais um bloco
    //n eh a quantidade de blocos no vetor blocos[16]
    int x, y, i, valor;

    if (n < 16){

        x = rand() % 4; //valor pode ser entre 0 e 3
        y = rand() % 4;
        valor = ((rand() % 2)*2) + 2; //(rand() % 2) -> 0 ou 1, *2 -> 0 ou 2, + 2 -> 2 ou 4

        //enquanto ja haver um bloco na posicao gerada aleatoriamente eh gerada uma nova posicao
        while(grade[y][x] != 0){
            x = rand() % 4;
            y = rand() % 4;
        }

        for (i = 0; i < 16; i++){
            if (blocos[i].valor == 0){
                //printf("i: %d\n", i);
                blocos[i].x = x;
                blocos[i].y = y;
                blocos[i].valor = valor;
                break;
            }
        }
    }
}

int pode_mover_tudo(int grade[4][4], Bloco blocos[16], char dir){
    //essa funcao eh usada para saber se ainda ha possibilidade de movimento de um bloco
    int b = 0, i;

    for (i = 0; i < 16; i++){
        if (pode_mover(grade, blocos[i], dir)){
            b = 1;
            break;
        }
    }
    return b;
}

int verifica(int grade[4][4], Bloco blocos[16], int n){
    //n eh o numero de elementos de blocos[]
    int b = 1, i;

    if (n == 16){
        b = 0;
        for (i = 0; i < n; i++){
            if (pode_mover(grade, blocos[i], 'w')){
                b = 1;
                break;
            }
            if (pode_mover(grade, blocos[i], 's')){
                b = 1;
                break;
            }
            if (pode_mover(grade, blocos[i], 'a')){
                b = 1;
                break;
            }
            if (pode_mover(grade, blocos[i], 'd')){
                b = 1;
                break;
            }
        }
    }

    return b;
}

int ganhou(int grade[4][4]){
    int i, j;
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            if (grade[i][j] == 2048){
                return 1;
            }
        }
    }
    return 0;
}

void delay(){
    int c, d;
    for (c = 1; c <= 1276; c++)
        for (d = 1; d <= 1276; d++) {}

}

void libera(Bloco blocos[16]){
    int i;
    for (i = 0; i < 16; i++){
        blocos[i].fundiu = 0;
    }
}

void imprime_record(char nomes[5][100], double records[5]){
    int i;
    printf("\nRECORDS:\n");
    for (i = 0; i < 5; i++){
        if (records[i] > 0){
            printf("%d- %s: %lf\n", i + 1, nomes[i], records[i]);
        }
    }
}

int main(){

    FILE *arq;
    Bloco blocos[16];
    int a, cont = 0, i, j, k, l, grade[4][4], cont_blocos = 0;
    char dir, nome[100], nomes[5][100], lixo;
    double tempo_de_cpu, records[5];
    clock_t inicio, fim;

    //Preencher a matriz com 0s
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            grade[i][j] = 0;
        }
    }
    for (i = 0; i < 16; i++){
        blocos[i].x = 0;
        blocos[i].y = 0;
        blocos[i].valor = 0;
        blocos[i].fundiu = 0;
    }

    add_bloco(grade, blocos, cont_blocos);
    atualiza(grade, blocos);

    add_bloco(grade, blocos, cont_blocos);
    atualiza(grade, blocos);

    cont_blocos = conta_blocos(blocos);

    inicio = clock();

    while (verifica(grade, blocos, cont_blocos) && !ganhou(grade)){
        system("cls");
        display(grade);
        scanf("%c*c", &dir);
        dir = minusc(dir);

        if (dir != 'a' && dir != 'w' && dir != 'd' && dir != 's'){
            printf("Comando invalido!");
        } else {
            while(pode_mover_tudo(grade, blocos, dir)){
                system("cls");
                for (i = 0; i < 16; i++){
                    move(grade, blocos, dir, i);
                    atualiza(grade, blocos);
                }
                cont_blocos = conta_blocos(blocos);
                display(grade);
                //delay();
            }
            cont_blocos = conta_blocos(blocos);
            add_bloco(grade, blocos, cont_blocos);
            libera(blocos);
            atualiza(grade, blocos);
        }
    }

    fim = clock();
    tempo_de_cpu = ((double)(fim-inicio))/CLOCKS_PER_SEC;

    arq = fopen("save.txt", "r");

    for (i = 0; i < 5; i++){
        fscanf(arq, "%s", &nomes[i]);
        fscanf(arq, "%lf", &records[i]);
        fscanf(arq, "%c", &lixo);
    }

    fclose(arq);

    if (ganhou(grade)){

        printf("Parabens!!! Voce concluiu 2048\n");
        printf("Tempo: %lf\n",tempo_de_cpu);

        for (i = 0; i < 5; i++){
            if (tempo_de_cpu < records[i] || records[i] == 0){
                break;
            }
        }
        if (i < 5){
            printf("Voce bateu um record! Qual o seu nome?\n");
            scanf("%s", &nome);
            if (i == 4){
                records[i] = tempo_de_cpu;
                for (j = 0; nomes[k][j] != '\0'; j++){
                    nomes[i][j] = nome[j];
                }

            } else {
                for (k = i; k < 4; k++){
                    if (records[k + 1] == 0){
                            for (l = 0; nomes[l][j] != '\0'; j++){
                            nomes[l][j] = nome[j];
                        }
                        records[k] = tempo_de_cpu;
                    }
                    for (j = 0; nomes[k][j] != '\0'; j++){
                        nomes[k + 1][j] = nomes[k][j];
                    }
                    nomes[k + 1][j] = '\0';
                    records[k + 1] = records[k];

                    for (l = 0; nomes[l][j] != '\0'; j++){
                        nomes[l][j] = nome[j];
                    }
                    records[k] = tempo_de_cpu;

                    tempo_de_cpu = records[k + 1];
                    for (j = 0; nomes[k][j] != '\0'; j++){
                        nome[j] = nomes[k][j];
                    }
                    nome[j] = '\0';

                }
            }

        }

        arq = fopen("save.txt", "w");
        for (i = 0; i < 5; i++){
            fprintf(arq, "%s ", nomes[i]);
            fprintf(arq, "%lf\n", records[i]);
        }
        fclose(arq);

    } else {
        printf("Voce perdeu :( \n");
    }
    imprime_record(nomes, records);
    return 0;
}
