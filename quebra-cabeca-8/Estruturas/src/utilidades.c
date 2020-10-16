/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utilidades.h"


/* Le uma string comum de tamanho indeterminado de 
 * uma stream. */
char *leString(FILE *entrada) {
   size_t strTam = 32, i = 0;
   char *string = malloc(strTam), c;

   while (((c = (char) getc(entrada)) != '\n') && c != EOF) {
      if (i == strTam) {
         strTam *= 2;
         string = realloc(string, strTam);
      }
      string[i++] = c;
   }
   string[i] = '\0';
   string = realloc(string, i + 1);
   return string;
}

/* Funções para serem usadas no iterador. */

void imprimeChar(void *dado) {
   printf("%c ", *(char *) dado);
   return;
}

void imprimeInteiro(void *dado) {
   printf("%d ", *(int *) dado);
   return;
}

void imprimeInteiroL(void *dado) {
   printf("%ld ", *(long *) dado);
   return;
}

void imprimeInteiroLL(void *dado) {
   printf("%lld ", *(long long *) dado);
   return;
}

void imprimeReal(void *dado) {
   printf("%lf ", *(float *) dado);
   return;
}

void imprimeRealD(void *dado) {
   printf("%lf ", *(double *) dado);
   return;
}

void imprimeString(void *dado) {
   printf("%s ", (char *) dado);
   return;
}

void imprimeStringV(void *dado) {
   printf("%s\n", (char *) dado);
   return;
}

//--------------------------------------------------------------------------------------------

/* Comparadores. 
 
Em vez de memcmp direto pode implementar uma função
propria para tipos simples do tipo:

return ((*(char *) dadoUm) - ((*(char *) dadoDois)));
*/

int comparaString(void *dadoUm, void *dadoDois) {
   return strcmp((char *) dadoUm, (char *) dadoDois);
}

//--------------------------------------------------------------------------------------------

/* Joga para o stderr os erros dados pelas estruturas. */
void checaErro(Erro saidaFunc) {
   switch (saidaFunc) {
      case 1:
         fprintf(stderr, "Dado repetido em estrutura sem repeticao\n");
         break;
      case 2:
         fprintf(stderr, "Estrutura eh ordenada, funcao incompativel\n");
         break;
      case 3:
         fprintf(stderr, "Estrutura nao eh ordenada, funcao incompativel\n");
         break;
      case 4:
         fprintf(stderr, "Grafo nao valorado, funcao incompativel\n");
         break;
      case 5:
         fprintf(stderr, "Estrutura vazia, nao houve remocao\n");
         break;
      case 6:
         fprintf(stderr, "Dado nao encontrado na estrutura\n");
         break;
      case 7:
         fprintf(stderr, "Estrutura vazia, nao houve busca\n");
         break;
      case 8:
         fprintf(stderr, "Posicao fora das possiveis\n");
         break;
      case 9:
         fprintf(stderr, "Numero da gaveta invalido\n");
         break;
      case 10:
         fprintf(stderr, "Estrutura nao inicializada, abortando\n");
         break;
   }
}
