/**
 * @author: Leonardo de Assis / Guilherme Pontes
 * @date: 04/05/2016
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>

int tam_vetor, elementos_vetor;

typedef struct item {
	int value;
	struct item *next;

} Item;

int contar_distintos();
void inserir_aresta(Item* *v, int a, int b);
int encontrar_linha(Item* *v, int value);
int encontrar_coluna(Item* *v, int value);
void inserir_linha(Item* *v, int value, int posicao);
Item* inserir_coluna(Item* line, Item *ptr);
Item* copiar_no(Item* ptr);


int main(){
	elementos_vetor = 0;
	tam_vetor = contar_distintos();

	Item* *vetor_linhas = malloc(sizeof(Item*)*tam_vetor);

	return 0;
}

int contar_distintos(){
	int distintos = 0;

	return distintos;
}

void inserir_aresta(Item* *v, int a, int b){
	a = encontrar_linha(v, a);
	b = encontrar_linha(v, b);

	v[a] = inserir_coluna(v[a], v[b]);
	v[b] = inserir_coluna(v[b], v[a]);
}

int encontrar_linha(Item* *v, int value){
	int posicao = encontrar_coluna(v, value);
	
	if(posicao  == -1)
		inserir_linha(v, value, posicao);

	return posicao;
}

int encontrar_coluna(Item* *v, int value){
	int esq = 0;
	int dir = tam_vetor-1;

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

void inserir_linha(Item* *v, int value, int posicao){
	int i;

	Item *node = malloc(sizeof(Item));
	node->value = value;
	node->next = NULL;
	
	for(i = elementos_vetor; i > posicao; i--)
		v[i+1] = v[i];
	
	v[posicao] = node;
	elementos_vetor++;	
}

Item* inserir_coluna(Item *line, Item *ptr){
	Item *previous, *aux = line;

	if(aux->next == NULL){
		aux->next = copiar_no(ptr);
		return aux;
	}

	while(aux->next != NULL && aux->value < ptr->value){
		previous = aux;
		aux = aux->next;
	}

	if(aux->next == NULL)
		aux->next = copiar_no(ptr);
	else{
		ptr->next = previous->next;
		previous->next = ptr;
	}

	return line;
}

Item* copiar_no(Item *ptr){
	Item *node = malloc(sizeof(Item));
	node->value = ptr->value;
	node->next = ptr->next;

	return node;
}
