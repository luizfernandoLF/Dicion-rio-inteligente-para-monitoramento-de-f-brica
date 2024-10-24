#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABELA_TAM 10

// Estrutura para Sensor
typedef struct sensor {
    char tipo_sensor[20];
    char localizacao_sensor[50];
    char historico_dados[100];
    bool alarmes;
    int codigo_barra; // chave
} Sensor;

// Estrutura para Equipamentos
typedef struct equipamentos {
    char modelo[50];
    char fabricante[50];
    char data_instalacao[10];
    char historico_manutencao[100];
    int codigo_barra; // chave
} Equipamento;

// Estrutura para Materiais
typedef struct materiais {
    char lote[10];
    char fornecedor[50];
    char data_validade[10];
    char localizacao_estoque[50];
    int codigo_barra; // chave
} Material;

// Estrutura de um elemento da tabela hash
typedef struct elem_hash {
    int chave;
    void* carga;
    struct elem_hash* prox;
} t_elem_hash;

// Estrutura da Tabela Hash
typedef struct {
    t_elem_hash** vetor;
    int tamanho;
} t_hash;

// Função para criação de sensor
Sensor* cria_sensor(const char* tipo_sensor, const char* localizacao_sensor, const char* historico_dados, bool alarmes, int codigo_barra) {
    Sensor* s = malloc(sizeof(Sensor));
    strcpy(s->tipo_sensor, tipo_sensor);
    strcpy(s->localizacao_sensor, localizacao_sensor);
    strcpy(s->historico_dados, historico_dados);
    s->alarmes = alarmes;
    s->codigo_barra = codigo_barra;
    return s;
}

// Função para criação de equipamento
Equipamento* cria_equipamento(const char* modelo, const char* fabricante, const char* data_instalacao, const char* historico_manutencao, int codigo_barra) {
    Equipamento* eq = malloc(sizeof(Equipamento));
    strcpy(eq->modelo, modelo);
    strcpy(eq->fabricante, fabricante);
    strcpy(eq->data_instalacao, data_instalacao);
    strcpy(eq->historico_manutencao, historico_manutencao);
    eq->codigo_barra = codigo_barra;
    return eq;
}

// Função para criação de material
Material* cria_material(const char* lote, const char* fornecedor, const char* data_validade, const char* localizacao_estoque, int codigo_barra) {
    Material* mat = malloc(sizeof(Material));
    strcpy(mat->lote, lote);
    strcpy(mat->fornecedor, fornecedor);
    strcpy(mat->data_validade, data_validade);
    strcpy(mat->localizacao_estoque, localizacao_estoque);
    mat->codigo_barra = codigo_barra;
    return mat;
}

// Função hash
unsigned int funcao_hashing(t_hash* t, int chave) {
    return chave % t->tamanho;
}

// Criação da tabela hash
t_hash* criar_hash(int tamanho) {
    t_hash* nova = malloc(sizeof(t_hash));
    nova->tamanho = tamanho;
    nova->vetor = malloc(tamanho * sizeof(t_elem_hash*));
    for (int i = 0; i < tamanho; i++) {
        nova->vetor[i] = NULL;
    }
    return nova;
}

// Inserção de um novo item na tabela hash
void inserir_hash(t_hash* t, int chave, void* carga) {
    unsigned int pos = funcao_hashing(t, chave);
    t_elem_hash* novo = malloc(sizeof(t_elem_hash));
    novo->chave = chave;
    novo->carga = carga;
    novo->prox = t->vetor[pos]; // Encadeamento no caso de colisão
    t->vetor[pos] = novo;
}

// Busca de um item na tabela hash
void* buscar_hash(t_hash* t, int chave) {
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

// Função para imprimir um sensor
void imprimir_sensor(FILE* saida, Sensor* sensor) {
    fprintf(saida, "Sensor: Tipo=%s, Localizacao=%s, Historico=%s, Alarme=%s\n",
            sensor->tipo_sensor, sensor->localizacao_sensor, sensor->historico_dados,
            sensor->alarmes ? "Ativado" : "Desativado");
}

// Função para imprimir um equipamento
void imprimir_equipamento(FILE* saida, Equipamento* equip) {
    fprintf(saida, "Equipamento: Modelo=%s, Fabricante=%s, Data Instalacao=%s, Historico=%s\n",
            equip->modelo, equip->fabricante, equip->data_instalacao, equip->historico_manutencao);
}

// Função para imprimir um material
void imprimir_material(FILE* saida, Material* material) {
    fprintf(saida, "Material: Lote=%s, Fornecedor=%s, Data Validade=%s, Localizacao=%s\n",
            material->lote, material->fornecedor, material->data_validade, material->localizacao_estoque);
}

// Leitura dos dados de um arquivo .in e inserção na tabela hash
void ler_arquivo_inserir(t_hash* tabela, const char* arquivo_entrada, const char* arquivo_saida) {
    FILE* entrada = fopen(arquivo_entrada, "r");
    FILE* saida = fopen(arquivo_saida, "w");

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    char tipo_item[20];
    int codigo_barra;

    // Variáveis temporárias para armazenar os dados
    char tipo_sensor[20], localizacao_sensor[50], historico_dados[100];
    bool alarmes;
    char modelo[50], fabricante[50], data_instalacao[10], historico_manutencao[100];
    char lote[10], fornecedor[50], data_validade[10], localizacao_estoque[50];

    // Leitura do arquivo
    while (fscanf(entrada, "%s %d", tipo_item, &codigo_barra) != EOF) {
        if (strcmp(tipo_item, "SENSOR") == 0) {
            fscanf(entrada, "%s %s %s %d", tipo_sensor, localizacao_sensor, historico_dados, &alarmes);
            Sensor* sensor = cria_sensor(tipo_sensor, localizacao_sensor, historico_dados, alarmes, codigo_barra);
            inserir_hash(tabela, codigo_barra, sensor);
            fprintf(saida, "Sensor inserido: %d\n", codigo_barra);
        } else if (strcmp(tipo_item, "EQUIPAMENTO") == 0) {
            fscanf(entrada, "%s %s %s %s", modelo, fabricante, data_instalacao, historico_manutencao);
            Equipamento* equip = cria_equipamento(modelo, fabricante, data_instalacao, historico_manutencao, codigo_barra);
            inserir_hash(tabela, codigo_barra, equip);
            fprintf(saida, "Equipamento inserido: %d\n", codigo_barra);
        } else if (strcmp(tipo_item, "MATERIAL") == 0) {
            fscanf(entrada, "%s %s %s %s", lote, fornecedor, data_validade, localizacao_estoque);
            Material* material = cria_material(lote, fornecedor, data_validade, localizacao_estoque, codigo_barra);
            inserir_hash(tabela, codigo_barra, material);
            fprintf(saida, "Material inserido: %d\n", codigo_barra);
        }
    }

    fclose(entrada);
    fclose(saida);
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    // Os nomes dos arquivos são passados pela linha de comando
    const char* arquivo_entrada = argv[1];
    const char* arquivo_saida = argv[2];

    // Crie a tabela hash (ou qualquer outra estrutura necessária)
    t_hash* tabela = criar_hash(10);  // Tamanho da tabela hash

    // Ler o arquivo de entrada e inserir os dados na tabela hash
    ler_arquivo_inserir(tabela, arquivo_entrada, arquivo_saida);

}

