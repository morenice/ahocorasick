#include <string.h>
#include "keyword_tree.h"

void aho_init_keyword_tree(struct aho_keyword_tree* t)
{
    memset(t, 0x00, sizeof(struct aho_keyword_tree));
}

void aho_destroy_keyword_tree(struct aho_keyword_tree* t)
{

}

bool aho_add_keyword_tree(struct aho_keyword_tree* t, struct aho_keyworkd_tree_node* node)
{
    return true;
}

bool aho_del_keyword_tree(struct aho_keyword_tree* t, struct aho_keyworkd_tree_node* node)
{
    return true;
}

bool aho_clean_keyword_tree(struct aho_keyword_tree* t)
{
    return true;
}

