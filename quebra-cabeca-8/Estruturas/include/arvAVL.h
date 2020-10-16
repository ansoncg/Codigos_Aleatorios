#ifndef ARVAVL_H
#define ARVAVL_H

#include <stddef.h>
#include <stdbool.h>
#include "definicoes.h"

typedef struct arvAVL ArvAVL;
typedef enum {Pre, In, Pos, Largura} Ordem;

Erro arvAVL_cria(ArvAVL **avl, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara);
Erro arvAVL_itera(ArvAVL *avl, dado_usa funcao, Ordem iteracaoOrdem); 
Erro arvAVL_contemDado(ArvAVL *avl, const void *dado, bool *respota); 
Erro arvAVL_insere(ArvAVL *avl, const void *dado); 
Erro arvAVL_contemDado(ArvAVL *avl, const void *dado, bool *respota); 
Erro arvAVL_destroi(ArvAVL **avl);
size_t arvAVL_pegaTamanho(ArvAVL *avl); 

#endif
