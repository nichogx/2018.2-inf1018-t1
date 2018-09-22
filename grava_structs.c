/* Nicholas de Godoy Xavier 1710878 3WA */
/* Alexandre Augusto Poggio Heine 1711905 3WA */

/* Modulo de Implementacao: grava_structs
 * Arquivo: grava_structs.c
 * 
 * Autores:
 * ngx - Nicholas Godoy
 * alx - Alexandre Augusto
 *
 * Versionamento:
 * Autor(es)   Versao    Data       Descricao
 * ngx         0.20      2018-09-20 Gravacao do cabecalho dos arquivos
 * ngx         0.10      2018-09-18 Inicial
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "grava_structs.h"

/* CABECALHO DAS FUNCOES ENCAPSULADAS NO MODULO */

int leCampo(char c);
int power(int bas, int exp);

/* FUNCOES EXPORTADAS PELO MODULO */
int grava_structs(int nstructs, void *valores, char *campos, char ord, char *arquivo)
{
	FILE *arq = fopen(arquivo, "wb");
	
	unsigned char qtCampos = 0;
	unsigned char segByte  = 0; /* inicializado como sendo big endian */
	unsigned char maiorCampo = leCampo(*campos);
	
	char *pCampos = campos;
	char *pVals = valores;
	
	unsigned char atual = 0;
	int contAtual = 0;
	
	int i;
	
	if (arq == NULL) return -1;
	
	for (pCampos = campos; *pCampos; pCampos++, qtCampos++) { /* conta campos e acha o maior */
		unsigned char campAt = leCampo(*pCampos);
		if (campAt > maiorCampo) {
			maiorCampo = campAt;
		}
	}

	fwrite(&nstructs, 1, 1, arq); /* escreve no arquivo o byte menos significativo de nstructs */
	                              /* assume que nstructs <= 255 (argumentos passados corretamente) */
	if (ord == 'L') {
		segByte = 0x80; /* seta o bit mais significativo para 1 */
	}
	segByte = segByte | qtCampos; /* assume que qtCampos <= 127 && qtCampos >= 0 (argumentos passados corretamente) */
	fwrite(&segByte, 1, 1, arq);
	
	for (pCampos = campos; *pCampos; pCampos++) {
		if (contAtual > 3) {
			fwrite(&atual, 1, 1, arq);
			contAtual = 0;
			atual = 0;
		}
		atual = atual << 2;
		atual = leCampo(*pCampos) | atual;
		contAtual++;
	} if (contAtual) { /* completa com zeros se ainda faltar */
		atual = atual << ((4 - contAtual) * 2);
		fwrite(&atual, 1, 1, arq);
	}
	
	for (i = 0; i < nstructs; i++) {
		unsigned char valAtual = 0;
		for (pCampos = campos; *pCampos; pCampos++) {
			unsigned char campAtual = leCampo(*pCampos);
			unsigned char tam = power(2, campAtual);
			
			while (valAtual % tam) { /* anda pelo padding */
				pVals++;
				valAtual++;
			}
			
			fwrite(pVals, tam, 1, arq);
			pVals += tam;
			valAtual += tam;
		}
		while (valAtual % power(2, maiorCampo)) { /* anda pelo padding */
			pVals++;
			valAtual++;
		}
	}
	
	fclose(arq);
	return 0;
}

void dump_structs(char *arquivo)
{
	FILE *arq = fopen(arquivo,"rb");
	//int linha = string2num(fgetc(arq));
	
	fclose(arq);
}

/* FUNCOES ENCAPSULADAS NO MODULO */

int leCampo(char c) {
	if (c == 's')
		return 1;
	
	if (c == 'i')
		return 2;
	
	if (c == 'l')
		return 3;
	
	return 0;
}

int power(int bas, int exp) {
	int i, res = 1;
	
	for (i = 0; i < exp; i++)
		res *= bas;
	
	return res;
}