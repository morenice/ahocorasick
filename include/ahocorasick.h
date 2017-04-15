#pragma once
#include <stdbool.h>
#include "keyword_tree.h"

struct aho_text_t
{
    int id;
    char* text;
    int len;
    struct aho_text_t *prev, *next;
};

struct aho_match_t
{
    int id;
    unsigned long long pos;
    unsigned long long len;
};

/* TODO: multi threading
 * 패턴 업데이트는 많지 않을 것이다. RCU lock과 같은 매커니즘이 필요하지 않을까...
 * list를 이동하는 포인터 정보 findtext를 부를 때 마다 만들어서 사용하면 문제가 되지 않을 듯
 */
struct ahocorasick
{
    int accumulate_text_id;
    struct aho_text_t* text_list_head;
    struct aho_text_t* text_list_tail;
    int text_list_len;

    struct aho_keyword_tree tree;
};

void aho_init(struct ahocorasick* aho);
void aho_destroy(struct ahocorasick* aho);

int aho_add_match_text(struct ahocorasick* aho, const char* text, unsigned int len);
bool aho_del_match_text(struct ahocorasick* aho, int id);
void aho_clear_match_text(struct ahocorasick* aho);
void aho_print_match_text(struct ahocorasick* aho);

void aho_create_keyword_tree(struct ahocorasick* aho);
void aho_clear_keyword_tree(struct ahocorasick* aho);

struct aho_match_t* aho_findtext(struct ahocorasick* aho, const char* target);

