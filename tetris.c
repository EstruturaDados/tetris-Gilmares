#include <stdio.h>
#include <stdlib.h> // Para a função exit() e rand()/srand() se quiser gerar peças aleatórias
#include <time.h>   // Para inicialização do gerador de números aleatórios

#define MAX_SIZE 5 // Tamanho fixo da fila

// Estrutura para representar uma peça de Tetris
typedef struct {
    char tipo; // Tipo da peça (ex: 'I', 'O', 'T', 'L')
    int id;   // Identificador numérico único
} Pecas;

// Estrutura para a Fila Circular
typedef struct {
    Pecas items[MAX_SIZE];
    int primeiro; // Índice do primeiro elemento (próximo a sair)
    int ultimo;  // Índice do último elemento (onde o novo será inserido)
    int atual; // Número atual de elementos na fila
} PecaQueue;

// Função para inicializar a fila
void initializeQueue(PecaQueue *q) {
    q->primeiro = 0;
    q->ultimo = -1;
    q->atual = 0;
}

// Função para verificar se a fila está vazia
int isEmpty(PecaQueue *q) {
    return q->atual == 0;
}

// Função para verificar se a fila está cheia
int isFull(PecaQueue *q) {
    return q->atual == MAX_SIZE;
}

// Função para adicionar uma nova peça (enqueue)
void enqueue(PecaQueue *q, Pecas newPeca) {
    if (isFull(q)) {
        printf("A fila esta cheia. Nao e possivel adicionar mais pecas.\n");
        return;
    }
    q->ultimo = (q->ultimo + 1) % MAX_SIZE; // Avança o rear circularmente
    q->items[q->ultimo] = newPeca;
    q->atual++;
    
    printf("Peça '%c' %d adicionada ao final da fila.\n", newPeca.tipo, newPeca.id);
}

// Função para remover e retornar a peça da frente (dequeue)
Pecas dequeue(PecaQueue *q) {
    if (isEmpty(q)) {
        printf("A fila esta vazia. Nao ha pecas para jogar.\n");
        // Retorna uma peça vazia/inválida como indicador de erro
        Pecas empty = {' ', -1}; 
        return empty;
    }
    Pecas removedPeca = q->items[q->primeiro];
    q->primeiro = (q->primeiro + 1) % MAX_SIZE; // Avança o front circularmente
    q->atual--;
    printf("Peça '%c' (ID: %d) removida da frente da fila (jogada).\n", removedPeca.tipo, removedPeca.id);
    return removedPeca;
}

// Função para exibir o estado atual da fila
void displayQueue(PecaQueue *q) {
    if (isEmpty(q)) {
        printf("A fila de pecas esta vazia.\n");
        return;
    }
    printf("\nFila de Pecas (Proxima a sair -> Ultima):\n\n");
    int i;
    int current = q->primeiro;
    for (i = 0; i < q->atual; i++) {
        printf("[%c, %d] ", q->items[current].tipo, q->items[current].id);
        current = (current + 1) % MAX_SIZE;
    }
    printf("\n");
}

// Função auxiliar para gerar uma nova peça com ID único (simulação)
Pecas generateNewPeca(int *id_counter) {
    char tipos[] = {'I', 'O', 'T', 'L' };
    int tipoIndex = rand() % 4; // Escolhe um tipo aleatório
    Pecas newPeca;
    newPeca.tipo = tipos[tipoIndex];
    newPeca.id = (*id_counter)++; // Atribui ID único e incrementa o contador
    return newPeca;
}

int main() {
    PecaQueue queue;
    initializeQueue(&queue);
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    int id_counter = 101; // Contador de ID inicial

    // Inicializa a fila com 5 elementos iniciais
    for (int i = 0; i < MAX_SIZE; i++) {
        enqueue(&queue, generateNewPeca(&id_counter));
    }
    displayQueue(&queue);

    int choice;
    do {
        printf("\nMenu de Opcoes:\n\n");
        printf("1. Visualizar fila\n");
        printf("2. Jogar uma peca (remover da frente)\n");
        printf("3. Adicionar nova peca (ao final)\n");
        printf("4. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayQueue(&queue);
                break;
            case 2:
                dequeue(&queue);
                displayQueue(&queue);
                break;
            case 3:
                // Tenta adicionar uma nova peça. generateNewPiece é chamada apenas se houver espaço
                // A verificação de full está dentro de enqueue, mas para um controle melhor podemos verificar antes ou usar a lógica interna.
                // Como generateNewPiece não tem side effects além do id, podemos passar diretamente.
                if (!isFull(&queue)) {
                   enqueue(&queue, generateNewPeca(&id_counter));
                }
                displayQueue(&queue);
                break;
            case 4:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 4);

    return 0;
}
