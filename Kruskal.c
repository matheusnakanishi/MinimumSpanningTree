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

//Edge Struct
typedef struct no{
    int x, y, peso;
    struct no *prox;
} No;

typedef struct {
    int nArestas;
    No *a;
} Aresta;

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

void ordenar_arestas(Aresta *arestas, int x, int y, int peso) {
    No *nova = (No*)malloc(sizeof(No));
    nova->x = x;
    nova->y = y;
    nova->peso = peso;

    No *aux = arestas->a;

    if (!aux || nova->peso < aux->peso) {
        nova->prox = aux;
        arestas->a = nova;
    }
    else {
        while (aux->prox && aux->prox->peso < nova->peso)
            aux = aux->prox;
        
        nova->prox = aux->prox;
        aux->prox = nova;
    }
    
}

void inserir_aresta(Grafo *grafo, int origem, int destino, int peso, Aresta *arestas, bool ordenar) {
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

    if (ordenar)
        ordenar_arestas(arestas, origem, destino, peso);
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

//Kruskal Functions
void inicializar_Kruskal(bool *conjunto) {
    for (int i = 0; i < N; i++) {
        conjunto[i] = true;
    }
}

bool resta_aresta(bool *conjunto) {
    for (int i = 0; i < N; i++) {
        if (conjunto[i])
            return true;
    }
    return false;
}

Grafo *Kruskal(Aresta *arestas) {
    bool *conjunto = (bool*)malloc(N * sizeof(bool));
    Grafo *AGM = inicializar_grafo(N);

    inicializar_Kruskal(conjunto);

    No *aux = arestas->a;
   
    while (resta_aresta(conjunto)) {
        int A = aux->x;
        int B = aux->y;
        int peso = aux->peso;
        
        if (conjunto[A] || conjunto[B]) {
            inserir_aresta(AGM, A, B, peso, NULL, false);
            conjunto[A] = conjunto[B] = false;
        }
        aux = aux->prox;
    }
    free(conjunto);
    return AGM;
}

int main() {
    Grafo *grafo = inicializar_grafo(N);
    Aresta *arestas = (Aresta*)malloc(sizeof(Aresta));
    arestas->a = NULL;

    inserir_aresta(grafo, 0, 1, 3, arestas, true);
    inserir_aresta(grafo, 0, 2, 5, arestas, true);
    inserir_aresta(grafo, 1, 3, 2, arestas, true);
    inserir_aresta(grafo, 0, 3, 8, arestas, true);
    inserir_aresta(grafo, 0, 4, 9, arestas, true);
    inserir_aresta(grafo, 3, 4, 1, arestas, true);
    inserir_aresta(grafo, 4, 2, 2, arestas, true);
    inserir_aresta(grafo, 2, 3, 8, arestas, true);
    

    Grafo *AGM = Kruskal(arestas);
    printf("Minimum Spanning Tree");
    imprimir_grafo(AGM);

    liberar_grafo(grafo);
    liberar_grafo(AGM);

    return 0;
}