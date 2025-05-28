#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio2.h>
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
struct tree
{
	struct tree *dir, *esq;
	int simbolo;
};
typedef struct tree Tree;
struct reg
{
	char palavra[15], codigo[8];
	int simbolo;
	float freq;
};
typedef struct reg Reg;

Tree *CriaNo(int simbolo)
{
	Tree *NC;
	NC = (Tree*)malloc(sizeof(Tree));
	NC->dir = NULL;
	NC->esq = NULL;
	NC->simbolo = simbolo;
	return NC;
	
}
void lerArqFraseCodificada(char *fraseCodificada)
{
	FILE * ptr = fopen("fraseCodificada.dat", "rb");
	if (ptr == NULL)
		printf("Erro de Abertura");
	else
	{
		rewind(ptr);
		union byte Byte;
		int i = 0;
		fread(&Byte.num, sizeof(unsigned char),1, ptr);
		while(!feof(ptr))
		{
			fraseCodificada[i] = Byte.bi.b0 + '0'; // converte numero para caractere
			fraseCodificada[i+1] = Byte.bi.b1 + '0';
			fraseCodificada[i+2] = Byte.bi.b2 + '0';
			fraseCodificada[i+3] = Byte.bi.b3 + '0';
			fraseCodificada[i+4] = Byte.bi.b4 + '0';
			fraseCodificada[i+5] = Byte.bi.b5 + '0';
			fraseCodificada[i+6] = Byte.bi.b6 + '0';
			fraseCodificada[i+7] = Byte.bi.b7 + '0';
			i = i + 8;
			fread(&Byte.num, sizeof(unsigned char),1, ptr);	
		}
	}
	
	fclose(ptr);
	
}
void construirArvoreHuffman(Tree **raiz)
{
	FILE * ptr = fopen("registro.dat", "rb");
	if (ptr == NULL)
		printf("Erro de Abertura\n");
	else
	{
		Reg reg;
		Tree *aux, *ant;
		rewind(ptr);
		fread(&reg, sizeof(Reg),1, ptr);
		while(!feof(ptr))
		{
			int i = 0;
			if (*raiz == NULL)
				*raiz = CriaNo(-1);	
			
			aux = *raiz;
			while(reg.codigo[i] != '\0')
			{
				if(reg.codigo[i] == '0')
				{
					if (reg.codigo[i+1] != '\0')
					{
						if(aux->esq == NULL)
							aux->esq = CriaNo(-1);
				
						aux = aux->esq;
					}
					else
					{
						if(aux->esq == NULL)
							aux->esq = CriaNo(reg.simbolo);
					
						aux = aux->esq;		

					}
					
				}
				else
				{
					if(reg.codigo[i+1] != '\0')
					{
						if(aux->dir == NULL)
							aux->dir = CriaNo(-1);
					
						aux = aux->dir;
							
					}
					else
					{
						if(aux->dir == NULL)
							aux->dir = CriaNo(reg.simbolo);
						
						aux = aux->dir;
					}
						
				}
					
				i++;
			}	
			
			fread(&reg, sizeof(Reg),1, ptr);
		}
	}
	fclose(ptr);
	
}
void decodificarFrase(Tree *raiz, char *fraseCodificada, char *fraseDecodificada)
{
	FILE * ptr = fopen("registro.dat", "rb");
	if (ptr == NULL)
		printf("Erro de Abertura\n");
	else
	{
		Reg reg;
		Tree *aux;
		int i = 0, j = 0;
		while(fraseCodificada[i] != '\0')
		{
			aux = raiz;
			while(aux != NULL && aux->simbolo == -1)
			{
				if (fraseCodificada[i] == '0')
					aux = aux->esq;
				else
					aux = aux->dir;
				i++;
			}
			
			if (aux != NULL)
			{
				rewind(ptr);
				fread(&reg, sizeof(Reg),1, ptr);
				while(!feof(ptr) && reg.simbolo != aux->simbolo)	
					fread(&reg, sizeof(Reg),1, ptr);
					
				if(!feof(ptr))		
					strcat(fraseDecodificada,reg.palavra);
			
				
			}
			
			
		}
		
	}
	fclose(ptr);
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
		printf ("(%d)\n",raiz->simbolo);
		//exibeh(raiz->esq, n+1);
		//n--; 
		
	}
	
}
int main(void)
{
	Tree *raiz;
	raiz = NULL;
	char fraseCodificada[1024] = "";
	char fraseDecodificada[1024] = "";
	lerArqFraseCodificada(fraseCodificada);
	//printf("Frase Codificada: %s", fraseCodificada);
	
	construirArvoreHuffman(&raiz);
	exibeh(raiz,0);
	
	decodificarFrase(raiz, fraseCodificada, fraseDecodificada);
	printf("----------------------------------------------------------------------------------\n");
 	printf("                            Frase Decodificada                                    \n");
	printf("----------------------------------------------------------------------------------\n\n");
	printf("%s\n", fraseDecodificada);
	printf("\n----------------------------------------------------------------------------------");
	return 0;
}
