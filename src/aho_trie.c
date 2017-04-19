#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "aho_trie.h"
#include "aho_text.h"
#include "aho_queue.h"

void _aho_trie_node_init(struct aho_trie_node* node)
{
    memset(node, 0x00, sizeof(struct aho_trie_node));
    node->output_link = false;
    node->ref_count = 1; }

void aho_init_trie(struct aho_trie* t)
{
    memset(t, 0x00, sizeof(struct aho_trie));
    _aho_trie_node_init(&(t->root));
}

void aho_destroy_trie(struct aho_trie* t)
{
    aho_clean_trie_node(t);
}

bool aho_add_trie_node(struct aho_trie* t, struct aho_text_t* text)
{
    struct aho_trie_node* travasal_node = &(t->root);

    for (int text_idx = 0; text_idx < text->len; text_idx++)
    {
        char node_text = text->text[text_idx];
        bool find_node = false;
        int child_idx = 0;

        if (travasal_node->child_count == 0)
        {
            /* insert first node to child_list */
            travasal_node->child_list[0] =
                         (struct aho_trie_node*) malloc(sizeof(struct aho_trie_node));
            travasal_node->child_count++;

            _aho_trie_node_init(travasal_node->child_list[0]);
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

            _aho_trie_node_init(child_node);
            child_node->text = node_text;
            child_node->parent = travasal_node;

            travasal_node = child_node;
        }
    }

    // connect output link
    if (travasal_node)
    {
        travasal_node->output_link = true;
        travasal_node->output_text = text;
    }

    return true;
}

void aho_create_failure_link(struct aho_trie* t)
{

}

bool aho_clean_trie_node(struct aho_trie* t)
{
    return true;
}

void aho_print_trie(struct aho_trie* t)
{
    struct aho_queue queue;
    aho_queue_init(&queue);
    aho_queue_enqueue(&queue, &(t->root));

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
        // root node
        if(travasal_node->parent == NULL)
        {
            printf("root node %p\n", travasal_node);
            continue;
        }

        printf("%c node %p (parent %p)", travasal_node->text, travasal_node, travasal_node->parent);
        if (travasal_node->output_link)
        {
            printf(" output_link(%d)\n", travasal_node->output_text->id);
        }
        else
        {
            printf("\n");
        }
    }

    aho_queue_dequeue(&queue);
}

