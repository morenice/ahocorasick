#pragma once
#include <stdbool.h>
#include <stdbool.h>

#define MAX_AHO_CHILD_NODE 256 /* Character 1 byte => 256 */

struct aho_trie_node
{
    unsigned char text;
    unsigned int ref_count;

    struct aho_trie_node* parent;
    struct aho_trie_node* child_list[MAX_AHO_CHILD_NODE];
    unsigned int child_count;

    bool text_end;
    struct aho_text_t* output_text; /* when text_end is true */

    struct aho_trie_node* failure_link;
    struct aho_trie_node* output_link;
};

struct aho_trie
{
    struct aho_trie_node root;
};

void aho_init_trie(struct aho_trie * restrict t);
void aho_destroy_trie(struct aho_trie * restrict t);

bool aho_add_trie_node(struct aho_trie * restrict t, struct aho_text_t * restrict text);
void aho_connect_link(struct aho_trie * restrict t);
void aho_clean_trie_node(struct aho_trie * restrict t);

struct aho_text_t* aho_find_trie_node(struct aho_trie_node** restrict start, const unsigned char text);

void aho_print_trie(struct aho_trie * restrict t);

/* TODO:
 * bool aho_del_trie_node(struct aho_trie* t, struct aho_text_t* text);
 */
