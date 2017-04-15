#include <stdio.h>
#include "ahocorasick.h"

int main(int argc, const char *argv[])
{
    struct ahocorasick aho;
    int id, id2, id3;

    printf("hello world ahocorasick\n");

    aho_init(&aho);
    id = aho_add_match_text(&aho, "asdf", 4);
    id2 = aho_add_match_text(&aho, "asdf1", 5);
    id3 = aho_add_match_text(&aho, "asdf2", 5);

    aho_print_match_text(&aho);

    //aho_create_keyword_tree(&aho);
    //aho_clear_keyword_tree(&aho);

    //aho_findtext(&aho, "asdfasdf", &m);
    aho_destroy(&aho);

    return 0;
}
