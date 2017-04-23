#pragma once
#include <stdbool.h>
#include "aho_trie.h"

struct aho_queue_node
{
    struct aho_queue_node *next, *prev;
    struct aho_trie_node *data;
};

struct aho_queue
{
    struct aho_queue_node *front;
    struct aho_queue_node *rear;
    unsigned int count;
};


void aho_queue_init(struct aho_queue * restrict que);
void aho_queue_destroy(struct aho_queue * restrict que);

bool aho_queue_enqueue(struct aho_queue * restrict que, struct aho_trie_node *node);
struct aho_queue_node* aho_queue_dequeue(struct aho_queue * restrict que);

/* inline */
bool aho_queue_empty(struct aho_queue * restrict que);
