
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
    struct sensor* prox;
} Sensor;

typedef struct elem_hash {
    int chave;
    Sensor* carga;
    struct elem_hash* prox;
} t_elem_hash;

typedef struct {
    t_elem_hash** vetor;
    int tamanho;
} t_hash;

unsigned int funcao_hashing(t_hash* t, int chave);
t_hash* criar_hash(int tamanho);
Sensor* cria_sensor(int pump_id, int class_id, double temperature, double vibration, double pressure, double flow_rate, double rpm, double operational_hours, int maintenance_flag);
void inserir_hash(t_hash* t, int chave, Sensor* sensor, FILE* saida);
Sensor* buscar_hash(t_hash* t, int chave, FILE* saida);
void remover_hash(t_hash* t, int chave, FILE* saida);
void gerar_relatorio(t_hash* t, const char* comando, int chave, FILE* saida);
void imprimirContadores();


