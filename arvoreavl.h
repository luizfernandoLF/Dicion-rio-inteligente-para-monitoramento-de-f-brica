typedef struct Medicao {
    double temperatura;
    double vibracao;
    double pressao;
    struct Medicao* prox;
} Medicao;

typedef struct No {
    int chave;
    Medicao* medicoes;
    struct No* esquerda;
    struct No* direita;
    int altura;
} No;

int altura(No *N);
int max(int a, int b);
No* novoNo(int chave);
No* inserirNo(No* raiz, int chave, double temperatura, double vibracao, double pressao);
void adicionarMedicao(No* sensor, double temperatura, double vibracao, double pressao);
double maxMedicao(No* sensor);
double minMedicao(No* sensor);
double calcularMediaMedicoes(No* sensor);
No *rotacaoDir(No *y);
No *rotacaoEsq(No *x);
No* removerNo(No* raiz, int chave);
No* buscarNo(No* raiz, int chave);
