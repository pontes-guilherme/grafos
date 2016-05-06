/**
 * @author: Leonardo de Assis / Guilherme Pontes
 * @date: 04/05/2016
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "GrafosOriginais/1.Airlines.csv"

int elementos_vetor;

typedef struct coluna {
	struct linha *linha;
	struct coluna *prev, *next, *edge;

} Coluna;

typedef struct linha {
	int value;
	int degree;
	struct coluna *edges;

} Linha;

void criar_grafo(Linha* *v);
int contar_linhas();
void inserir_aresta(Linha* *v, int a, int b);
int encontrar_linha(Linha* *v, int value);
int encontrar_posicao(Linha* *v, int value);
void inserir_linha(Linha* *v, int value, int *posicao);
Coluna* inserir_coluna(Linha** v, int dest, int src);
Coluna* criar_coluna(Linha* linha);
void swap(Linha* *v, int i, int j);
int partition(Linha* *v, int p, int r, int q);
void quicksort(Linha* *v, int p, int r);
int clique_maximo(Linha* *v);
int verificar_clique(Linha* *v, int clique);
void remover_no(Linha* *v, Linha* no);
void remover_aresta(Coluna* edge);


int main(){
	
	int tam_vetor = contar_linhas() * 2;

	Linha* *vetor_linhas = malloc(sizeof(Linha)*tam_vetor);
	elementos_vetor = 0;

	criar_grafo(vetor_linhas);
	quicksort(vetor_linhas, 0, elementos_vetor-1);
	printf("CLique maximo: %d\n", clique_maximo(vetor_linhas));

	return 0;
}

void criar_grafo(Linha* *v){
	FILE* stream = fopen(FILENAME, "r");
    char line[50];
    int a, b;
int i = 0;
    while(fgets(line, 50, stream)){
        a = b = -1;
        sscanf(line, "%d,%d", &a, &b);
        if(a == 50 || b == 50) i++;
        if(a != -1 && b != -1)
        	inserir_aresta(v, a, b);
    }
   printf("%d\n", i);
    fclose(stream);
}

int contar_linhas(){
	FILE* stream = fopen(FILENAME, "r");
    char line[50];
    int linhas = 0;

    while(fgets(line, 50, stream)){
    	linhas++;
    }

    fclose(stream);

	return linhas;
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
		inserir_linha(v, value, &posicao);

	return posicao;
}

int encontrar_posicao(Linha* *v, int value){
	int esq = 0;
	int dir = elementos_vetor-1;

	while(esq <= dir) {
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

void inserir_linha(Linha* *v, int value, int *posicao){
	int i;

	Linha *node = malloc(sizeof(Linha));
	node->value = value;
	node->degree = 0;
	node->edges = NULL;

	if(elementos_vetor > 0){
		if(value < v[0]->value){
			for(i = elementos_vetor; i > 0; i--)
				v[i+1] = v[i];

			*posicao = 0;
		}
		else{
			*posicao = elementos_vetor;
		}
	}
	else
		*posicao = 0;

	v[*posicao] = node;
	elementos_vetor++;	
}

Coluna* inserir_coluna(Linha* *v, int dest, int src){  
	if(v[dest]->edges == NULL){
		v[dest]->edges = criar_coluna(v[src]);

		return v[dest]->edges;
	}
	else{
		Coluna *aux = v[dest]->edges;

		while(aux->next != NULL && (aux->linha)->value < v[src]->value)
			aux = aux->next;

		if(aux->next == NULL){
			Coluna *new = criar_coluna(v[src]);
			new->prev = aux;
			aux->next = new;
		}
		else{
			Coluna *new = criar_coluna(v[src]);

			new->prev = aux->prev;
			new->next = aux;
			aux->prev = new;

			if(aux->prev != NULL)
				aux->prev->next = new;
			else
				v[dest]->edges = new;
		}

		return aux;
	}
}

Coluna* criar_coluna(Linha* linha){
	Coluna *node = malloc(sizeof(Coluna));
	node->linha = linha;
	node->prev = node->next = node->edge = NULL;

	return node;
}

void swap(Linha* *v, int i, int j){
    Linha* tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}


int partition(Linha* *v, int p, int r, int q){
    int pos, i;
    swap(v, q, r);
    pos = p;

    for(i = p; i < r; i++){
        if (v[i] < v[r]){
            swap(v, i, pos);
            pos++;
        }
    }
    swap(v, r, pos);
    return pos;
}

void quicksort(Linha* *v, int p, int r){
    if (p < r){
		int j = partition(v, p, r, (p+r)/2);
		quicksort(v, p, j - 1);
		quicksort(v, j + 1, p);
    }
}

int clique_maximo(Linha* *v){
	int clique = elementos_vetor;
	printf("tamanho do grafo: %d\n", elementos_vetor);
	while(verificar_clique(v, clique) == 0 && clique > 1){
		printf("clique: %d\n", clique);
		remover_no(v, v[elementos_vetor-1]);
		quicksort(v, 0, elementos_vetor);
		clique = elementos_vetor;
	}

	return clique;
}

int verificar_clique(Linha* *v, int clique){
	int i;

	for(i = elementos_vetor-1; i >= 0; i--)
		if(v[i]->degree != clique-1)
			return 0;

	return 1;
}

void remover_no(Linha* *v, Linha* no){
	Coluna* aux = no->edges;
	printf("%d %d %d\n", no->value, no->degree, aux->linha->value);		
	while(aux != NULL){
		remover_aresta(aux->edge);

		Coluna* tmp = aux;
		aux = aux->next;

		free(tmp);
	}

	free(no);
	elementos_vetor--;
}

void remover_aresta(Coluna* edge){
	if(edge == NULL)printf("??\n");
	edge->linha->degree--;

	if(edge->prev == NULL)
		edge->linha->edges = edge->next;
	else
		edge->prev->next = edge->next;

	if(edge->next != NULL)
		edge->next->prev = edge->prev;

	free(edge);
}
