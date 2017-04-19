#include <stdio.h>
#include "ahocorasick.h"

int main(int argc, const char *argv[])
{
    struct ahocorasick aho;
    int id[10] = {0};

    printf("hello world ahocorasick\n");

    aho_init(&aho);
    id[0] = aho_add_match_text(&aho, "asdf", 4);
    id[1] = aho_add_match_text(&aho, "asdf1", 5);
    id[2] = aho_add_match_text(&aho, "asdf2", 5);
    id[3] = aho_add_match_text(&aho, "cc", 2);
    id[4] = aho_add_match_text(&aho, "ccd", 3);
    id[5] = aho_add_match_text(&aho, "ceef", 4);

    aho_print_match_text(&aho);

    aho_create_trie(&aho);

    //aho_findtext(&aho, "asdfasdf", &m);
    aho_destroy(&aho);

    return 0;
}
