#pragma once

#include <stdbool.h>
#include "aho_trie.h"
#include "aho_text.h"

struct aho_match_t
{
    int id;
    unsigned long long pos;
    int len;
};

struct ahocorasick
{
#define AHO_MAX_TEXT_ID INT_MAX
    int accumulate_text_id;
    struct aho_text_t* text_list_head;
    struct aho_text_t* text_list_tail;
    int text_list_len;

    struct aho_trie trie;

    void (*callback_match)(void* arg, struct aho_match_t*);
    void* callback_arg;
};

void aho_init(struct ahocorasick* aho);
void aho_destroy(struct ahocorasick* aho);

int aho_add_match_text(struct ahocorasick* aho, const char* text, unsigned int len);
bool aho_del_match_text(struct ahocorasick* aho, int id);
void aho_clear_match_text(struct ahocorasick* aho);

void aho_create_trie(struct ahocorasick* aho);
void aho_clear_trie(struct ahocorasick* aho);

unsigned int aho_findtext(struct ahocorasick* aho, const char* data, unsigned long long data_len);

void aho_register_match_callback(struct ahocorasick *aho,
        void (*callback_match)(void* arg, struct aho_match_t*),
        void *arg);

/* for debug */
void aho_print_match_text(struct ahocorasick* aho);
