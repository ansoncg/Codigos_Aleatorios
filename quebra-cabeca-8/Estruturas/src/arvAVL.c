/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdlib.h>
#include <string.h>
#include "../include/arvAVL.h"

// TODO: Colocar remoção, aceitar repetição, iteração em largura, erros.

typedef struct arvAVL_no {
   void *dado; 
   int altura;
   struct arvAVL_no *esquerda, *direita;
} ArvAVL_No;

struct arvAVL {
   ArvAVL_No *raiz;
   size_t dadoTam, noQuant; 
   bool repeticao;
   dado_copia funcCopia;
   dado_libera funcLibera;
   dado_compara funcCompara;
}; 

/* Funções auxiliares, internas. */

static int arvAVL_max(int a, int b) {
   return (a > b) ? a : b;
}

static int arvAVL_noAltura(ArvAVL_No *no) {
   if (no == NULL)
      return 0;
   return no->altura;
}

static void arvAVL_iteraPre(ArvAVL_No *avl_no, dado_usa funcao) {
   if (avl_no == NULL)
      return;
   (*funcao)(avl_no->dado);
   arvAVL_iteraPre(avl_no->esquerda, funcao);
   arvAVL_iteraPre(avl_no->direita, funcao);
   return;
} 

static void arvAVL_iteraIn(ArvAVL_No *avl_noAtual, dado_usa funcao) {
   if (avl_noAtual == NULL)
      return;
   arvAVL_iteraPre(avl_noAtual->esquerda, funcao);
   (*funcao)(avl_noAtual->dado);
   arvAVL_iteraPre(avl_noAtual->direita, funcao);
   return;
} 

static void arvAVL_iteraPos(ArvAVL_No *avl_no, dado_usa funcao) {
   if (avl_no == NULL)
      return;
   arvAVL_iteraPre(avl_no->esquerda, funcao);
   arvAVL_iteraPre(avl_no->direita, funcao);
   (*funcao)(avl_no->dado);
   return;
} 

static void arvAVL_dadoCopia(ArvAVL_No **avl_no, const void *dado, ArvAVL *avl) {
   if (avl->funcCopia == NULL)
      memcpy((*avl_no)->dado, dado, avl->dadoTam);
   else
      avl->funcCopia((*avl_no)->dado, dado);
   return;
}

static void arvAVL_dadoLibera(ArvAVL *avl, ArvAVL_No **noDel) {
   if ((avl)->funcCopia != NULL)
      (avl)->funcLibera(*noDel);
   free((*noDel)->dado);
   free(*noDel);
   return;
}

static int arvAVL_dadoCmp(ArvAVL *avl, const void *dadoUm, const void *dadoDois) {
   if (avl->funcCompara == NULL) 
      return memcmp(dadoUm, dadoDois, avl->dadoTam);
   else
      return avl->funcCompara(dadoUm, dadoDois);
}

static ArvAVL_No *arvAVL_buscaDado(ArvAVL *avl, const void *dado, ArvAVL_No *noAtual) {
   int comparacao;

   if (noAtual == NULL)
      return NULL;
   comparacao = arvAVL_dadoCmp(avl, dado, noAtual->dado);
   if (!comparacao)
      return noAtual;
   else
      return (comparacao < 0 ? arvAVL_buscaDado(avl, dado, noAtual->esquerda) : arvAVL_buscaDado(avl, dado, noAtual->direita));
}


static ArvAVL_No* arvAVL_criaNo(ArvAVL *avl, const void *dado) {
   ArvAVL_No *avl_no = malloc(sizeof(ArvAVL_No));

   avl_no->dado = malloc(avl->dadoTam);
   arvAVL_dadoCopia(&avl_no, dado, avl);   
   avl_no->direita = avl_no->esquerda = NULL;
   avl_no->altura = 1;
   return avl_no;
}

static ArvAVL_No* arvAVL_rotacionaDir(ArvAVL_No *no) {
   ArvAVL_No *raizNova = no->esquerda;
   ArvAVL_No *realoca = raizNova->direita;

   raizNova->direita = no;
   no->esquerda = realoca;
   no->altura = arvAVL_max(arvAVL_noAltura(no->esquerda), arvAVL_noAltura(no->direita)) + 1;
   raizNova->altura = arvAVL_max(arvAVL_noAltura(raizNova->esquerda), arvAVL_noAltura(raizNova->direita)) + 1;
   return raizNova;
}

static ArvAVL_No* arvAVL_rotacionaEsq(ArvAVL_No *no) {
   ArvAVL_No *raizNova = no->direita;
   ArvAVL_No *realoca = raizNova->esquerda;

   raizNova->esquerda = no;
   no->direita = realoca;
   no->altura = arvAVL_max(arvAVL_noAltura(no->esquerda), arvAVL_noAltura(no->direita)) + 1;
   raizNova->altura = arvAVL_max(arvAVL_noAltura(raizNova->esquerda), arvAVL_noAltura(raizNova->direita)) + 1;
   return raizNova;
}

static int arvAVL_pegaFB(ArvAVL_No *no) {
   if (no == NULL)
      return 0;
   return arvAVL_noAltura(no->esquerda) - arvAVL_noAltura(no->direita);
}

static ArvAVL_No* arvAVL_insereNo(ArvAVL *avl, ArvAVL_No *noAtual, const void *dado, bool *repetido) { //Deixando o bool repetido por enquanto, tirar depois.
   int compDir, compEsq, balanceamento;

   if (noAtual == NULL) 
      return arvAVL_criaNo(avl, dado); 

   compDir = arvAVL_dadoCmp(avl, dado, noAtual->dado);
   if (compDir < 0)
      noAtual->esquerda = arvAVL_insereNo(avl, noAtual->esquerda, dado, repetido);
   else if (compDir > 0)
      noAtual->direita = arvAVL_insereNo(avl, noAtual->direita, dado, repetido);
   else {
      *repetido = true;
      return noAtual;
   }

   noAtual->altura = 1 + arvAVL_max(arvAVL_noAltura(noAtual->esquerda), arvAVL_noAltura(noAtual->direita));
   balanceamento = arvAVL_pegaFB(noAtual);

   // Rotaciona Direita.
   if (balanceamento > 1 && (compEsq = arvAVL_dadoCmp(avl, dado, noAtual->esquerda->dado)) < 0)
      return arvAVL_rotacionaDir(noAtual);

   // Rotaciona Esquerda.
   if (balanceamento < -1 && (compDir = arvAVL_dadoCmp(avl, dado, noAtual->direita->dado)) > 0)
      return arvAVL_rotacionaEsq(noAtual);

   // Rotaciona Esquerda-Direita.
   if (balanceamento > 1 && compEsq > 0) {
      noAtual->esquerda = arvAVL_rotacionaEsq(noAtual->esquerda); 
      return arvAVL_rotacionaDir(noAtual);
   }

   // Rotaciona Direita-Esquerda.
   if (balanceamento < -1 && compDir < 0) {
      noAtual->direita = arvAVL_rotacionaDir(noAtual->direita);
      return arvAVL_rotacionaEsq(noAtual);
   }
   return noAtual;
}

void arvAVL_destroiNos(ArvAVL *avl, ArvAVL_No *noAtual) {
   if (noAtual == NULL)
      return;
   arvAVL_destroiNos(avl, noAtual->direita);
   arvAVL_destroiNos(avl, noAtual->esquerda);
   arvAVL_dadoLibera(avl, &noAtual); 
   return;
}

/* Funções externas, funcionalidades da Arvore AVL. */

Erro arvAVL_cria(ArvAVL **avl, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   (*avl) = malloc(sizeof(ArvAVL));
   (*avl)->raiz = NULL;
   (*avl)->repeticao = repeticao;
   (*avl)->noQuant = 0;
   (*avl)->dadoTam = dadoTam;
   (*avl)->funcCopia = funcCopia;
   (*avl)->funcLibera = funcLibera;
   (*avl)->funcCompara = funcCompara;
   return 0;
}

Erro arvAVL_itera(ArvAVL *avl, dado_usa funcao, Ordem iteracaoOrdem) {
   switch (iteracaoOrdem) {
      case Pre:
         arvAVL_iteraPre(avl->raiz, funcao);    
         break;
      case In:
         arvAVL_iteraIn(avl->raiz, funcao);    
         break;
      case Pos:
         arvAVL_iteraPos(avl->raiz, funcao);    
         break;
      case Largura:
         //TODO
         break;
   }
   return 0;
}

Erro arvAVL_insere(ArvAVL *avl, const void *dado) { 
   bool repetido = false;
   avl->raiz = arvAVL_insereNo(avl, avl->raiz, dado, &repetido);
   if (repetido)
      return 1;
   else {
      avl->noQuant++;
      return 0;
   }
}

Erro arvAVL_contemDado(ArvAVL *avl, const void *dado, bool *respota) {
   ArvAVL_No *noProcura = arvAVL_buscaDado(avl, dado, avl->raiz);

   *respota = (noProcura != NULL);
   return 0;
}

Erro arvAVL_destroi(ArvAVL **avl) {
   arvAVL_destroiNos(*avl, (*avl)->raiz); 
   free(*avl);
   *avl = NULL;
   return 0;
}

size_t arvAVL_pegaTamanho(ArvAVL *avl) {
   return avl->noQuant; 
}
