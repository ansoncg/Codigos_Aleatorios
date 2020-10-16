/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stddef.h>
#include "definicoes.h"

char *leString(FILE *entrada); 

void imprimeChar(void *dado); 
void imprimeInteiro(void *dado);
void imprimeInteiroL(void *dado); 
void imprimeInteiroLL(void *dado); 
void imprimeReal(void *dado); 
void imprimeRealD(void *dado); 
void imprimeString(void *dado);
void imprimeStringV(void *dado); 

int comparaChar(void *dadoUm, void *dadoDois); 
int comparaInteiro(void *dadoUm, void *dadoDois); 
int comparaInteiroL(void *dadoUm, void *dadoDois); 
int comparaInteiroLL(void *dadoUm, void *dadoDois); 
int comparaReal(void *dadoUm, void *dadoDois); 
int comparaRealD(void *dadoUm, void *dadoDois); 
int comparaString(void *dadoUm, void *dadoDois); 

void checaErro(Erro saidaFunc); 

#endif
