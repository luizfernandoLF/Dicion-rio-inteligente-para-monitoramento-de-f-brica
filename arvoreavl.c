#include <stdio.h>
#include <stdlib.h>
#include "arvoreavl.h"

int contador_insercao = 0;
int contador_remocao = 0;
int contador_busca = 0;
int contadorMovInsercao = 0;
int contadorMovRemocao = 0;


int altura(No *N) {
    contador_insercao++;
    if (N == NULL)
        return -1;
    return N->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

No* novoNo(int chave, int classID) {
    No* nodo = malloc(sizeof(No));
    nodo->chave = chave;
    nodo->classID = classID;
    nodo->esquerda = NULL;
    nodo->direita = NULL;
    nodo->altura = 1;
    nodo->medicoes = NULL;
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
    contador_insercao;
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

No* inserirNo(No* raiz, int chave, int classID, double temperatura, double vibracao, double pressao) {
    if (raiz == NULL) {
        contador_insercao++;
        No* novo = novoNo(chave, classID);
        adicionarMedicao(novo, temperatura, vibracao, pressao);
        return novo;
    }

    if (chave < raiz->chave) {
        contador_insercao++;
        raiz->esquerda = inserirNo(raiz->esquerda, chave, classID, temperatura, vibracao, pressao);
    } else if (chave > raiz->chave) {
        contador_insercao++;
        raiz->direita = inserirNo(raiz->direita, chave, classID, temperatura, vibracao, pressao);
    } else {
        adicionarMedicao(raiz, temperatura, vibracao, pressao);
        return raiz;
    }

    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));
    int balanceamento = fatorBalanc(raiz);

    if (balanceamento > 1 && chave < raiz->esquerda->chave){
        contadorMovInsercao++;
        return rotacaoDir(raiz);
    }
        
    if (balanceamento < -1 && chave > raiz->direita->chave) {
        contadorMovInsercao++;
        return rotacaoEsq(raiz);
    }
        
    if (balanceamento > 1 && chave > raiz->esquerda->chave) {
        contadorMovInsercao++;
        raiz->esquerda = rotacaoEsq(raiz->esquerda);
        contadorMovInsercao++;
        return rotacaoDir(raiz);
    }
    if (balanceamento < -1 && chave < raiz->direita->chave) {
        contadorMovInsercao++;
        raiz->direita = rotacaoDir(raiz->direita);
        contadorMovInsercao++;
        return rotacaoEsq(raiz);
    }

    return raiz;
}

No *noDeValorMinimo(No *nodo) {
    No* atual = nodo;
    while (atual->esquerda != NULL) {
        contador_remocao++;
        atual = atual->esquerda;
    }
    return atual;
}

No* buscarNo(No* raiz, int chave) {
    contador_busca++;
    while (raiz != NULL) {
        contador_busca++;
        if (chave == raiz->chave) {
            contador_busca++;
            return raiz;
        } else if (chave < raiz->chave) {
            contador_busca++;
            raiz = raiz->esquerda;
        } else {
            raiz = raiz->direita;
        }
    }
    return NULL;
}

No* buscarNoRemocao(No* raiz, int chave) {
    contador_remocao++;
    while (raiz != NULL) {
        contador_remocao++;
        if (chave == raiz->chave) {
            contador_remocao++;
            return raiz;
        } else if (chave < raiz->chave) {
            contador_remocao++;
            raiz = raiz->esquerda;
        } else {
            raiz = raiz->direita;
        }
    }
    return NULL;
}

No* removerNo(No* raiz, int chave) {
    No* pai = NULL;
    No* atual = raiz;

    // Busca o nó a ser removido e seu pai
    while (atual != NULL && atual->chave != chave) {
        pai = atual;
        if (chave < atual->chave) {
            contadorMovRemocao++;
            atual = atual->esquerda;
        } else {
            contadorMovRemocao++;
            atual = atual->direita;
        }
        contador_remocao++;
    }

    // Se o nó não foi encontrado
    if (atual == NULL) {
        return NULL;
    }

    // Caso 1: Nó a ser removido é uma folha
    if (atual->esquerda == NULL && atual->direita == NULL) {
        contador_remocao++;
        if (atual == raiz) {
            contador_remocao++;
            free(atual);
            return NULL; // A árvore fica vazia
        }
        if (pai->esquerda == atual) {
            contador_remocao++;
            pai->esquerda = NULL;
        } else {
            pai->direita = NULL;
        }
    }
    // Caso 2: Nó a ser removido tem um filho
    else if (atual->esquerda == NULL || atual->direita == NULL) {
        contador_remocao++;
        No* filho = (atual->esquerda != NULL) ? atual->esquerda : atual->direita;
        contadorMovRemocao++;
        contador_remocao++;
        if (atual == raiz) {
            contador_remocao++;
            free(atual);
            return filho; // Retorna o filho como nova raiz
        }
        if (pai->esquerda == atual) {
            contador_remocao++;
            contadorMovRemocao++;
            pai->esquerda = filho;
        } else {
            contadorMovRemocao++;
            pai->direita = filho;
        }
    }
    // Caso 3: Nó a ser removido tem dois filhos
    else {
        // Encontra o nó sucessor (menor na subárvore direita)
        No* sucessor = atual->direita;
        No* paiSucessor = atual;

        while (sucessor->esquerda != NULL) {
            contador_remocao++;
            paiSucessor = sucessor;
            contadorMovRemocao++;
            sucessor = sucessor->esquerda;
            contadorMovRemocao++;
        }

        // Copia o valor do sucessor para o nó atual
        atual->chave = sucessor->chave;

        // Remove o sucessor
        if (paiSucessor->esquerda == sucessor) {
            contador_remocao++;
            paiSucessor->esquerda = sucessor->direita;
            contadorMovRemocao++;
        } else {
            paiSucessor->direita = sucessor->direita;
            contadorMovRemocao++;
        }
        atual = sucessor; // Atualiza o nó atual para liberar memória
    }

    free(atual); // Libera a memória do nó removido
    return atual;

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

void imprimirContadores() {
    printf("Funcao de insercao fez %d comparacoes e %d movimentacoes\n", contador_insercao, contadorMovInsercao);
    printf("Funcao de remocao fez %d comparacoes e %d movimentacoes\n", contador_remocao, contadorMovRemocao);
    printf("Funcao de busca fez %d comparacoes\n", contador_busca);
}

double maxMedicao(No* sensor) {
    if (sensor->medicoes == NULL) return 0.0;

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
    if (sensor->medicoes == NULL) return 0.0;

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
