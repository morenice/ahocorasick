#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ahocorasick.h"
#include "aho_trie.h"

void aho_init(struct ahocorasick * restrict aho)
{
    memset(aho, 0x00, sizeof(struct ahocorasick));
}

void aho_destroy(struct ahocorasick * restrict aho)
{
    aho_clear_match_text(aho);
    aho_clear_trie(aho);
}

int aho_add_match_text(struct ahocorasick * restrict aho, const char* text, unsigned int len)
{
    struct aho_text_t* a_text = NULL;
    if (aho->accumulate_text_id == AHO_MAX_TEXT_ID)
    {
        return -1;
    }

    a_text = (struct aho_text_t*) malloc(sizeof(struct aho_text_t));
    if (!a_text)
        goto lack_free_mem;

    a_text->text = (char*) malloc(sizeof(char)*len);
    if (!a_text->text)
        goto lack_free_mem;

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

lack_free_mem:
    return -1;
}

bool aho_del_match_text(struct ahocorasick * restrict aho, const int id)
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
                free(iter->text);
            }
            else if (iter == aho->text_list_tail)
            {
                aho->text_list_tail = iter->prev;
                free(iter->text);
            }
            else
            {
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
                free(iter->text);
            }
            free(iter);
            aho->text_list_len--;
            return true;
        }
    }
    return false;
}

void aho_clear_match_text(struct ahocorasick * restrict aho)
{
    for(int i = 0; i < aho->accumulate_text_id; i++)
    {
        aho_del_match_text(aho, i);
    }

    // reset id
    aho->accumulate_text_id = 0;
}


void aho_create_trie(struct ahocorasick * restrict aho)
{
    struct aho_text_t* iter = NULL;
    aho_init_trie(&(aho->trie));

    for (iter = aho->text_list_head; iter != NULL; iter = iter->next)
    {
        aho_add_trie_node(&(aho->trie), iter);
    }

    aho_connect_link(&(aho->trie));

    /* debugging */
    //aho_print_trie(&(aho->trie));
}

void aho_clear_trie(struct ahocorasick * restrict aho)
{
    aho_destroy_trie(&aho->trie);
}

unsigned int aho_findtext(struct ahocorasick * restrict aho, const char* data, unsigned long long data_len)
{
    int i = 0;
    int match_count = 0;
    struct aho_trie_node* travasal_node = NULL;

    travasal_node = &(aho->trie.root);

    for (i = 0; i < data_len; i++)
    {
        struct aho_match_t match;
        struct aho_text_t* result;

        result = aho_find_trie_node(&travasal_node, data[i]);
        if (result == NULL)
        {
            continue;
        }

        match.id = result->id;
        match.len = result->len;

        match.pos = i - result->len + 1;
        if (result->len == 1)
        {
            match.pos = i;
        }

        match_count++;
        if (aho->callback_match)
        {
            aho->callback_match(aho->callback_arg, &match);
        }
    }

    return match_count;
}

inline void aho_register_match_callback(struct ahocorasick * restrict aho,
        void (*callback_match)(void* arg, struct aho_match_t*),
        void *arg)
{
    aho->callback_arg = arg;
    aho->callback_match = callback_match;
}

void aho_print_match_text(struct ahocorasick * restrict aho)
{
    struct aho_text_t* iter = NULL;
    for (iter = aho->text_list_head; iter != NULL; iter = iter->next)
    {
        printf("id:%d text:%s len:%d this:%p prev:%p next:%p\n",
                iter->id, iter->text, iter->len, iter, iter->prev, iter->next);
    }
}
