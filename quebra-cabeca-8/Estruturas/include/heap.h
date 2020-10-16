#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdbool.h>
#include "definicoes.h"

typedef struct heap Heap;


Erro heap_cria(Heap **h, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara);
Erro heap_destroi(Heap **h);
Erro heap_insere(Heap *h, void *dado);
Erro heap_pegaProx(Heap *h, void **dadoRetorno, bool memoriaNova);
Erro heap_remove(Heap *h);
Erro heap_itera(Heap *h, dado_usa funcao);

size_t heap_pegaTamanho(Heap *h); 

#endif
