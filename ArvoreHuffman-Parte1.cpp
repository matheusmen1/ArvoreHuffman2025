#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio2.h>
#include "TADPilha.h"
struct bits
{
 unsigned char b7:1;
 unsigned char b6:1;
 unsigned char b5:1;
 unsigned char b4:1;
 unsigned char b3:1;
 unsigned char b2:1;
 unsigned char b1:1;
 unsigned char b0:1;
};
union byte
{
 struct bits bi;
 unsigned char num;
};
struct reg
{
	char palavra[15], codigo[8];
	int simbolo;
	float freq;
};
typedef struct reg Reg;

struct lista
{
	Reg reg;
	struct lista *prox;
};
typedef struct lista Lista;
struct floresta
{
	Tree *no;
	struct floresta *prox;
};
typedef struct floresta Floresta;

Lista *CriaNo(char *palavra)
{
	Lista *NC;
	NC = (Lista*)malloc(sizeof(Lista));
	NC->prox = NULL;
	strcpy(NC->reg.palavra, palavra);
	NC->reg.freq = 0;
	NC->reg.simbolo = 0;
	strcpy(NC->reg.codigo, "");
	
	return NC;
}
Tree *CriaNoTree(int simbolo, float freq)
{
	Tree *NC;
	NC = (Tree*)malloc(sizeof(Tree));
	NC->dir = NULL;
	NC->esq = NULL;
	NC->simbolo = simbolo;
	NC->freq = freq;
	
	return NC;
}
Floresta *CriaNoFloresta(Tree *no)
{
	Floresta *NC;
	NC = (Floresta*)malloc(sizeof(Floresta));
	NC->prox = NULL;
	NC->no = no;
	
	return NC;
	
}
Lista *buscarPalavra(char *palavra, Lista *L, Lista **no)
{
	while(L != NULL && strcmp(L->reg.palavra, palavra) != 0)
		L = L->prox;
	if (L != NULL) // achou
		*no = L;
	else
		*no = NULL;
		
	return *no;
}
void inserirLista(Lista **L, Lista *no)
{
	Lista *aux;
	if (*L == NULL)
		*L = no;
	else
	{
		aux = *L;
		while(aux->prox != NULL)
			aux = aux->prox;
			
		aux->prox = no;
			
	}
}
void interpretarFrase(char *frase, Lista **L, int *contPalavras)
{
	int i = 0, j = 0, cont = 0;
	char palavra[15];
	Lista *no = NULL;
	while(frase[i] != '\0')
	{
		if (frase[i] != ' ')
		{
			palavra[j] = frase[i];
			j++;
		}
		else // sempre que achar uma palavra, acho um espaco
		{
			buscarPalavra(" ", *L, &no);
			if (no != NULL)
			{
				no->reg.freq++;
			}
			else
			{
				no = CriaNo(" ");
				no->reg.freq++;
				no->reg.simbolo = cont;
				cont++;
				inserirLista(&(*L), no);
			}
			
			if (j > 0) // se não for espaço
			{
				palavra[j] = '\0';
				buscarPalavra(palavra, *L, &no);
				if (no != NULL) // achou, portanto, so incrementa freq
				{
					no->reg.freq++;
				}
				else // insere no final da lista
				{
					no = CriaNo(palavra);
					no->reg.freq++;
					no->reg.simbolo = cont;
					cont++;
					inserirLista(&(*L), no);
					
				}
				palavra[0] = '\0';
				j = 0;
				*contPalavras = *contPalavras + 1;
			}
			*contPalavras = *contPalavras + 1;
		}
		i++;
	}
	if (j > 0) // pegar a ultima palavra restante
	{
	    palavra[j] = '\0';
	    buscarPalavra(palavra, *L, &no);
	    if (no != NULL)
	    {
	        no->reg.freq++;
	    }
	    else
	    {
	        no = CriaNo(palavra);
	        no->reg.freq++;
	        no->reg.simbolo = cont;
	        inserirLista(&(*L), no);
	    }
	    *contPalavras = *contPalavras + 1;
	}

}
void calcularFreq(Lista *L, int contPalavras)
{
		
	while(L != NULL)
	{
		L->reg.freq = L->reg.freq / (float)contPalavras;
		L = L->prox;
	}
	

}
void construirFloresta(Floresta **floresta, Lista *L)
{
	Floresta *NCFloresta, *aux, *ant;
	Tree *NCTree;

	while(L != NULL)
	{
		
		NCTree = CriaNoTree(L->reg.simbolo, L->reg.freq);
		NCFloresta = CriaNoFloresta(NCTree);
		if (*floresta == NULL)
		{
			*floresta = NCFloresta;
		}
		else // se o que for inserir, for menor do que a cabeca aponta
		if((*floresta)->no->freq > L->reg.freq)
		{
			NCFloresta->prox = *floresta;
			*floresta = NCFloresta;
		}
		else // preciso percorrer a floresta, e assim, inserir
		{
			aux = *floresta;
			while(aux->prox != NULL && aux->prox->no->freq < L->reg.freq)
				aux = aux->prox;
			
			NCFloresta->prox = aux->prox;
			aux->prox = NCFloresta;
			
		}
		
		
		L = L->prox;
	}
	
}
void construirArvoreHuffman(Floresta **F)
{
	Floresta *primeiro, *segundo, *aux, *NCFloresta;
	Tree *NCTree;
	while((*F)->prox != NULL)
	{
		primeiro = *F;
		segundo = (*F)->prox;
		NCTree = CriaNoTree(-1, primeiro->no->freq + segundo->no->freq);
		NCTree->esq = primeiro->no;
		NCTree->dir = segundo->no;
		NCFloresta = CriaNoFloresta(NCTree);
		*F = segundo->prox;
		free(primeiro);
		free(segundo);
		if(*F == NULL)
			*F = NCFloresta;
		else
		if((*F)->no->freq > NCFloresta->no->freq)
		{
			NCFloresta->prox = *F;
			*F = NCFloresta;
		}
		else
		{
			aux = *F;
			while(aux->prox != NULL && aux->prox->no->freq < NCFloresta->no->freq)
				aux = aux->prox;
			
			NCFloresta->prox = aux->prox;
			aux->prox = NCFloresta;
		}
	}
}
void gerarCodigosDeHuffman(Tree *raiz, Lista *L)
{
	Lista *aux;
	char codigo[9] = "";
	Pilha *P;
	init(&P);
	push(&P, raiz, codigo);
	while(!isEmpty(P))
	{
		if(raiz != NULL)
		{
			pop(&P, &raiz, codigo);
			while(raiz != NULL)
			{
				push(&P, raiz, codigo);
				strcat(codigo, "1");
				raiz = raiz->dir;
			}
		}
		pop(&P, &raiz, codigo);
		if (raiz->esq == NULL) // folha
		{
			aux = L;
			while(aux != NULL && aux->reg.simbolo != raiz->simbolo)
				aux = aux->prox;
			if (aux != NULL)
				strcpy(aux->reg.codigo, codigo);
			strcpy(codigo, "");
		}
		
		raiz = raiz->esq;
		if (raiz != NULL)
		{
			strcat(codigo, "0");
			push(&P, raiz, codigo);
		}
		
			
	}
	
}
void gravarArqBin(Lista *L)
{
	FILE * ptr = fopen("registro.dat", "wb");
	if (ptr == NULL)
		printf("Erro de Abertura\n");
	else
	{	
		rewind(ptr);
		Reg reg;
		while(L != NULL)
		{
			strcpy(reg.codigo, L->reg.codigo);
			reg.freq = L->reg.freq;
			strcpy(reg.palavra, L->reg.palavra);
			reg.simbolo = L->reg.simbolo;
			fwrite(&reg, sizeof(reg), 1, ptr);
			L = L->prox;
			
		}
	}
	
	fclose(ptr);
}
void codificarFrase(char *frase, char *fraseCodificada, Lista *L)
{
	int i = 0, j = 0, qtde;
	char palavra[15];
	Lista *no = NULL;
	while(frase[i] != '\0')
	{
		if (frase[i] != ' ')
		{
			palavra[j] = frase[i];
			j++;
		}
		else // achei uma palavra e um espaco
		{
				
			if (j > 0)
			{
				palavra[j] = '\0';
				buscarPalavra(palavra, L, &no);
				if (no != NULL)
				{
					strcat(fraseCodificada, no->reg.codigo);
				}
				palavra[0] = '\0';
				j = 0;
			}
			buscarPalavra(" ",L,&no);
			if (no != NULL)
			{
				strcat(fraseCodificada, no->reg.codigo);
			}
		}
		
		i++;
	}
	if (j > 0)
	{
		palavra[j] = '\0';
		buscarPalavra(palavra, L, &no);
		if (no != NULL)
		{
			strcat(fraseCodificada, no->reg.codigo);
		}
	}
	
	while(strlen(fraseCodificada)%8 != 0)
	{
		buscarPalavra(" ", L, &no);
		strcat(fraseCodificada, no->reg.codigo);
	}
	
}
void armazenarFraseCodificada(char *fraseCodificada)
{	
	FILE * ptr = fopen("fraseCodificada.dat", "wb");
	if (ptr == NULL)
		printf("Erro de Abertura\n");
	else
	{
		union byte Byte;
		int i = 0, j = 0;
		rewind(ptr);
		while(fraseCodificada[i] != '\0')
		{
			Byte.bi.b0 = fraseCodificada[i] - '0'; // converte caractere para numero
			Byte.bi.b1 = fraseCodificada[i+1] - '0';
			Byte.bi.b2 = fraseCodificada[i+2] - '0';
			Byte.bi.b3 = fraseCodificada[i+3] - '0';
			Byte.bi.b4 = fraseCodificada[i+4] - '0';
			Byte.bi.b5 = fraseCodificada[i+5] - '0';
			Byte.bi.b6 = fraseCodificada[i+6] - '0';
			Byte.bi.b7 = fraseCodificada[i+7] - '0';
			i = i + 8;
			fwrite(&Byte.num, sizeof(unsigned char),1, ptr);
			
		}
		
		
	}
	fclose(ptr);
}
void exibirFraseCodificada(void)
{
	FILE * ptr = fopen("fraseCodificada.dat", "rb");
	if (ptr == NULL)
		printf("Erro de Abertura\n");
	else
	{
		union byte Byte;
		int i = 0, j = 0;
		rewind(ptr);
		fread(&Byte.num, sizeof(unsigned char),1, ptr);
		while(!feof(ptr))
		{
			printf("%d", Byte.bi.b0);
			printf("%d", Byte.bi.b1);
			printf("%d", Byte.bi.b2);
			printf("%d", Byte.bi.b3);
			printf("%d", Byte.bi.b4);
			printf("%d", Byte.bi.b5);
			printf("%d", Byte.bi.b6);
			printf("%d", Byte.bi.b7);			
			fread(&Byte.num, sizeof(unsigned char),1, ptr);
		}
			
	}
	fclose(ptr);
}
void exibirLista(Lista *L)
{
    if (L != NULL)
    {	
		printf("---------------------------------------------------------------\n");
	 	printf("                            Registros                          \n");
		printf("---------------------------------------------------------------\n");
        printf("\n%-15s%-12s%-10s\t%-s\n", "Simbolo", "Palavra", "Frequencia", "Codigo");

		while (L != NULL)
		{
		    printf("%-15d%-12s%-10.2f\t%s\n",
		           L->reg.simbolo, 
		           L->reg.palavra, 
		           L->reg.freq, 
		           L->reg.codigo);
		
		    L = L->prox;
		}

    }
    else
		printf("Lista Vazia");
    
}
void exibirFloresta(Floresta *F)
{
	while(F != NULL)
	{
		printf ("Simbolo: %d\nFrequencia: %.2f\n", F->no->simbolo, F->no->freq);
		F = F->prox;
	}
}
void exibeh(Tree *raiz, int n)
{	
	//static int n = -1;
	if (raiz != NULL)
	{
		n++;
		exibeh(raiz->dir, n+1);
		for (int i =0; i < 5*n; i++)
			printf (" ");
		printf ("(%d,%.2f)\n",raiz->simbolo,raiz->freq);
		exibeh(raiz->esq, n+1);
		//n--; 
		
	}
	
}
void exibirArqBin(void)
{
	FILE * ptr = fopen("registro.dat", "rb");
	if (ptr == NULL)
		printf("Erro de Abertura\n");
	else
	{
		rewind(ptr);
		Reg reg;
		fread(&reg, sizeof(reg), 1, ptr);
		
		while(!feof(ptr))
		{
		  printf("\nSimbolo: %d\n", reg.simbolo);
		  printf("Palavra: %s\n", reg.palavra);
		  printf("Frequencia: %.2f\n", reg.freq);
		  printf("Codigo: %s\n", reg.codigo);
		  fread(&reg, sizeof(reg), 1, ptr);
		}
		
	}
	fclose(ptr);
}

int main(void)
{
	char frase[1024], fraseNova[1024];
	strcpy(frase,"na imensidao das possibilidades que a vida nos oferece somos frequentemente desafiados a enxergar alem do obvio a acreditar que mesmo os momentos mais dificeis carregam em si licoes valiosas cada queda cada duvida e cada obstaculo sao na verdade degraus invisiveis que nos conduzem ao nosso verdadeiro potencial e preciso coragem para persistir sabedoria para aprender e fe para continuar pois e na caminhada e nao apenas no destino que descobrimos o que realmente nos torna unicos e capazes de transformar sonhos em realidade");
	strcpy(fraseNova,"somos frequentemente desafiados a acreditar que mesmo na imensidao das possibilidades que a vida nos oferece os momentos mais dificeis carregam em si licoes valiosas cada obstaculo cada queda e cada duvida sao degraus invisiveis que nos conduzem ao nosso verdadeiro potencial e preciso coragem para persistir fe para continuar e sabedoria para aprender pois descobrimos o que realmente nos torna unicos e capazes de transformar sonhos em realidade na caminhada e nao apenas no destino");
	char fraseCodificada[1024] = "";
	Lista *L = NULL;
	Floresta *F = NULL;
	int contPalavras = 0;
	interpretarFrase(frase, &L, &contPalavras);
	calcularFreq(L, contPalavras);
	
	construirFloresta(&F, L);
	//exibirFloresta(F);
	
	construirArvoreHuffman(&F);
	
	gerarCodigosDeHuffman(F->no, L);
	
	gravarArqBin(L);
	
	codificarFrase(fraseNova, fraseCodificada,L);
	armazenarFraseCodificada(fraseCodificada);
	
	//printf("\nFrase Codificada: %s\n", fraseCodificada);
	//printf("\nFrase Armazenada: ");
	//exibirFraseCodificada();
	
	//exibirArqBin();
	printf("----------------------------------------------------------------------------------\n");
 	printf("                            Arvore no Formato Horizontal                          \n");
	printf("----------------------------------------------------------------------------------\n\n");
	exibeh(F->no, 0);
	exibirLista(L);
	return 0;
}
