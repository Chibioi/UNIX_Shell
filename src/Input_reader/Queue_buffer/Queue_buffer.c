#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512

typedef struct Node {
  char data[BLOCK_SIZE];
  int char_len;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct Queue {
  Node *front;
  Node *rear;
  Node *cursor;
  int cursor_pos;
} Queue;

Node *createNode() {
  Node *temp = (Node *)malloc(sizeof(Node));
  if (temp == NULL) {
    puts("Failure allocating memory to the Node");
    exit(1);
  }
  // Not initializing the pointer causes wild pointers which could lead to
  // memory bugs.
  temp->next = NULL;
  temp->prev = NULL;
  temp->char_len = 0;
  return temp;
};

Queue *createQueue() {
  Queue *q = (Queue *)malloc(sizeof(Queue));
  if (q == NULL) {
    puts("Failure allocating memory to the queue");
    exit(1);
  }
  q->front = NULL;
  q->rear = NULL;
  q->cursor = NULL;
  return q;
}

// CURSOR MOVEMENTS
void move_left(Queue *q) {
  if (!q || !q->cursor) {
    return;
  }

  if (q->cursor_pos > 0) {
    q->cursor_pos--;
  } else if (q->cursor->prev) {
    q->cursor = q->cursor->prev;
    q->cursor_pos = q->cursor->char_len;
  }
}

void move_right(Queue *q) {
  if (!q || !q->cursor) {
    return;
  }

  if (q->cursor_pos < q->cursor->char_len) {
    q->cursor_pos++;
  } else if (q->cursor->next) {
    q->cursor = q->cursor->next;
    q->cursor_pos = 0;
  }
}

void insert_char(Queue *q, char c) {
  if (!q) {
    return;
  }

  if (!q->cursor) {
    Node *newNode = createNode();
    newNode->data[0] = c;
    newNode->char_len = 1;
    q->front = q->rear = q->cursor = newNode;
    q->cursor_pos = 1;
    return;
  }

  Node *n = q->cursor;
  // If current node is full, split
  if (n->char_len == BLOCK_SIZE) {
    Node *newNode = createNode();
    // Move chars after cursor to new node
    int move_len = n->char_len - q->cursor_pos;
    if (move_len > 0) {
      memcpy(newNode->data, n->data + q->cursor_pos, move_len);
      newNode->char_len = move_len;
    }
    // Update links
    newNode->next = n->next;
    newNode->prev = n;
    if (n->next)
      n->next->prev = newNode;
    n->next = newNode;
    n->char_len = q->cursor_pos; // truncate current node at cursor
  }
  // Insert character into current node at cursor_pos
  memmove(n->data + q->cursor_pos + 1, n->data + q->cursor_pos,
          n->char_len - q->cursor_pos);
  n->data[q->cursor_pos] = c;
  n->char_len++;
  q->cursor_pos++;
}

void backspace(Queue *q) {
  if (!q || !q->cursor)
    return;

  Node *n = q->cursor;

  if (q->cursor_pos > 0) {
    // Remove character in current node
    memmove(n->data + q->cursor_pos - 1, n->data + q->cursor_pos,
            n->char_len - q->cursor_pos);
    n->char_len--;
    q->cursor_pos--;
  } else if (n->prev) {
    // Move to previous node
    q->cursor = n->prev;
    q->cursor_pos = q->cursor->char_len;
    // Delete last character of previous node
    if (q->cursor_pos > 0) {
      q->cursor->char_len--;
      q->cursor_pos--;
    }
    // Optional: merge nodes if empty
  }
}

void freeQueue(Queue *q) {
  if (!q) {
    return;
  }
  Node *n = q->front;
  while (n) {
    Node *next = n->next;
    free(n);
    n = next;
  }
  free(q);
}

// Print the entire buffer
void printBuffer(Queue *q) {
  Node *n = q->front;
  while (n) {
    for (int i = 0; i < n->char_len; i++) {
      putchar(n->data[i]);
    }
    n = n->next;
  }
  putchar('\n');
}

void queue_clear(Queue *q) {
  if (!q)
    return;

  Node *cur = q->front;
  while (cur) {
    Node *next = cur->next;
    free(cur);
    cur = next;
  }

  q->front = NULL;
  q->rear = NULL;
  q->cursor = NULL;
}
