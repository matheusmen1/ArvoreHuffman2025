struct tree{
	int simbolo;
	float freq;
	tree *dir, *esq;
};
typedef struct tree Tree;

struct pilha{
	
	Tree *raiz;
	char codigo[9];
	struct pilha *prox;
	
};
typedef struct pilha Pilha;
void init(Pilha **p)
{
	*p = NULL;
}
void push(Pilha **p, Tree *raiz, char *codigo)
{
	Pilha *NC;
	NC = (Pilha*)malloc(sizeof(Pilha));
	strcpy(NC->codigo, codigo);
	NC->prox = NULL;
	NC->raiz = raiz;
	if(*p == NULL)
		*p = NC;
	else
	{
		NC->prox = *p;
		*p = NC;
	} 
	
}
void pop(Pilha **P, Tree **x, char *codigo)
{
	
	Pilha *aux;
	aux = *P;
	strcpy(codigo, aux->codigo);
	*x = (*P)->raiz ;
	*P = (*P)->prox;
	free(aux);
	
}
char isEmpty(Pilha *p){
	
	return p == NULL;
}
