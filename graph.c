/**
 * @author: Leonardo de Assis / Guilherme Pontes
 * @date: 04/05/2016
 *
 * Clique maximo: sage / greedy
 *  1 -> 11 / 11
 *  2 -> 22 / 22
 *  3 ->  4 /  3
 *  4 ->  8 /  6
 *  5 -> 13 / 11
 *  6 ->  7 /  5
 *  7 ->  9 /  9
 *  8 -> 44 / 44
 *  9 ->  5 /  4
 * 10 ->  3 /  2
 * 11 ->  5 /  4
 * 12 ->  4 /  3
 * 13 ->  4 /  4
 * 14 -> 32 / 32
 * 15 ->  4 /  2
 * 16 -> 57 / 57
 * 17 ->  4 /  2
 * 18 -> 26 / 26
 * 19 ->  4 /  2
 * 20 -> 23 / 20
 * 21 ->  4 /
 * 22 -> 20 /
 * 23 -> 37 /
 * 24 ->  4 /
 * 25 -> 23 /
 *
 * Grafos:
 * 1.Airlines.csv
 * 2.USAir.csv
 * 3.Codeminer.csv
 * 4.CpanAuthors.csv
 * 5.EuroSis.csv
 * 6.Oclinks.csv
 * 7.YeastS.csv
 * 8.CA-GrQc.csv
 * 9.p2p-Gnutella08.csv
 * 10.Wiki-Vote1.csv
 * 11.p2p-Gnutella09.csv
 * 12.p2p-Gnutella06.csv
 * 13.p2p-Gnutella05.csv
 * 14.CA-HepTh.csv
 * 15.p2p-Gnutella04.csv
 * 16.CA-AstroPh.csv
 * 17.p2p-Gnutella25.csv
 * 18.CA-CondMat.csv
 * 19.p2p-Gnutella24.csv
 * 20.Cit-HepTh.csv
 * 21.p2p-Gnutella30.csv
 * 22.Email-Enron.csv
 * 23.Brightkite_edges.csv
 * 24.p2p-Gnutella31.csv
 * 25.soc-Epinions1.csv
 *
 **/

#define FILENAME "GrafosOriginais/17.p2p-Gnutella25.csv"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	Linha* *v = malloc(sizeof(Linha)*tam_vetor);
	elementos_vetor = 0;

	criar_grafo(v);
	quicksort(v, 0, elementos_vetor-1);
	printf("Clique maximo: %d\n", clique_maximo(v));
	free(v);

	return 0;
}

void criar_grafo(Linha* *v){
	FILE* stream = fopen(FILENAME, "r");
    char line[50];
    int a, b;

    while(fgets(line, 50, stream)){
        a = b = -1;
        sscanf(line, "%d,%d", &a, &b);
        //printf("%d %d \n", a, b);
        if(a != b && a != -1 && b != -1)
        	inserir_aresta(v, a, b);
    }

    fclose(stream);
}

int contar_linhas(){
	FILE* stream = fopen(FILENAME, "r");
    char line[50];
    int linhas = 0;

    while(fgets(line, 50, stream))
    	linhas++;

    fclose(stream);

	return linhas;
}

void inserir_aresta(Linha* *v, int a, int b){
	a = encontrar_linha(v, a);
	b = encontrar_linha(v, b);

	if(a == b) ++a;

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
				v[i] = v[i-1];
			*posicao = 0;
		}
		else if(value > v[elementos_vetor-1]->value)
			*posicao = elementos_vetor;
		else
			for(i = elementos_vetor-1; value < v[i]->value; i--){
				v[i+1] = v[i];
				*posicao = i;
			}
	}
	else
		*posicao = 0;

	v[*posicao] = node;
	++elementos_vetor;	
}

Coluna* inserir_coluna(Linha* *v, int dest, int src){  
	Coluna *aux = v[dest]->edges;
	
	if(aux != NULL){
		while(v[src]->value > aux->edge->linha->value && aux->next != NULL)
			aux = aux->next;
		
		if(v[src]->value == aux->edge->linha->value){
			v[dest]->degree--;
			return aux;
		}
		else if(v[src]->value < aux->edge->linha->value){
			Coluna *new = criar_coluna(v[dest]);
			new->prev = aux->prev;
			new->next = aux;
			aux->prev = new;

			if(new->prev != NULL)
				new->prev->next = new;
			else
				v[dest]->edges = new;

			return new;
		}
		else{
			Coluna *new = criar_coluna(v[dest]);
			new->prev = aux;
			new->next = aux->next;

			if(new->next != NULL)
				new->next->prev = new;

			if(new->prev != NULL)
				new->prev->next = new;
			else
				v[dest]->edges = new;

			return new;
		}
	}		
	else{
		v[dest]->edges = criar_coluna(v[dest]);
		return v[dest]->edges;
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
        if (v[i]->degree > v[r]->degree){
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
		quicksort(v, p, j-1);
		quicksort(v, j+1, r);
    }
}

int clique_maximo(Linha* *v){
	int clique = elementos_vetor;

	/*int i;
	for(i = 0; i < elementos_vetor; i++){
		printf("\n\n%d - %d: ", v[i]->value, v[i]->degree);
		Coluna *aux = v[i]->edges;
		while(aux != NULL){
			printf("%d ", aux->edge->linha->value);
			aux = aux->next;
		}
	}*/

	while(clique > 1 && verificar_clique(v, clique) == 0){
		//printf("%d\n", clique);
		remover_no(v, v[elementos_vetor-1]);
		quicksort(v, 0, elementos_vetor-1);
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
	edge->linha->degree--;

	if(edge->prev == NULL)
		edge->linha->edges = edge->next;
	else
		edge->prev->next = edge->next;

	if(edge->next != NULL)
		edge->next->prev = edge->prev;

	free(edge);
}
