#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_encadeamento.h"

unsigned int funcao_hashing(t_hash* t, int chave) {
    return chave % t->tamanho;
}

t_hash* criar_hash(int tamanho) {
    t_hash* nova = malloc(sizeof(t_hash));
    nova->tamanho = tamanho;
    nova->vetor = malloc(tamanho * sizeof(t_elem_hash*));
    for (int i = 0; i < tamanho; i++) {
        nova->vetor[i] = NULL;
    }
    return nova;
}

Sensor* cria_sensor(int pump_id, int class_id, double temperature, double vibration, double pressure, double flow_rate, double rpm, double operational_hours, int maintenance_flag) {
    Sensor* s = malloc(sizeof(Sensor));
    s->pump_id = pump_id;
    s->class_id = class_id;
    s->temperature = temperature;
    s->vibration = vibration;
    s->pressure = pressure;
    s->flow_rate = flow_rate;
    s->rpm = rpm;
    s->operational_hours = operational_hours;
    s->maintenance_flag = maintenance_flag;
    return s;
}

void inserir_hash(t_hash* t, int chave, Sensor* sensor) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* novo = malloc(sizeof(t_elem_hash));
    novo->chave = chave;
    novo->carga = sensor;
    novo->prox = t->vetor[pos]; //Encadeamento
    t->vetor[pos] = novo;
}

Sensor* buscar_hash(t_hash* t, int chave) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* atual = t->vetor[pos];
    while (atual != NULL) {
        if (atual->chave == chave) {
            return atual->carga;
        }
        atual = atual->prox;
    }
    return NULL;
}

void remover_hash(t_hash* t, int chave) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* atual = t->vetor[pos];
    t_elem_hash* anterior = NULL;

    while (atual != NULL && atual->chave != chave) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Sensor com Pump_ID %d nao encontrado.\n", chave);
        return;
    }

    if (anterior == NULL) {
        t->vetor[pos] = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual->carga);
    free(atual);
    printf("Sensor com Pump_ID %d removido.\n", chave);
}

// Função para gerar relatórios (MEAN, MAX, MIN)
void gerar_relatorio(t_hash* t, const char* comando, int chave) {
    Sensor* sensor = buscar_hash(t, chave);
    if (sensor == NULL) {
        printf("Sensor com Pump_ID %d nao encontrado.\n", chave);
        return;
    }

    if (strcmp(comando, "MEAN") == 0) {
        double media = (sensor->temperature + sensor->vibration + sensor->pressure) / 3;
        printf("Media das medições do sensor com Pump_ID %d: %.2f\n", chave, media);
    } else if (strcmp(comando, "MAX") == 0) {
        double max_valor = sensor->temperature;
        if (sensor->vibration > max_valor) max_valor = sensor->vibration;
        if (sensor->pressure > max_valor) max_valor = sensor->pressure;
        printf("Maximo das medições do sensor com Pump_ID %d: %.2f\n", chave, max_valor);
    } else if (strcmp(comando, "MIN") == 0) {
        double min_valor = sensor->temperature;
        if (sensor->vibration < min_valor) min_valor = sensor->vibration;
        if (sensor->pressure < min_valor) min_valor = sensor->pressure;
        printf("Minimo das medições do sensor com Pump_ID %d: %.2f\n", chave, min_valor);
    }
}
