#include <stdlib.h>
#include <string.h>
#include "../include/vetor.h"

/* TODO: Botar erros, comentarios, criar a opção de iterar até certo dado, Getters.*/

/* Dificuldades da implementação de um vetor generico sem usar macros:
 *    - Não tem como criar apenas um vetor "void *elementos", não tem como dividir pois não se sabe o tamanho,
 *    portanto é necessario um vetor de ponteiros void o que torna a memoria dos dados
 *    não consecutiva, criando alguns problemas.
 *
 * Esse problemas acarretam em:
 *    - A função de comparação passada pelo usuario terá que entender que o dado é um ponteiro duplo. 
 *    É um problema contornavel se fizer um ordernar que não usar qsort(). 
 *    - Não tem como usar tirar proveito de funções que levam em conta memoria consecutiva, deixando a
 *    implementação menos efeciente.
 *    - Quando um dado complexo entra em uma posição do vetor já alocada é necessario desalocar
 *    com a função do usario antes pois não se sabe o tamanho do novo dado. Ex: Tinha uma string que foi
 *    usada malloc(5) mas a nova string precisa de malloc(10).
 *
 */

// Se declarar a struct nessa ordem dá para usar o qsort com uma função de comparar que desreferencia duas vezes (**).
// Garantido pela implementação do C, a ordem dos dados é a mesma e não há padding antes do primeiro campo.
typedef struct _elemento {
   void *dado;
   bool alocado;
} Vetor_No;

struct vetor {
   Vetor_No *vetDados;
   size_t tamanho, capacidade, dadoTam;
   dado_copia funcCopia;
   dado_libera funcLibera;
   dado_compara funcCompara;
};

/* Funções auxiliares. */

static bool vetor_cabe(Vetor *vet, size_t pos) {
   return vet->capacidade > pos;
}

static void vetor_aumentaCap(Vetor *vet, size_t capDesejada) {
   size_t capAtual = vet->capacidade;
   size_t multiplicador = (capDesejada / capAtual) + 1;
   vet->vetDados = realloc(vet->vetDados, multiplicador*capAtual*sizeof(Vetor_No));
   for (size_t i = capAtual; i < (multiplicador*capAtual); i++) 
      vet->vetDados[i].alocado = false;
   vet->capacidade *= multiplicador;
   return;
}

/* Funcionalidades. */

Erro vetor_insere(Vetor *vet, const void *dado, size_t pos) {
   if (!vetor_cabe(vet, pos))
      vetor_aumentaCap(vet, pos);
   if (!(vet->vetDados[pos].alocado)) {
      vet->vetDados[pos].dado = malloc(vet->dadoTam);
      vet->vetDados[pos].alocado = true;
   }
   else if (vet->funcCopia != NULL) 
      vet->funcLibera(vet->vetDados[pos].dado);
   memcpy(vet->vetDados[pos].dado, dado, vet->dadoTam);
   if (vet->funcCopia != NULL)
      vet->funcCopia(vet->vetDados[pos].dado, dado);
   if (pos >= vet->tamanho)
      vet->tamanho = pos + 1;
   return 0;
} 

Erro vetor_retorna(Vetor *vet, void **dadoRetorno, size_t pos, bool memoriaNova) {
   if (memoriaNova) {
      (*dadoRetorno) = malloc(vet->dadoTam);
      if (vet->funcCopia == NULL)
         memcpy(*dadoRetorno, (vet->vetDados[pos].dado), vet->dadoTam);
      else
         vet->funcCopia(*dadoRetorno, (vet->vetDados[pos].dado));
   }
   else  
      (*dadoRetorno) = vet->vetDados[pos].dado;
   return 0;
}

Erro vetor_cria(Vetor **vet, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   *vet = malloc(sizeof(Vetor)); 
   (*vet)->dadoTam = dadoTam;
   (*vet)->funcCopia = funcCopia;
   (*vet)->funcLibera = funcLibera;
   (*vet)->funcCompara = funcCompara;
   (*vet)->tamanho = 0;
   (*vet)->capacidade = 2;
   (*vet)->vetDados = malloc(sizeof(Vetor_No)*2);
   (*vet)->vetDados[0].alocado = (*vet)->vetDados[1].alocado = false;
   return 0;
}

Erro vetor_insereFim(Vetor *vet, void *dado) {
   vetor_insere(vet, dado, vet->tamanho); 
   return 0;
}

Erro vetor_removeFim(Vetor *vet) {
   vet->tamanho--;
   return 0;
}

Erro vetor_esvazia(Vetor *vet) {
   vet->tamanho = 0;
   return 0;
}

Erro vetor_copia(Vetor *vetDest, const Vetor *vetOrig) {
   if (vetDest == NULL || vetOrig == NULL)
      return 10;
   vetor_destroi(&vetDest);
   vetor_cria(&vetDest, vetOrig->dadoTam, vetOrig->funcCopia, vetOrig->funcLibera, vetOrig->funcCompara);
   for (size_t i = 0; i < vetOrig->tamanho; i++)
      if (vetOrig->vetDados[i].alocado)
         vetor_insere(vetDest, vetOrig->vetDados[i].dado, i);
   return 0;
}

Erro vetor_destroi(Vetor **vet) {
   if ((*vet)->funcLibera != NULL)  {
      for (size_t i = 0; i < (*vet)->capacidade; i++) {
         if ((*vet)->vetDados[i].alocado) {
            (*vet)->funcLibera((*vet)->vetDados[i].dado);
            free((*vet)->vetDados[i].dado);
         }
      }
   }
   else 
      for (size_t i = 0; i < (*vet)->capacidade; i++) 
         if ((*vet)->vetDados[i].alocado) 
            free((*vet)->vetDados[i].dado);
   free((*vet)->vetDados);
   free(*vet);
   *vet = NULL;
   return 0;
}

Erro vetor_itera(Vetor *vet, dado_usa funcao) {
   for (size_t i = 0; i < vet->tamanho; i++) {
      if (vet->vetDados[i].alocado)
         funcao((vet->vetDados[i].dado));
   }
   return 0;
}

Erro vetor_comparaPos(Vetor *vet, size_t posUm, size_t posDois, int *resultado) {
   if (vet->funcCompara == NULL)
      *resultado = memcmp(vet->vetDados[posUm].dado, vet->vetDados[posDois].dado, vet->dadoTam);
   else
      *resultado = vet->funcCompara(vet->vetDados[posUm].dado, vet->vetDados[posDois].dado);
   return 0;
}

Erro vetor_trocaPos(Vetor *vet, size_t posUm, size_t posDois) {
   void *guarda = vet->vetDados[posUm].dado;
   vet->vetDados[posUm].dado = vet->vetDados[posDois].dado;
   vet->vetDados[posDois].dado = guarda;
   return 0;
}

// Enquanto não faço meu proprio sort a funCompara terá que castar ponteiro duplo.
Erro vetor_ordena(Vetor *vet) {
   if (vet->funcCompara != NULL)
      qsort(vet->vetDados, vet->tamanho, sizeof(Vetor_No), vet->funcCompara);
   return 0;
}

Erro vetor_temBuraco(Vetor *vet, bool *retorno) {
   *retorno = true;
   for (size_t i = 0; i < vet->tamanho; i++)
      if (!vet->vetDados[i].alocado) 
         *retorno = false;
   return 0; 
}

Erro vetor_inicializa(Vetor *vet, void *dadoInicializador, size_t tamFinal) {
   for (size_t i = 0; i < tamFinal; i++)
      vetor_insere(vet, dadoInicializador, i); 
   return 0;
}

// Getters do vetor.

size_t vetor_pegaTamanho(Vetor *vet) {
   return vet->tamanho;
}

size_t vetor_pegaDadoTam(Vetor *vet) {
   return vet->dadoTam;
}

