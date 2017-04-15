#pragma once
#include <stdbool.h>

/* Character(1 byte) => 256
 * TODO: failure link, output link
 */
#define AHO_CHILD_NODE_COUNT 256

struct aho_keyword_tree_node
{
    char text;
    struct aho_keyword_tree_node* next[AHO_CHILD_NODE_COUNT];

    bool output_link;
    struct aho_text_t* output_text;
    struct aho_graph_node* failure_link;
};

struct aho_keyword_tree
{
    struct aho_keyworkd_tree_node* root;
};

void aho_init_keyword_tree(struct aho_keyword_tree* t);
void aho_destroy_keyword_tree(struct aho_keyword_tree* t);

bool aho_add_keyword_tree(struct aho_keyword_tree* t, struct aho_keyworkd_tree_node* node);
bool aho_del_keyword_tree(struct aho_keyword_tree* t, struct aho_keyworkd_tree_node* node);
bool aho_clean_keyword_tree(struct aho_keyword_tree* t);
