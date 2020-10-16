#include <stdlib.h>
#include "../include/heap.h"
#include "../include/vetor.h"

struct heap {
   Vetor *vetor;
};


/* Funções auxiliares. */

static size_t heap_pai(size_t posNo) {
   return (posNo - 1) / 2;
}

static size_t heap_filhoDir(size_t posNo) {
   return (2*posNo + 1);
}

static size_t heap_filhoEsq(size_t posNo) {
   return (2*posNo + 2);
}

static void heap_tornarHeap(Heap *h, size_t posNo) {
   size_t esquerda = heap_filhoEsq(posNo);
   size_t direita = heap_filhoDir(posNo);
   size_t menor = posNo, tamanho = vetor_pegaTamanho(h->vetor);
   int comparador;
   
   if (esquerda < tamanho) {
      vetor_comparaPos(h->vetor, esquerda, posNo, &comparador);
      if (comparador < 0)
         menor = esquerda;
   }

   if (direita < tamanho) {
      vetor_comparaPos(h->vetor, direita, menor, &comparador);
      if (comparador < 0)
         menor = direita;
   }

   if (menor != posNo) {
      vetor_trocaPos(h->vetor, posNo, menor);
      heap_tornarHeap(h, menor);
   }
   return;
}


/* Funções da heap. */

Erro heap_cria(Heap **h, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   *h = malloc(sizeof(Heap));
   vetor_cria(&((*h)->vetor), dadoTam, funcCopia, funcLibera, funcCompara);
   return 0;
}

Erro heap_destroi(Heap **h) {
   vetor_destroi(&((*h)->vetor));
   free(*h);
   *h = NULL;
   return 0;
}

Erro heap_insere(Heap *h, void *dado) {
   size_t i = vetor_pegaTamanho(h->vetor); 
   int comparador;

   vetor_insereFim(h->vetor, dado);
   while (i) {
      vetor_comparaPos(h->vetor, heap_pai(i), i, &comparador);
      if (comparador < 1)
         break;
      vetor_trocaPos(h->vetor, i, heap_pai(i));
      i = heap_pai(i);
   }
   return 0;
}

Erro heap_pegaProx(Heap *h, void **dadoRetorno, bool memoriaNova) {
   vetor_retorna(h->vetor, dadoRetorno, 0, memoriaNova);
   return 0;
}

Erro heap_remove(Heap *h) {
   size_t tamanho = heap_pegaTamanho(h);
   if (!tamanho) 
      return 10;
   if (tamanho == 1) 
      vetor_removeFim(h->vetor);
   else {
      vetor_trocaPos(h->vetor, 0, tamanho - 1);
      vetor_removeFim(h->vetor);
      heap_tornarHeap(h, 0);
   }
   return 0;
} 

Erro heap_itera(Heap *h, dado_usa funcao) {
   vetor_itera(h->vetor, funcao);
   return 0;
}

size_t heap_pegaTamanho(Heap *h) {
   return vetor_pegaTamanho(h->vetor);
}

