#pragma once

struct aho_text_t
{
    int id;
    char* text;
    int len;
    struct aho_text_t *prev, *next;
};
