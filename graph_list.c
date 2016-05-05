/**
 * @author: Leonardo de Assis / Guilherme Pontes
 * @date: 04/05/2016
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>

int tam_vetor, elementos_vetor;

typedef struct coluna {
	int value;
	struct coluna *prev, *next, *edge;

} Coluna;

typedef struct linha {
	int value;
	int degree;
	struct coluna *edges;

} Linha;

int contar_distintos();
void inserir_aresta(Linha* *v, int a, int b);
int encontrar_linha(Linha* *v, int value);
int encontrar_posicao(Linha* *v, int value);
void inserir_linha(Linha* *v, int value, int posicao);
Coluna* inserir_coluna(Linha** v, int dest, int src);
Coluna* criar_coluna(int value);


int main(){
	elementos_vetor = 0;
	tam_vetor = contar_distintos();

	Linha* *vetor_linhas = malloc(sizeof(Linha)*tam_vetor);

	return 0;
}

int contar_distintos(){
	int distintos = 0;

	return distintos;
}

void inserir_aresta(Linha* *v, int a, int b){
	a = encontrar_linha(v, a);
	b = encontrar_linha(v, b);

	Coluna *ptr_a = inserir_coluna(v, a, b);
	Coluna *ptr_b = inserir_coluna(v, b, a);

	ptr_a->edge = ptr_b;
	ptr_b->edge = ptr_a;

	v[a]->degree++;
	v[b]->degree++;
}

int encontrar_linha(Linha* *v, int value){
	int posicao = encontrar_posicao(v, value);
	
	if(posicao  == -1)
		inserir_linha(v, value, posicao);

	return posicao;
}

int encontrar_posicao(Linha* *v, int value){
	int esq = 0;
	int dir = elementos_vetor-1;

	while(esq < dir) {
		int i = (esq + dir) / 2;

		if(value < v[i]->value)
			dir = i - 1;
		else if(value > v[i]->value)
			esq = i + 1;
		else
			return i;
	}
	return -1;
}

void inserir_linha(Linha* *v, int value, int posicao){
	int i;

	Linha *node = malloc(sizeof(Linha));
	node->value = value;
	node->degree = 0;
	node->edges = NULL;
	
	for(i = elementos_vetor; i > posicao; i--)
		v[i+1] = v[i];
	
	v[posicao] = node;
	elementos_vetor++;	
}

Coluna* inserir_coluna(Linha* *v, int dest, int src){
	Coluna *aux = v[dest]->edges;

	if(aux == NULL){
		Coluna *new = criar_coluna(v[src]->value);
		new->prev = aux;
		aux->next = new;
	}
	else{
		while(aux->next != NULL && aux->value < v[src]->value)
			aux = aux->next;
		
		if(aux->next == NULL){
			Coluna *new = criar_coluna(v[src]->value);
			new->prev = aux;
			aux->next = new;
		}
		else{
			Coluna *new = criar_coluna(v[src]->value);
			new->prev = aux->prev;
			new->next = aux;
			new->prev->next = new;
			new->next->prev = new;
		}
	}

	return aux;
}

Coluna* criar_coluna(int value){
	Coluna *node = malloc(sizeof(Coluna));
	node->value = value;
	node->prev = node->next = node->edge = NULL;

	return node;
}
