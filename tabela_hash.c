#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_hash.h"

int contador_insercao = 0;
int contador_busca = 0;
int contador_remocao = 0;

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
    s->prox = NULL; 
    return s;
}

unsigned int funcao_hashing(t_hash* t, int chave) {
    return chave % t->tamanho;
}

void inserir_hash(t_hash* t, int chave, Sensor* sensor, FILE* saida) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* atual = t->vetor[pos];

    while (atual != NULL) {
        contador_insercao++; 
        if (atual->chave == chave) {
            Sensor* medicao_atual = atual->carga;
            while (medicao_atual->prox != NULL) {
                contador_insercao++;
                medicao_atual = medicao_atual->prox;
            }
            medicao_atual->prox = sensor;
            fprintf(saida, "Sensor com Pump_ID %d adicionado.\n", chave);
            return;
        }
        atual = atual->prox;
    }

    t_elem_hash* novo = malloc(sizeof(t_elem_hash));
    novo->chave = chave;
    novo->carga = sensor;
    novo->prox = t->vetor[pos];
    t->vetor[pos] = novo;
    fprintf(saida, "Sensor com Pump_ID %d adicionado.\n", chave);
}

Sensor* buscar_hash(t_hash* t, int chave, FILE* saida) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* atual = t->vetor[pos];

    while (atual != NULL) {
        contador_busca++;
        if (atual->chave == chave) {
            Sensor* medicao = atual->carga;
            fprintf(saida,"SENSOR ENCONTRADO ↓\n");
            while (medicao != NULL) {
                fprintf(saida, "Pump_ID: %d, Class_ID: %d, Temperatura: %.2f, Vibracao: %.2f, Pressao: %.2f, "
                               "Flow_Rate: %.2f, RPM: %.2f, Horas Operacionais: %.2f, Flag de manutencao: %d\n",
                        medicao->pump_id, medicao->class_id, medicao->temperature, medicao->vibration, medicao->pressure,
                        medicao->flow_rate, medicao->rpm, medicao->operational_hours, medicao->maintenance_flag);
                medicao = medicao->prox;
            }
            return atual->carga;
        }
        atual = atual->prox;
    }
    
    fprintf(saida, "Sensor com Pump_ID %d nao encontrado.\n", chave);
    return NULL;
}

void remover_hash(t_hash* t, int chave, FILE* saida) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* atual = t->vetor[pos];
    t_elem_hash* anterior = NULL;

    while (atual != NULL && atual->chave != chave) {
        contador_remocao++;
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        fprintf(saida, "Sensor com Pump_ID %d nao encontrado.\n", chave);
        return;
    }

    Sensor* medicao = atual->carga;
    while (medicao != NULL) {
        fprintf(saida, "Removendo - Pump_ID: %d, Class_ID: %d, Temperatura: %.2f, Vibracao: %.2f, Pressao: %.2f, "
                       "Flow_Rate: %.2f, RPM: %.2f, Horas Operacionais: %.2f, Flag de manutencao: %d\n",
                medicao->pump_id, medicao->class_id, medicao->temperature, medicao->vibration, medicao->pressure,
                medicao->flow_rate, medicao->rpm, medicao->operational_hours, medicao->maintenance_flag);
        Sensor* temp = medicao;
        medicao = medicao->prox;
        free(temp);
    }

    if (anterior == NULL) {
        t->vetor[pos] = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    fprintf(saida, "Sensor com Pump_ID %d removido.\n", chave);
}

void calcularMedia(Sensor* sensor, FILE* saida) {
    double total_temperature = 0, total_vibration = 0, total_pressure = 0;
    int count = 0;
    Sensor* medicao = sensor;

    while (medicao != NULL) {
        total_temperature += medicao->temperature;
        total_vibration += medicao->vibration;
        total_pressure += medicao->pressure;
        count++;
        medicao = medicao->prox;
    }

    if (count > 0) {
        fprintf(saida, "Média das medições do sensor %d: Temperatura=%.2f, Vibracao=%.2f, Pressao=%.2f\n",
                sensor->pump_id, total_temperature / count, total_vibration / count, total_pressure / count);
    }
}

void maxMedicao(Sensor* sensor, FILE* saida) {
    if (sensor == NULL) return;

    double max_temperature = sensor->temperature;
    double max_vibration = sensor->vibration;
    double max_pressure = sensor->pressure;

    Sensor* medicao = sensor->prox;
    while (medicao != NULL) {
        if (medicao->temperature > max_temperature) max_temperature = medicao->temperature;
        if (medicao->vibration > max_vibration) max_vibration = medicao->vibration;
        if (medicao->pressure > max_pressure) max_pressure = medicao->pressure;
        medicao = medicao->prox;
    }

    fprintf(saida, "Máximo das medições do sensor %d: Temperatura=%.2f, Vibracao=%.2f, Pressao=%.2f\n",
            sensor->pump_id, max_temperature, max_vibration, max_pressure);
}

void minMedicao(Sensor* sensor, FILE* saida) {
    if (sensor == NULL) return;

    double min_temperature = sensor->temperature;
    double min_vibration = sensor->vibration;
    double min_pressure = sensor->pressure;

    Sensor* medicao = sensor->prox;
    while (medicao != NULL) {
        if (medicao->temperature < min_temperature) min_temperature = medicao->temperature;
        if (medicao->vibration < min_vibration) min_vibration = medicao->vibration;
        if (medicao->pressure < min_pressure) min_pressure = medicao->pressure;
        medicao = medicao->prox;
    }

    fprintf(saida, "Mínimo das medições do sensor %d: Temperatura=%.2f, Vibracao=%.2f, Pressao=%.2f\n",
            sensor->pump_id, min_temperature, min_vibration, min_pressure);
}

void gerar_relatorio(t_hash* t, const char* comando, int chave, FILE* saida) {
    Sensor* sensor = buscar_hash(t, chave, saida);
    if (sensor == NULL) {
        return;
    }

    if (strcmp(comando, "MEAN") == 0) {
        calcularMedia(sensor, saida);
    } else if (strcmp(comando, "MAX") == 0) {
        maxMedicao(sensor, saida);
    } else if (strcmp(comando, "MIN") == 0) {
        minMedicao(sensor, saida);
    }
}

void imprimirContadores() {
    printf("Funcao de insercao fez %d comparacoes\n", contador_insercao);
    printf("Funcao de busca fez %d comparacoes\n", contador_busca);
    printf("Funcao de remocao fez %d comparacoes\n", contador_remocao);
}
