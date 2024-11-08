#include <stdio.h>
#include <stdlib.h>
#include "arvoreavl.h"

int altura(No *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

No* novoNo(int chave) {
    No* nodo = malloc(sizeof(No));
    nodo->chave = chave;
    nodo->esquerda = NULL;
    nodo->direita = NULL;
    nodo->altura = 1;
    nodo->medicoes = NULL;  // Inicializa a lista de medições como vazia
    return nodo;
}

No *rotacaoDir(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

No *rotacaoEsq(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

int fatorBalanc(No *N) {
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

No* inserirNo(No* raiz, int chave, double temperatura, double vibracao, double pressao) {
    if (raiz == NULL) {
        No* novo = novoNo(chave);
        adicionarMedicao(novo, temperatura, vibracao, pressao);
        return novo;
    }

    if (chave < raiz->chave)
        raiz->esquerda = inserirNo(raiz->esquerda, chave, temperatura, vibracao, pressao);
    else if (chave > raiz->chave)
        raiz->direita = inserirNo(raiz->direita, chave, temperatura, vibracao, pressao);
    else {
        adicionarMedicao(raiz, temperatura, vibracao, pressao);
        return raiz;
    }

    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));
    int balanceamento = fatorBalanc(raiz);

    if (balanceamento > 1 && chave < raiz->esquerda->chave)
        return rotacaoDir(raiz);
    if (balanceamento < -1 && chave > raiz->direita->chave)
        return rotacaoEsq(raiz);
    if (balanceamento > 1 && chave > raiz->esquerda->chave) {
        raiz->esquerda = rotacaoEsq(raiz->esquerda);
        return rotacaoDir(raiz);
    }
    if (balanceamento < -1 && chave < raiz->direita->chave) {
        raiz->direita = rotacaoDir(raiz->direita);
        return rotacaoEsq(raiz);
    }

    return raiz;
}

No *noDeValorMinimo(No *nodo) {
    No* atual = nodo;
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

Medicao* novaMedicao(double temperatura, double vibracao, double pressao) {
    Medicao* medicao = malloc(sizeof(Medicao));
    medicao->temperatura = temperatura;
    medicao->vibracao = vibracao;
    medicao->pressao = pressao;
    medicao->prox = NULL;
    return medicao;
}

void adicionarMedicao(No* sensor, double temperatura, double vibracao, double pressao) {
    Medicao* nova = novaMedicao(temperatura, vibracao, pressao);
    nova->prox = sensor->medicoes;
    sensor->medicoes = nova;
}

double calcularMediaMedicoes(No* sensor) {
    if (sensor->medicoes == NULL) {
        return 0.0;
    }

    double soma = 0.0;
    int count = 0;
    Medicao* atual = sensor->medicoes;

    while (atual != NULL) {
        soma += (atual->temperatura + atual->vibracao + atual->pressao) / 3.0;
        count++;
        atual = atual->prox;
    }

    return (count > 0) ? (soma / count) : 0.0;
}

double maxMedicao(No* sensor) {
    double max_valor = sensor->medicoes->temperatura;
    Medicao* atual = sensor->medicoes;

    while (atual != NULL) {
        if (atual->temperatura > max_valor) max_valor = atual->temperatura;
        if (atual->vibracao > max_valor) max_valor = atual->vibracao;
        if (atual->pressao > max_valor) max_valor = atual->pressao;
        atual = atual->prox;
    }

    return max_valor;
}

double minMedicao(No* sensor) {
    double min_valor = sensor->medicoes->temperatura;
    Medicao* atual = sensor->medicoes;

    while (atual != NULL) {
        if (atual->temperatura < min_valor) min_valor = atual->temperatura;
        if (atual->vibracao < min_valor) min_valor = atual->vibracao;
        if (atual->pressao < min_valor) min_valor = atual->pressao;
        atual = atual->prox;
    }

    return min_valor;
}
