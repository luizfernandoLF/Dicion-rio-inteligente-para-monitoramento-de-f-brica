#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_hash.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.in> <arquivo_saida.out>\n", argv[0]);
        return 1;
    }

    const char* arquivo_entrada = argv[1];
    const char* arquivo_saida = argv[2];

    t_hash* tabela = criar_hash(1000);

    FILE* entrada = fopen(arquivo_entrada, "r");
    if (!entrada) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    FILE* saida = fopen(arquivo_saida, "w");
    if (!saida) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
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
            inserir_hash(tabela, pump_id, sensor, saida);

        } else if (strcmp(comando, "SEARCH") == 0) {
            fscanf(entrada, "%d", &pump_id);
            buscar_hash(tabela, pump_id, saida); 

        } else if (strcmp(comando, "REMOVE") == 0) {
            fscanf(entrada, "%d", &pump_id);
            remover_hash(tabela, pump_id, saida);

        } else if (strcmp(comando, "REPORT") == 0) {
            char subcomando[10];
            fscanf(entrada, "%s %d", subcomando, &pump_id);
            gerar_relatorio(tabela, subcomando, pump_id, saida);

        } else if (strcmp(comando, "END") == 0) {
            break;
        }
    }

    fclose(entrada);
    fclose(saida);

    imprimirContadores();

    return 0;
}
