#include <stdio.h>
#include <stdlib.h>

#define AP_MAX 1000
#define AP_TAM 6

void leApostasArq(int apostas[AP_MAX][AP_TAM], int *quantApostas); 
void checaResultado(int apostas[AP_MAX][AP_TAM], int quantApostas);

int main() {
    int apostas[AP_MAX][AP_TAM], quantApostas = 0;

    leApostasArq(apostas, &quantApostas); 
    checaResultado(apostas, quantApostas);
    return 0;
}

void leApostasArq(int apostas[AP_MAX][AP_TAM], int *quantApostas) {
    char nomeArquivo[100];
    FILE *arquivo = NULL;

    while ((arquivo = fopen(nomeArquivo, "r")) == NULL) {
        printf("Entre com um nome valido para o arquivo de apostas:\n");  
        scanf(" %s", nomeArquivo);
    }
    while (!feof(arquivo)) {
        for (int i = 0; i < AP_TAM; i++)
            fscanf(arquivo, "%d ", &(apostas[*quantApostas][i]));
        (*quantApostas)++;
    }
    fclose(arquivo);
    return;
}

int compInt(const void *a, const void *b) {
    return (*(int *) a) - (*(int *) b);
}

int checaAcertos(int aposta[AP_TAM], int sorteado[AP_TAM]) {
    int acertos = 0;

    qsort(aposta, AP_TAM, sizeof(int), compInt); 
    for (int i = 0; i < AP_TAM; i++) {
        for (int j = 0; j < AP_TAM; j++) {
            if (aposta[j] == sorteado[i]) {
                acertos++;
                printf("%d ", aposta[j]);
                break;
            }
            else if (aposta[j] > sorteado[i])
                break;
        }
    }  
    return acertos;
}

void checaResultado(int apostas[AP_MAX][AP_TAM], int quantApostas) {
    int sorteado[AP_TAM], maxAcertos = 0, acertos;

    printf("Quais os numeros sorteados?\n");
    for (int i = 0; i < AP_TAM; i++)
        scanf("%d", &sorteado[i]); 
    qsort(sorteado, AP_TAM, sizeof(int), compInt); 
    printf("\nNumeros sorteados:\n");
    for (int i = 0; i < AP_TAM; i++)
        printf("%d ", sorteado[i]);
    printf("\n\n");
    for (int i = 0; i < quantApostas; i++) {
        printf("Acertos aposta %d: ", i + 1); 
        acertos = checaAcertos(apostas[i], sorteado);
        if (acertos > maxAcertos)
            maxAcertos = acertos;
        putchar('\n');
    } 
    printf("\nMaior quantidade de acertos: %d\n", maxAcertos);
    return;
}
