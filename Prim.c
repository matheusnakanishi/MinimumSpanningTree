#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define N 5

//Graph Structs
typedef struct adjacencia {
    int vertice, peso;
    struct adjacencia *prox;
} Adj;

typedef struct {
    Adj *cab;
} Vertice;

typedef struct grafo {
    int vertices, arestas;
    Vertice *adj;
} Grafo;

//Graph Functions
Grafo* inicializar_grafo(int vertices) {
    Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));

    grafo->vertices = vertices;
    grafo->arestas = 0;

    grafo->adj = (Vertice*)malloc(vertices * sizeof(Vertice));  //List vector alocation

    for (int i = 0; i < vertices; i++) 
        grafo->adj[i].cab = NULL;
    
    return grafo;
}

void inserir_aresta(Grafo *grafo, int origem, int destino, int peso) {
    Adj *aresta = (Adj*)malloc(sizeof(Adj));

    aresta->vertice = destino;
    aresta->peso = peso;
    aresta->prox = grafo->adj[origem].cab;
    grafo->adj[origem].cab = aresta;

    Adj *aresta2 = (Adj*)malloc(sizeof(Adj));
    aresta2->vertice = origem;
    aresta2->peso = peso;
    aresta2->prox = grafo->adj[destino].cab;
    grafo->adj[destino].cab = aresta2;

    grafo->arestas++;
}

void imprimir_grafo(Grafo *grafo) {
    for (int i = 0; i < grafo->vertices; i++) {
        printf("\n[%d]: ", i);

        Adj *aux = grafo->adj[i].cab;
        while (aux) {
            printf("(%d|%d)->", aux->vertice, aux->peso);
            aux = aux->prox;
        }
    }
}

void liberar_grafo(Grafo *grafo) {
    for (int i = 0; i < grafo->vertices; i++) {
        Adj *aux = grafo->adj[i].cab;

        while (aux) {
            Adj *delete = aux;
            aux = aux->prox;
            free(delete);
        }
    }

    free(grafo->adj);
    free(grafo);
}

//Prim Functions
void inicializar_Prim(Grafo *grafo, int *key, int *pai, bool *aberto, int s) {
    for (int i = 0; i < grafo->vertices; i++) {
        key[i] = INT_MAX/2;
        pai[i] = -1;
        aberto[i] = true;
    }
    key[s] = 0;
}

bool existe_aberto(Grafo *grafo, bool *aberto) {
    for (int i = 0; i < grafo->vertices; i++) {
        if (aberto[i])
            return true;
    }
    return false;
}

int menor_key(Grafo *grafo, bool *aberto, int *key) {
    int i;

    for (i = 0; i < grafo->vertices; i++) {
        if (aberto[i])
            break;
    }

    if (i == grafo->vertices)
        return -1;

    int menor = i;
    
    for (i = menor + 1; i < grafo->vertices; i++) {
        if (aberto[i] && (key[menor] > key[i]))
            menor = i;
    }
    
    return menor;
}

void gerar_AGM(Grafo *AGM, int *key, int *pai) {
    for (int i = 1; i < AGM->vertices; i++) {
        inserir_aresta(AGM, i, pai[i], key[i]);
    }
}

Grafo *Prim(Grafo *grafo, int s) {
    int *key = (int*)malloc(grafo->vertices * sizeof(int));
    int *pai = (int*)malloc(grafo->vertices * sizeof(int));
    bool *aberto = (bool*)malloc(grafo->vertices * sizeof(bool));
    
    inicializar_Prim(grafo, key, pai, aberto, s);

     while (existe_aberto(grafo, aberto)) {
        int u = menor_key(grafo, aberto, key);
        aberto[u] = false;
        Adj *adj = grafo->adj[u].cab;

        while (adj) {
            if (aberto[adj->vertice] && adj->peso < key[adj->vertice]) {
                pai[adj->vertice] = u;
                key[adj->vertice] = adj->peso;
            }
            
            adj = adj->prox;
        }  
    }

    Grafo *AGM = inicializar_grafo(N);
    gerar_AGM(AGM, key, pai);

    free(key);
    free(pai);
    free(aberto);

    return AGM;
}

int main() {
    Grafo *grafo = inicializar_grafo(N);

    inserir_aresta(grafo, 0, 1, 3);
    inserir_aresta(grafo, 0, 2, 5);
    inserir_aresta(grafo, 1, 3, 2);
    inserir_aresta(grafo, 0, 3, 8);
    inserir_aresta(grafo, 0, 4, 9);
    inserir_aresta(grafo, 3, 4, 1);
    inserir_aresta(grafo, 4, 2, 2);
    inserir_aresta(grafo, 2, 3, 8);
    

    Grafo *AGM = Prim(grafo, 0);
    printf("Minimum Spanning Tree");
    imprimir_grafo(AGM);

    liberar_grafo(grafo);
    liberar_grafo(AGM);

    return 0;
}