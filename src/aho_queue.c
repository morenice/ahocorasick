#include "aho_queue.h"
#include <string.h>
#include <stdlib.h>

void aho_queue_init(struct aho_queue * restrict que)
{
    memset(que, 0x00, sizeof(struct aho_queue));
}

void aho_queue_destroy(struct aho_queue * restrict que)
{
    struct aho_queue_node* que_node = NULL;
    while ((que_node = aho_queue_dequeue(que)) != NULL)
    {
        free(que_node);
    }
}

bool aho_queue_enqueue(struct aho_queue * restrict que, struct aho_trie_node* node)
{
    struct aho_queue_node* que_node;
    que_node = (struct aho_queue_node*) malloc(sizeof(struct aho_queue_node));

    if (!que_node)
    {
        /* free memory error!! */
        return false;
    }

    memset(que_node, 0x00, sizeof(struct aho_queue_node));
    que_node->data = node;

    if (que->count == 0)
    {
        que->rear = que_node;
        que->front = que_node;
        que->count++;
        return true;
    }

    que_node->prev = que->rear;
    que->rear->next = que_node;
    que->rear = que_node;
    que->count++;

    return true;
}

inline bool aho_queue_empty(struct aho_queue * restrict que)
{
    return (que->count == 0);
}

struct aho_queue_node* aho_queue_dequeue(struct aho_queue * restrict que)
{
    struct aho_queue_node* deque_node;
    struct aho_queue_node* after_last_node;
    if (aho_queue_empty(que) == true)
    {
        return NULL;
    }

    if (que->count == 1)
    {
        deque_node = que->rear;
        que->front = que->rear = NULL;
        que->count--;
        return deque_node;
    }

    deque_node = que->rear;

    after_last_node = que->rear->prev;
    after_last_node->next = NULL;
    que->rear = after_last_node;
    que->count--;
    return deque_node;
}
