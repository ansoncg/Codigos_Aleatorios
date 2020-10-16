#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Estruturas/include/arvAVL.h"
#include "../Estruturas/include/heap.h"

typedef struct mat {
   int partes[3][3];
} matrizTabuleiro;

typedef struct _jogo {
   matrizTabuleiro m; 
   int profundidade, xBuraco, yBuraco;
   char caminho[40];
} jogo;

void leJogo(jogo *jg); 
int jogoPossivel(jogo jg); 
void busca(jogo jg); 

int main() {
   int testesQuant;
   jogo jg;

   if (!scanf("%d", &testesQuant))
      return 1;
   while (testesQuant--) {
      leJogo(&jg);
      if (jogoPossivel(jg))
         busca(jg);
      else 
         printf("unsolvable\n");
      if (testesQuant)
         putchar('\n');
   }
   return 0;
}

// Leitura e inicialização da entrada dos testes.
void leJogo(jogo *jg) {
   for (int i = 0; i < 3; i++) 
      for (int j = 0; j < 3; j++) {
         if(!scanf("%d", &jg->m.partes[i][j])) {
            jg->m.partes[i][j] = 0;
            jg->xBuraco = i;
            jg->yBuraco = j;
            getchar();
         }
      }
   jg->caminho[0] = '\0';
   jg->profundidade = 0;
   return;
}

// Distancia de manhattan.
int distaManhattan(jogo jg) {
   int dist = 0;  
    
   for (int i = 0; i < 3; i++) 
      for (int j = 0; j < 3; j++)  {
         if (jg.m.partes[i][j])
            dist += abs(((jg.m.partes[i][j] - 1) / 3) - i) + abs(((jg.m.partes[i][j] - 1) % 3) - j); 
      }
   return dist;
}

// Move o tabuleiro e insere na heap caso não esteja na AVL. 
void processaInsercao(Heap *h, ArvAVL *avl, jogo jg, int xMais, int yMais, const char *direcao) {
   jg.m.partes[jg.xBuraco][jg.yBuraco] = jg.m.partes[jg.xBuraco + xMais][jg.yBuraco + yMais];
   jg.m.partes[jg.xBuraco + xMais][jg.yBuraco + yMais] = 0;
   if (arvAVL_insere(avl, &jg.m.partes) != 1) { // Se não houver repetição insere na AVL. Senão retorna 1.
      jg.xBuraco += xMais;
      jg.yBuraco += yMais; 
      strcat(jg.caminho, direcao);
      heap_insere(h, &jg);
   }
}

// Entra as proximas posição na heap. 
void entraProximos(Heap *h, ArvAVL *avl, jogo jg) {
   jg.profundidade++;   
   if (jg.xBuraco < 2)  // Baixo
      processaInsercao(h, avl, jg, 1, 0, "d");
   if (jg.yBuraco < 2)  // Direita
      processaInsercao(h, avl, jg, 0, 1, "r");
   if (jg.xBuraco > 0)  // Cima
      processaInsercao(h, avl, jg, -1, 0, "u");
   if (jg.yBuraco > 0)  // Esquerda
      processaInsercao(h, avl, jg, 0, -1, "l");
   return;
}

// Testa se chegou no fim.
int chegouFim(jogo jg) {
   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
         if (j == 2 && i == 2)
            break;
         if (jg.m.partes[i][j] != (3*i + j + 1)) 
            return 0;
      }
   return 1;
}

// Testa se o jogo é possivel.
int jogoPossivel(jogo jg) {
   int inversoes = 0, vetJogo[9];

   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) 
         vetJogo[3*i + j] = jg.m.partes[i][j];
   for (int i = 0; i < 9; i++) {
      for (int j = i; j < 9; j++)
         if (vetJogo[i] > vetJogo[j] && vetJogo[j])
            inversoes++;
   }
   return (!(inversoes % 2));
}

// Pega o proximo da heap e remove o menor.
void pegaProx(Heap *h, jogo *jg) {
   void *pega = NULL;

   heap_pegaProx(h, &pega, false); 
   *jg = *(jogo *) pega;
   heap_remove(h);
   return;
}

// Heuristica de comparação para inserir na heap. Distancia manhattan + profundidade.
dadoCompara(compJogo) {
   int distUm = distaManhattan(*(jogo *) dadoUm) + ((jogo *)dadoUm)->profundidade;
   int distDois = distaManhattan(*(jogo *) dadoDois) + ((jogo *)dadoDois)->profundidade;
   return distUm - distDois;
}

// Compara as matriz para inserir na lista.
dadoCompara(compMatriz) {
   int comp;

   for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
         comp = ((((matrizTabuleiro *)dadoUm)->partes[i][j] - (((matrizTabuleiro *)dadoDois)->partes[i][j])));
         if (comp)
           return comp;
      }
   return 0;
}

// Resolve com A*.
void busca(jogo jg) {
   Heap *h = NULL; 
   ArvAVL *avl = NULL;

   heap_cria(&h, sizeof(jogo), NULL, NULL, compJogo);
   heap_insere(h, &jg); 
   arvAVL_cria(&avl, sizeof(matrizTabuleiro), false, NULL, NULL, compMatriz); // AVL de 'matrizTabuleiro' que não aceita repetição.
   while (heap_pegaTamanho(h)) {
      pegaProx(h, &jg); 
      if (chegouFim(jg)) {
         printf("%s\n", jg.caminho);
         break;
      }
      entraProximos(h, avl, jg);
   }
   heap_destroi(&h);
   arvAVL_destroi(&avl);
   return;
}
