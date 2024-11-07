typedef struct No{
    int chave;
    struct No *esquerda;
    struct No *direita;
    int altura;
} No;

int altura(No *N);
int max(int a, int b);
No* novoNo(int chave);
No *rotacaoDir(No *y);
No *rotacaoEsq(No *x);
int fatorBalanc(No *N);
No* inserirNo(No* No, int chave);
No *noDeValorMinimo(No *No);
No* removerNo(No* raiz, int chave);
No* buscarNo(No* raiz, int chave);
void imprimirArvore(No *raiz);
