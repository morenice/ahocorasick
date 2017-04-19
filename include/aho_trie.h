#pragma once
#include <stdbool.h>
#include <stdbool.h>

#define MAX_AHO_CHILD_NODE 256 /* Character 1 byte => 256 */

struct aho_trie_node
{
    char text;

    struct aho_trie_node* parent;
    struct aho_trie_node* child_list[MAX_AHO_CHILD_NODE];
    unsigned int child_count;

    bool output_link;
    struct aho_text_t* output_text;
    struct aho_trie_node* failure_link;
    unsigned int ref_count;
};

struct aho_trie
{
    struct aho_trie_node root;
};

void aho_init_trie(struct aho_trie* t);
void aho_destroy_trie(struct aho_trie* t);

bool aho_add_trie_node(struct aho_trie* t, struct aho_text_t* text);
void aho_create_failure_link(struct aho_trie* t);
bool aho_clean_trie_node(struct aho_trie* t);

void aho_print_trie(struct aho_trie* t);

/* TODO:
 * bool aho_del_trie_node(struct aho_trie* t, struct aho_text_t* text);
 */
