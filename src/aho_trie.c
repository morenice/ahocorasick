#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "aho_trie.h"
#include "aho_text.h"
#include "aho_queue.h"

void __aho_trie_node_init(struct aho_trie_node * restrict node)
{
    memset(node, 0x00, sizeof(struct aho_trie_node));
    node->text_end = false;
    node->ref_count = 1;
}

void aho_init_trie(struct aho_trie * restrict t)
{
    memset(t, 0x00, sizeof(struct aho_trie));
    __aho_trie_node_init(&(t->root));
}

void aho_destroy_trie(struct aho_trie * restrict t)
{
    aho_clean_trie_node(t);
}

bool aho_add_trie_node(struct aho_trie * restrict t, struct aho_text_t * restrict text)
{
    struct aho_trie_node* travasal_node = &(t->root);

    for (int text_idx = 0; text_idx < text->len; text_idx++)
    {
        unsigned char node_text = text->text[text_idx];
        bool find_node = false;
        int child_idx = 0;

        if (travasal_node->child_count == 0)
        {
            /* insert first node to child_list */
            travasal_node->child_list[0] =
                         (struct aho_trie_node*) malloc(sizeof(struct aho_trie_node));
            travasal_node->child_count++;

            __aho_trie_node_init(travasal_node->child_list[0]);
            travasal_node->child_list[0]->text = node_text;
            travasal_node->child_list[0]->parent = travasal_node;

            travasal_node = travasal_node->child_list[0];
            continue;
        }

        if (travasal_node->child_count == MAX_AHO_CHILD_NODE)
        {
            return false;
        }

        for (child_idx=0; child_idx < travasal_node->child_count; child_idx++)
        {
            if (travasal_node->child_list[child_idx]->text == node_text )
            {
                find_node = true;
                break;
            }
        }

        if (find_node == true)
        {
            travasal_node->child_list[child_idx]->ref_count++;
            travasal_node = travasal_node->child_list[child_idx];
        }
        else
        {
            /* push_back to child_list */
            struct aho_trie_node* child_node = NULL;

            travasal_node->child_list[travasal_node->child_count] =
                         (struct aho_trie_node*) malloc(sizeof(struct aho_trie_node));

            child_node = travasal_node->child_list[travasal_node->child_count];
            travasal_node->child_count++;

            __aho_trie_node_init(child_node);
            child_node->text = node_text;
            child_node->parent = travasal_node;

            travasal_node = child_node;
        }
    }

    // connect output link
    if (travasal_node)
    {
        travasal_node->text_end = true;
        travasal_node->output_text = text;
    }
    return true;
}

bool __aho_connect_link(struct aho_trie_node* p, struct aho_trie_node* q)
{
    struct aho_trie_node *pf = NULL;
    int i = 0;

    /* is root node */
    if (p->parent == NULL)
    {
        q->failure_link = p;
        return true;
    }

    pf = p->failure_link;
    for (i=0; i < pf->child_count; i++)
    {
        /* check child node of failure link(p) */
        if (pf->child_list[i]->text == q->text )
        {
            /* connect failure link */
            q->failure_link = pf->child_list[i];

            /* connect output link */
            if (pf->child_list[i]->text_end)
            {
                q->output_link = pf->child_list[i];
            }
            else
            {
                q->output_link = pf->child_list[i]->output_link;
            }
            return true;
        }
    }
    return false;
}

void aho_connect_link(struct aho_trie * restrict t)
{
    struct aho_queue queue;
    aho_queue_init(&queue);
    aho_queue_enqueue(&queue, &(t->root));

    /* BFS access
     *  connect failure link and output link
     */
    while (true)
    {
        /* p :parent, q : child node */
        struct aho_queue_node *queue_node = NULL;
        struct aho_trie_node *p = NULL;
        struct aho_trie_node *q = NULL;
        int i = 0;

        queue_node = aho_queue_dequeue(&queue);
        if (queue_node == NULL)
        {
            break;
        }

        p = queue_node->data;
        free(queue_node);

        /* get child node list of p */
        for (i=0; i < p->child_count; i++)
        {
            struct aho_trie_node *pf = p;

            aho_queue_enqueue(&queue, p->child_list[i]);
            q = p->child_list[i];

            while (__aho_connect_link(pf, q) == false)
            {
                pf = pf->failure_link;
            }
        }
    }

    aho_queue_destroy(&queue);
}

void aho_clean_trie_node(struct aho_trie * restrict t)
{
    struct aho_queue queue;
    aho_queue_init(&queue);
    aho_queue_enqueue(&queue, &(t->root));

    /* BFS */
    while (true)
    {
        struct aho_queue_node *queue_node = NULL;
        struct aho_trie_node *remove_node = NULL;
        int i = 0;

        queue_node = aho_queue_dequeue(&queue);
        if (queue_node == NULL)
        {
            break;
        }

        remove_node = queue_node->data;
        free(queue_node);

        for (i=0; i < remove_node->child_count; i++)
        {
            aho_queue_enqueue(&queue, remove_node->child_list[i]);
        }

        /* is root node */
        if (remove_node->parent == NULL)
        {
            continue;
        }

        free(remove_node);
    }
}

bool __aho_find_trie_node(struct aho_trie_node** restrict start, const unsigned char text)
{
    struct aho_trie_node* search_node = NULL;
    int i = 0;

    search_node = *start;
    for (i = 0; i < search_node->child_count; i++)
    {
        if (search_node->child_list[i]->text == text)
        {
            /* find it! move to find child node! */
            *start = search_node->child_list[i];
            return true;
        }
    }

    /* not found */
    return false;
}

struct aho_text_t* aho_find_trie_node(struct aho_trie_node** restrict start, const unsigned char text)
{
    while (__aho_find_trie_node(start, text) == false)
    {
        /* not found!
         * when root node stop
         */
        if( (*start)->parent == NULL)
        {
            return NULL;
        }

        /* retry find. move failure link. */
        *start = (*start)->failure_link;
    }

    /* found node... */
    /* match case1: find text end! */
    if ((*start)->text_end)
    {
        return (*start)->output_text;
    }

    /* match case2: exist output_link */
    if ((*start)->output_link)
    {
        return (*start)->output_link->output_text;
    }

    /* keep going */
    return NULL;
}

void aho_print_trie(struct aho_trie * restrict t)
{
    struct aho_queue queue;
    aho_queue_init(&queue);
    aho_queue_enqueue(&queue, &(t->root));

    /* BFS */
    while (true)
    {
        struct aho_queue_node *queue_node = NULL;
        struct aho_trie_node *travasal_node = NULL;
        int i = 0;

        queue_node = aho_queue_dequeue(&queue);
        if (queue_node == NULL)
        {
            break;
        }

        travasal_node = queue_node->data;
        free(queue_node);

        for (i=0; i < travasal_node->child_count; i++)
        {
            aho_queue_enqueue(&queue, travasal_node->child_list[i]);
        }

        /* is root node */
        if(travasal_node->parent == NULL)
        {
            printf("root node %p\n", travasal_node);
            continue;
        }

        printf("%c (textend:%d) node %p ref %u (parent %p) failure_link(%p) output_link(%p)",
                travasal_node->text, travasal_node->text_end,
                travasal_node, travasal_node->ref_count,
                travasal_node->parent, travasal_node->failure_link,
                travasal_node->output_link);

        printf("\n");
    }

    aho_queue_destroy(&queue);
}
