#include <stdio.h>
#include <stdlib.h>
#include "arvoreavl.h"

typedef struct No {
    int chave;
    struct No *esquerda;
    struct No *direita;
    int altura;
} No;

int altura(No *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

No* novoNo(int chave) {
    No* No = (No*)malloc(sizeof(No));
    No->chave = chave;
    No->esquerda = NULL;
    No->direita = NULL;
    No->altura = 1;
    return No;
}

No *rotacaoDir(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    // Rotaciona
    x->direita = y;
    y->esquerda = T2;

    // Atualiza as alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

No *rotacaoEsq(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    // Atualiza as alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

int fatorBalanc(No *N) {
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

No* inserirNo(No* No, int chave) {
    if (No == NULL)
        return novoNo(chave);
    if (chave < No->chave)
        No->esquerda = inserirNo(No->esquerda, chave);
    else if (chave > No->chave)
        No->direita = inserirNo(No->direita, chave);
    else // Chaves duplicadas não são permitidas
        return No;

    // Atualiza a altura deste ancestral
    No->altura = 1 + max(altura(No->esquerda), altura(No->direita));

    // fator de balanceamentoamento deste ancestral
    int balanceamento = fatorBalanc(No);

    // Se este nó se tornar desbalanceamentoado, então há 4 casos

    // Caso Esquerda-Esquerda
    if (balanceamento > 1 && chave < No->esquerda->chave)
        return rotacaoDir(No);

    // Caso Direita-Direita
    if (balanceamento < -1 && chave > No->direita->chave)
        return rotacaoEsq(No);

    // Caso Esquerda-Direita
    if (balanceamento > 1 && chave > No->esquerda->chave) {
        No->esquerda = rotacaoEsq(No->esquerda);
        return rotacaoDir(No);
    }

    // Caso Direita-Esquerda
    if (balanceamento < -1 && chave < No->direita->chave) {
        No->direita = rotacaoDir(No->direita);
        return rotacaoEsq(No);
    }

    return No;
}

// Função para encontrar o nó com o valor mínimo
No *noDeValorMinimo(No *No) {
    No *atual = No;
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

No* removerNo(No* raiz, int chave) {
    if (raiz == NULL)
        return raiz;
    if (chave < raiz->chave)
        raiz->esquerda = removerNo(raiz->esquerda, chave);
    else if (chave > raiz->chave)
        raiz->direita = removerNo(raiz->direita, chave);
    else {
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            No *temp = noDeValorMinimo(raiz->direita);
            raiz->chave = temp->chave;
            raiz->direita = removerNo(raiz->direita, temp->chave);
        }
    }

    if (raiz == NULL)
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

    int balanceamento = fatorBalanc(raiz);

    if (balanceamento > 1 && fatorBalanc(raiz->esquerda) >= 0)
        return rotacaoDir(raiz);

    if (balanceamento > 1 && fatorBalanc(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsq(raiz->esquerda);
        return rotacaoDir(raiz);
    }

    if (balanceamento < -1 && fatorBalanc(raiz->direita) <= 0)
        return rotacaoEsq(raiz);

    if (balanceamento < -1 && fatorBalanc(raiz->direita) > 0) {
        raiz->direita = rotacaoDir(raiz->direita);
        return rotacaoEsq(raiz);
    }
    return raiz;
}

No* buscarNo(No* raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;
    if (chave < raiz->chave)
        return buscarNo(raiz->esquerda, chave);
    return buscarNo(raiz->direita, chave);
}

void imprimirArvore(No *raiz) {
    if (raiz != NULL) {
        imprimirArvore(raiz->esquerda);
        printf("%d ", raiz->chave);
        imprimirArvore(raiz->direita);
    }
}

// Função principal
int main() {
    No *raiz = NULL;

    // Inserções
    raiz = inserirNo(raiz, 10);
    raiz = inserirNo(raiz, 20);
    raiz = inserirNo(raiz, 30);
    raiz = inserirNo(raiz, 40);
    raiz = inserirNo(raiz, 50);
    raiz = inserirNo(raiz, 25);

    // Impressão da árvore em ordem
    printf("Árvore em ordem: ");
    imprimirArvore(raiz);
    printf("\n");

    // Remoção de um nó
    raiz = removerNo(raiz, 30);
    printf("Árvore após remoção de 30: ");
    imprimirArvore(raiz);
    printf("\n");

    // Busca de um nó
    No *result = buscarNo(raiz, 20);
    if (result != NULL) {
        printf("Nó encontrado: %d\n", result->chave);
    } else {
        printf("Nó não encontrado.\n");
    }

    return 0;
}
