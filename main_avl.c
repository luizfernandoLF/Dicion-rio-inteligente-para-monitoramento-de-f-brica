#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreavl.h"

void executar_comandos(FILE* entrada, FILE* saida) {
    No* raiz = NULL;
    char comando[10];
    int chave, classID;
    double temperatura, vibracao, pressao;

    while (fscanf(entrada, "%s", comando) != EOF) {
        if (strcmp(comando, "ADD") == 0) {
            if (fscanf(entrada, "%d %d %lf %lf %lf", &chave, &classID, &temperatura, &vibracao, &pressao) == 5) {
                raiz = inserirNo(raiz, chave, classID, temperatura, vibracao, pressao);
                fprintf(saida, "Sensor com Pump_ID %d adicionado.\n", chave);
            }
        } else if (strcmp(comando, "REMOVE") == 0) {
            if (fscanf(entrada, "%d", &chave) == 1) {
                No* resultado = buscarNoRemocao(raiz, chave);
                raiz = removerNo(raiz, chave);
                if (raiz != NULL) {
                    Medicao* medicao = resultado->medicoes;
                    fprintf(saida, "Sensor com Pump_ID %d removido.\nDados:\n", chave);
                    fprintf(saida, "Pump_ID: %d, Class_ID: %d, Temperatura: %.2f, Vibracao: %.2f, Pressao: %.2f\n",
                                resultado->chave, resultado->classID,
                                medicao->temperatura, medicao->vibracao, medicao->pressao);
                } else {
                    fprintf(saida, "Sensor com Pump_ID %d não encontrado.\n", chave);
                }
            }
        } else if (strcmp(comando, "SEARCH") == 0) {
            if (fscanf(entrada, "%d", &chave) == 1) {
                No* resultado = buscarNo(raiz, chave);
                if (resultado != NULL) {
                    Medicao* medicao = resultado->medicoes;
                    while (medicao != NULL) {
                        fprintf(saida, "Buscando...\n");
                        fprintf(saida, "Sensor com Pump_ID %d encontrado!\nDados:\n", chave);
                        fprintf(saida, "Pump_ID: %d, Class_ID: %d, Temperatura: %.2f, Vibracao: %.2f, Pressao: %.2f\n",
                                resultado->chave, resultado->classID,
                                medicao->temperatura, medicao->vibracao, medicao->pressao);
                        medicao = medicao->prox;
                    }
                } else {
                    fprintf(saida, "Sensor com Pump_ID %d não encontrado.\n", chave);
                }
            }
        } else if (strcmp(comando, "REPORT") == 0) {
            char tipo[10];
            if (fscanf(entrada, "%s %d", tipo, &chave) == 2) {
                No* resultado = buscarNo(raiz, chave);
                if (resultado != NULL) {
                    if (strcmp(tipo, "MEAN") == 0) {
                        double media = calcularMediaMedicoes(resultado);
                        fprintf(saida, "Relatório MEAN para Pump_ID %d: %.2f\n", chave, media);
                    } else if (strcmp(tipo, "MAX") == 0) {
                        double max_valor = maxMedicao(resultado);
                        fprintf(saida, "Relatório MAX para Pump_ID %d: %.2f\n", chave, max_valor);
                    } else if (strcmp(tipo, "MIN") == 0) {
                        double min_valor = minMedicao(resultado);
                        fprintf(saida, "Relatório MIN para Pump_ID %d: %.2f\n", chave, min_valor);
                    }
                } else {
                    fprintf(saida, "Sensor com Pump_ID %d nao encontrado para relatório.\n", chave);
                }
            }
        } else if (strcmp(comando, "END") == 0) {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.in> <arquivo_saida.out>\n", argv[0]);
        return 1;
    }

    FILE* entrada = fopen(argv[1], "r");
    if (!entrada) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    FILE* saida = fopen(argv[2], "w");
    if (!saida) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
        fclose(entrada);
        return 1;
    }

    executar_comandos(entrada, saida);

    fclose(entrada);
    fclose(saida);

    imprimirContadores();  // Exibe os contadores de comparações no terminal

    return 0;
}
