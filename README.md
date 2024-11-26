# Dicionário-inteligente-para-monitoramento-de-fábrica
Descrição

Este projeto implementa um sistema de dicionário inteligente utilizando duas estruturas de dados: Tabela Hash e Árvore AVL. O objetivo é comparar o desempenho dessas estruturas em operações como inserção, remoção, busca e geração de relatórios (média, máximo e mínimo). Cada entrada no dicionário representa um sensor com medições associadas a atributos como temperatura, vibração e pressão.
Estruturas de Dados Implementadas
1. Tabela Hash

    Uma tabela hash com encadeamento separado para tratar colisões.
    Operações principais:
        Inserção: Insere ou atualiza medições associadas a um sensor.
        Busca: Retorna todas as medições associadas a um sensor específico.
        Remoção: Remove um sensor e todas as suas medições, exibindo os dados antes da remoção.
        Relatórios: Gera relatórios de:
            Média: Média das medições (temperatura, vibração e pressão).
            Máximo: Valores máximos registrados em temperatura, vibração e pressão.
            Mínimo: Valores mínimos registrados em temperatura, vibração e pressão.

2. Árvore AVL

    Uma árvore AVL para armazenamento ordenado e balanceado dos sensores.
    Operações principais:
        Inserção: Insere ou atualiza medições em um nó específico.
        Busca: Retorna todas as medições associadas a um sensor.
        Remoção: Remove um sensor e todas as suas medições, exibindo os dados antes da remoção.
        Relatórios: Semelhante à tabela hash, gera relatórios de média, máximo e mínimo.

####Estrutura dos Arquivos####
Arquivos de Código

    Tabela Hash
        tabela_hash.c: Implementação da tabela hash e suas funções.
        tabela_hash.h: Cabeçalhos e definições para a tabela hash.
        main_hash.c: Programa principal para executar os comandos de entrada na tabela hash.

    Árvore AVL
        arvoreavl.c: Implementação da árvore AVL e suas funções.
        arvoreavl.h: Cabeçalhos e definições para a árvore AVL.
        main_avl.c: Programa principal para executar os comandos de entrada na árvore AVL.

####Arquivos de Entrada e Saída####

    Entrada (.in): Arquivos contendo os comandos de teste no seguinte formato:

    ADD Pump_ID Class_ID Temperature Vibration Pressure
    SEARCH Pump_ID
    REMOVE Pump_ID
    REPORT MEAN/ MAX/ MIN Pump_ID
    END

    Saída (.out): Arquivos gerados com os resultados das operações executadas, incluindo buscas, remoções e relatórios.

####Uso####
Compilação

Para compilar os programas, utilize os seguintes comandos:
Tabela Hash

gcc -o main_hash main_hash.c tabela_hash.c

Árvore AVL

gcc -o main_avl main_avl.c arvoreavl.c

####Execução####

Para executar os programas, forneça um arquivo de entrada (.in) e um arquivo de saída (.out):
Tabela Hash

./main_hash arquivo_entrada.in arquivo_saida.out

Árvore AVL

./main_avl arquivo_entrada.in arquivo_saida.out

####Contadores de Comparações e Movimentações####

O desempenho das operações é medido utilizando contadores de comparações e movimentações:

    Tabela Hash:
        Comparações:
            Verificações de igualdade para identificar o sensor correto.
        Movimentações:
            Navegações para encadear ou acessar elementos no bucket.

    Árvore AVL:
        Comparações:
            Verificações para encontrar o local correto de inserção ou remoção.
        Movimentações:
            Rebalanceamento dos nós após operações de inserção ou remoção.

Os contadores são exibidos no terminal após a execução de todas as operações.
Relatórios Gerados

    Média (MEAN):
        Calcula a média de temperatura, vibração e pressão de todas as medições associadas a um sensor.

    Máximo (MAX):
        Exibe o maior valor registrado de temperatura, vibração e pressão entre todas as medições do sensor.

    Mínimo (MIN):
        Exibe o menor valor registrado de temperatura, vibração e pressão entre todas as medições do sensor.

Exemplo de saída para um sensor:

Relatório MEAN para Pump_ID 207: Temperatura=123.45, Vibração=12.34, Pressão=567.89
Relatório MAX para Pump_ID 207: Temperatura=150.67, Vibração=15.78, Pressão=600.23
Relatório MIN para Pump_ID 207: Temperatura=100.12, Vibração=10.45, Pressão=500.34

####Observações####

    Desempenho:
        Tabela Hash é eficiente para operações rápidas de inserção e busca em grande escala.
        Árvore AVL mantém os dados ordenados e fornece balanceamento, tornando-a mais eficiente para consultas que requerem ordenação.

    Limitações:
        O tamanho fixo da tabela hash (1000) pode levar a colisões excessivas se o número de sensores for muito grande.
        A árvore AVL pode apresentar desempenho inferior em inserções e remoções devido ao rebalanceamento.

####Conclusão####

Este projeto demonstra como a escolha da estrutura de dados pode impactar o desempenho de um sistema. Enquanto a tabela hash é mais eficiente em operações rápidas e diretas, a árvore AVL é melhor para consultas ordenadas e estruturadas.

Faça os testes utilizando diferentes datasets para verificar o desempenho de cada estrutura em cenários distintos.

