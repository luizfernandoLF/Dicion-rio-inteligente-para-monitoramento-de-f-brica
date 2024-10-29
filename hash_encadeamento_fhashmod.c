#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define TABELA_TAM 10

// Estrutura para Medições de Sensores
typedef struct sensor {
    int pump_id;
    int class_id;
    double temperature;
    double vibration;
    double pressure;
    double flow_rate;
    double rpm;
    double operational_hours;
    int maintenance_flag;
} Sensor;

//elemento que vai ser armazenado na tb hash
typedef struct elem_hash {
    int chave;
    Sensor* carga;
    struct elem_hash* prox;
} t_elem_hash;

// Tabela em si, com vetor pra ponteiro
typedef struct {
    t_elem_hash** vetor;
    int tamanho;
} t_hash;

//Função hash
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

// Inserção de um novo sensor na tabela hash
void inserir_hash(t_hash* t, int chave, Sensor* sensor) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* novo = malloc(sizeof(t_elem_hash));
    novo->chave = chave;
    novo->carga = sensor;
    novo->prox = t->vetor[pos]; // Encadeamento no caso de colisão
    t->vetor[pos] = novo;
}

// Busca de um sensor na tabela hash
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

// Função para remover um sensor
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

// gerador de relatórios brabos
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

int main(int argc, char* argv[]) {

    
    if (argc != 3) {
        printf("Tente novamente, Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    const char* arquivo_entrada = argv[1];
    const char* arquivo_saida = argv[2];

    int tamanho = TABELA_TAM;
    t_hash* tabela = criar_hash(tamanho);

    FILE* entrada = fopen(arquivo_entrada, "r");
    if (!entrada) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    FILE* saida = fopen(arquivo_saida, "w");
    if (!saida) {
        printf("Erro ao abrir o arquivo de saida.\n");
        fclose(entrada);
        return 1;
    }

    char comando[10];
    int pump_id, class_id, maintenance_flag;
    double temperature, vibration, pressure, flow_rate, rpm, operational_hours;

    while (fscanf(entrada, "%s", comando) != EOF) {
        if (strcmp(comando, "ADD") == 0) {
            fscanf(entrada, "%d %d %lf %lf %lf %lf %lf %lf %d",
                   &pump_id, &class_id, &temperature, &vibration, &pressure,
                   &flow_rate, &rpm, &operational_hours, &maintenance_flag);
            Sensor* sensor = cria_sensor(pump_id, class_id, temperature, vibration, pressure,
                                         flow_rate, rpm, operational_hours, maintenance_flag);
            inserir_hash(tabela, pump_id, sensor);
            fprintf(saida, "Sensor com Pump_ID %d adicionado.\n", pump_id);
        } else if (strcmp(comando, "SEARCH") == 0) {
            fscanf(entrada, "%d", &pump_id);
            Sensor* sensor = buscar_hash(tabela, pump_id);
            if (sensor) {
                fprintf(saida, "Sensor com Pump_ID %d encontrado.\n", pump_id);
            } else {
                fprintf(saida, "Sensor com Pump_ID %d nao encontrado.\n", pump_id);
            }
        } else if (strcmp(comando, "REMOVE") == 0) {
            fscanf(entrada, "%d", &pump_id);
            remover_hash(tabela, pump_id);
        } else if (strcmp(comando, "REPORT") == 0) {
            char subcomando[10];
            fscanf(entrada, "%s %d", subcomando, &pump_id);
            gerar_relatorio(tabela, subcomando, pump_id);
        }
    }

    fclose(entrada);
    fclose(saida);
}
