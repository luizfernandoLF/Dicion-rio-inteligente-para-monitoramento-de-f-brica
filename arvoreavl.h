typedef struct Medicao {
    double temperatura;
    double vibracao;
    double pressao;
    struct Medicao* prox;
} Medicao;

typedef struct No {
    int chave;
    int classID;
    Medicao* medicoes;  
    struct No* esquerda;
    struct No* direita;
    int altura;
} No;

int altura(No *N);
int max(int a, int b);
No* novoNo(int chave, int classID);
No* inserirNo(No* raiz, int chave, int classID, double temperatura, double vibracao, double pressao);
void adicionarMedicao(No* sensor, double temperatura, double vibracao, double pressao);
int fatorBalanc(No *N);
No *rotacaoDir(No *y);
No *rotacaoEsq(No *x);
No* removerNo(No* raiz, int chave);
No* buscarNo(No* raiz, int chave);
No* buscarNoRemocao(No* raiz, int chave);
double calcularMediaMedicoes(No* sensor); 
double maxMedicao(No* sensor);  
double minMedicao(No* sensor);  

void imprimirContadores();
