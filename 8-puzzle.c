#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define N 3 // Dimension of the puzzle
#define MAX_PQ_SIZE 100000 // Max size of the priority queue

typedef struct {
    int tiles[N][N];
    int blank_row, blank_col;
} Board;

typedef struct Node {
    Board board;
    int moves;
    int priority;
    struct Node *prev;
} Node;

typedef struct {
    Node **nodes;
    int size;
    int capacity;
} MinPQ;

// Function prototypes
void initialize_board(Board *board, int tiles[N][N]);
int hamming(const Board *board);
int manhattan(const Board *board);
bool is_goal(const Board *board);
bool equals(const Board *board1, const Board *board2);
Node *create_node(Board board, int moves, Node *prev);
void insert(MinPQ *pq, Node *node);
Node *del_min(MinPQ *pq);
void swap(Node **a, Node **b);
void min_heapify(MinPQ *pq, int i);
MinPQ *init_pq(int capacity);
void free_pq(MinPQ *pq);
void solve_puzzle(Board *initial);
void print_solution(Node *node);

void initialize_board(Board *board, int tiles[N][N]) {
    memcpy(board->tiles, tiles, sizeof(board->tiles));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tiles[i][j] == 0) {
                board->blank_row = i;
                board->blank_col = j;
            }
        }
    }
}

int hamming(const Board *board) {
    int h = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board->tiles[i][j] != 0 && board->tiles[i][j] != i * N + j + 1) {
                h++;
            }
        }
    }
    return h;
}

int manhattan(const Board *board) {
    int m = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value = board->tiles[i][j];
            if (value != 0) {
                int target_row = (value - 1) / N;
                int target_col = (value - 1) % N;
                m += abs(i - target_row) + abs(j - target_col);
            }
        }
    }
    return m;
}

bool is_goal(const Board *board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board->tiles[i][j] != 0 && board->tiles[i][j] != i * N + j + 1) {
                return false;
            }
        }
    }
    return true;
}

bool equals(const Board *board1, const Board *board2) {
    return memcmp(board1->tiles, board2->tiles, sizeof(board1->tiles)) == 0;
}

Node *create_node(Board board, int moves, Node *prev) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->board = board;
    node->moves = moves;
    node->priority = moves + manhattan(&board);
    node->prev = prev;
    return node;
}

MinPQ *init_pq(int capacity) {
    MinPQ *pq = (MinPQ *)malloc(sizeof(MinPQ));
    pq->nodes = (Node **)malloc(sizeof(Node *) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void insert(MinPQ *pq, Node *node) {
    if (pq->size == pq->capacity) {
        printf("Priority queue overflow!\n");
        exit(EXIT_FAILURE);
    }
    pq->nodes[pq->size++] = node;
    int i = pq->size - 1;
    while (i > 0 && pq->nodes[i]->priority < pq->nodes[(i - 1) / 2]->priority) {
        swap(&pq->nodes[i], &pq->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Node *del_min(MinPQ *pq) {
    if (pq->size == 0) return NULL;
    Node *min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    min_heapify(pq, 0);
    return min;
}

void swap(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(MinPQ *pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->nodes[left]->priority < pq->nodes[smallest]->priority) {
        smallest = left;
    }
    if (right < pq->size && pq->nodes[right]->priority < pq->nodes[smallest]->priority) {
        smallest = right;
    }
    if (smallest != i) {
        swap(&pq->nodes[i], &pq->nodes[smallest]);
        min_heapify(pq, smallest);
    }
}

void free_pq(MinPQ *pq) {
    for (int i = 0; i < pq->size; i++) {
        free(pq->nodes[i]);
    }
    free(pq->nodes);
    free(pq);
}

void solve_puzzle(Board *initial) {
    MinPQ *pq = init_pq(MAX_PQ_SIZE);
    Node *initial_node = create_node(*initial, 0, NULL);
    insert(pq, initial_node);

    while (pq->size > 0) {
        Node *current = del_min(pq);
        if (is_goal(&current->board)) {
            printf("Minimum number of moves = %d\n", current->moves);
            print_solution(current);
            free_pq(pq);
            return;
        }

        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int d = 0; d < 4; d++) {
            int new_row = current->board.blank_row + directions[d][0];
            int new_col = current->board.blank_col + directions[d][1];

            if (new_row >= 0 && new_row < N && new_col >= 0 && new_col < N) {
                Board new_board = current->board;
                new_board.tiles[current->board.blank_row][current->board.blank_col] = new_board.tiles[new_row][new_col];
                new_board.tiles[new_row][new_col] = 0;
                new_board.blank_row = new_row;
                new_board.blank_col = new_col;

                if (current->prev == NULL || !equals(&new_board, &current->prev->board)) {
                    Node *neighbor = create_node(new_board, current->moves + 1, current);
                    insert(pq, neighbor);
                }
            }
        }
        free(current);
    }

    printf("No solution possible\n");
    free_pq(pq);
}

void print_solution(Node *node) {
    if (node == NULL) return;
    print_solution(node->prev);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", node->board.tiles[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int tiles[N][N] = {
        {8, 1, 3},
        {4, 0, 2},
        {7, 6, 5}
    };

    Board initial;
    initialize_board(&initial, tiles);

    solve_puzzle(&initial);

    return 0;
}
