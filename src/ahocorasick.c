#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ahocorasick.h"
#include "keyword_tree.h"

void aho_init(struct ahocorasick* aho)
{
    memset(aho, 0x00, sizeof(struct ahocorasick));
}

void aho_destroy(struct ahocorasick* aho)
{
    aho_clear_match_text(aho);
    aho_clear_keyword_tree(aho);
}

int aho_add_match_text(struct ahocorasick* aho, const char* text, unsigned int len)
{
    struct aho_text_t* a_text = NULL;
    if (aho->accumulate_text_id + 1 == INT_MAX)
    {
        return -1;
    }

    a_text = (struct aho_text_t*) malloc(sizeof(struct aho_text_t));
    a_text->text = (char*) malloc(sizeof(char)*len);

    a_text->id = aho->accumulate_text_id++;
    memcpy(a_text->text, text, len);
    a_text->len = len;
    a_text->prev = NULL;
    a_text->next = NULL;

    if (aho->text_list_head == NULL)
    {
        aho->text_list_head = a_text;
        aho->text_list_tail = a_text;
        aho->text_list_len++;
        return a_text->id;
    }

    aho->text_list_tail->next = a_text;
    a_text->prev = aho->text_list_tail;
    aho->text_list_tail = a_text;
    aho->text_list_len++;
    return a_text->id;
}

bool aho_del_match_text(struct ahocorasick* aho, int id)
{
    struct aho_text_t* iter = NULL;
    for (iter = aho->text_list_head; iter != NULL; iter = iter->next)
    {
        /*if (iter->id > id)
        {
            return false;
        }
        */

        if (iter->id == id)
        {
            if (iter == aho->text_list_head)
            {
                aho->text_list_head = iter->next;
            }
            else if (iter == aho->text_list_tail)
            {
                aho->text_list_tail = iter->prev;
            }
            else
            {
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
            }
            free(iter);
            aho->text_list_len--;
            return true;
        }
    }
    return false;
}

void aho_clear_match_text(struct ahocorasick* aho)
{
    for(int i = 0; i < aho->accumulate_text_id; i++)
    {
        aho_del_match_text(aho, i);
    }
}

void aho_print_match_text(struct ahocorasick* aho)
{
    struct aho_text_t* iter = NULL;
    for (iter = aho->text_list_head; iter != NULL; iter = iter->next)
    {
        printf("id:%d text:%s len:%d this:%p prev:%p next:%p\n",
                iter->id, iter->text, iter->len, iter, iter->prev, iter->next);
    }
}

void aho_create_keyword_tree(struct ahocorasick* aho)
{

}

void aho_clear_keyword_tree(struct ahocorasick* aho)
{

}

struct aho_match_t* aho_findtext(struct ahocorasick* aho, const char* target)
{

    return NULL;
}
