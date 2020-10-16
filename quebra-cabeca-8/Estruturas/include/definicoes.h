/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef DEFINICOES_H
#define DEFINICOES_H

/* - Toda funcionalidade externa de uma estrutura retorna um valor tipo Erro. Retorno 0 é sucesso. Getters não precisam.
 * - Toda funcionalidade começa com o nome da estrutura antes. Ex: lista_destroi().
 * - Nos nomes das variaveis, caracteristicas que as diferenciam vem depois do tipo. Ex: dadoUm, dadoNovo.
 * - As estruturas sempre terão partes de mesmo tamanho. Porém é possivel usar ponteiros para regiões que variam de tamanho. */

#define Erro int
#define Sim true
#define Nao false
#define Nada NULL
#define Proximo true
#define Anterior false
#define Zero 0
#define Primeira 1
#define Segunda 2
#define Ambas 3

/* Macros de criação de funções que serão usadas nas estruturas. */
#define dadoCopia(nome) void nome(void *dest, const void *orig)
#define dadoLibera(nome) void nome(void *dado)
#define dadoCompara(nome) int nome(const void *dadoUm, const void *dadoDois)
#define dadoUsa(nome) void nome(void *dado)

/* Funções que as estruturas recebem. */
typedef void (*dado_copia)(void *dest, const void *orig); 
typedef void (*dado_libera)(void *dado);
typedef int (*dado_compara)(const void *dadoUm, const void *dadoDois);
typedef void (*dado_usa)(void *dado); 
typedef void *(*dado_aninhado)(void *dado); 


#endif
