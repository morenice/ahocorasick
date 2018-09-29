#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "ahocorasick.h"

void callback_match_total(void *arg, struct aho_match_t* m)
{
    long long int* match_total = (long long int*)arg;
    (*match_total)++;
}

void callback_match_pos(void *arg, struct aho_match_t* m)
{
    char* text = (char*)arg;

    printf("match text:");
    for(unsigned int i = m->pos; i < (m->pos+m->len); i++)
    {
        printf("%c", text[i]);
    }

    printf(" (match id: %d position: %llu length: %d)\n", m->id, m->pos, m->len);
}

void sample_code()
{
    struct ahocorasick aho;
    long long int match_total = 0;
    int id[10] = {0};

    aho_init(&aho);

    id[0] = aho_add_match_text(&aho, "ab", 2);
    id[1] = aho_add_match_text(&aho, "c", 1);
    id[2] = aho_add_match_text(&aho, "a", 1);
    id[3] = aho_add_match_text(&aho, "acd", 3);

    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_total, &match_total);

    printf("try: %s\n", "dabcacdfc");
    printf("total match:%u\n", aho_findtext(&aho, "dabcacdfc", strlen("dabcacdfc")));

    aho_destroy(&aho);
}

void sample_code2()
{
    struct ahocorasick aho;
    int id[10] = {0};
    char* target = "Hello world, C언어 공부하자. ab abcde";

    aho_init(&aho);

    id[0] = aho_add_match_text(&aho, "C언어", strlen("C언어"));
    id[2] = aho_add_match_text(&aho, "공부하자", strlen("공부하자"));
    id[3] = aho_add_match_text(&aho, "공부", strlen("공부"));
    id[3] = aho_add_match_text(&aho, "abcd", 4);
    id[3] = aho_add_match_text(&aho, "bc", 2);

    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_pos, (void*)target);

    printf("try: %s\n", target);
    printf("total match:%u\n", aho_findtext(&aho, target, strlen(target)));

    aho_destroy(&aho);
}

void sample_code3()
{
    struct ahocorasick aho;
    int id[10] = {0};
    char* target = "Lorem ipsum dolor sit amet, consectetur brown elit. Proin vehicula brown egestas. Aliquam a dui tincidunt, elementum sapien in, ultricies lacus. Phasellus congue, sapien nec";
    aho_init(&aho);

    id[0] = aho_add_match_text(&aho, "consectetur", strlen("consectetur"));
    id[2] = aho_add_match_text(&aho, "Proin", strlen("Proin"));
    id[3] = aho_add_match_text(&aho, "egestasAliquam", strlen("egestasAliquam"));
    id[3] = aho_add_match_text(&aho, "elementum", strlen("elementum"));
    id[4] = aho_add_match_text(&aho, "ultricies", strlen("ultricies"));
    id[5] = aho_add_match_text(&aho, "vehicula", strlen("vehicula"));
    
    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_pos, (void*)target);

    printf("try: %s\n", target);
    printf("total match:%u\n", aho_findtext(&aho, target, strlen(target)));

    aho_destroy(&aho);
}

void sample_code_bench_file()
{
    struct ahocorasick aho;
    long long int match_total = 0;
    int id[10] = {0};

    FILE *fp = NULL;
    char buf[4096] = {0};

    aho_init(&aho);

    id[0] = aho_add_match_text(&aho, "1984", 4);
    id[1] = aho_add_match_text(&aho, "1985", 4);

    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_total, &match_total);

    fp = fopen("googlebooks-eng-all-1gram-20120701-0", "r");
    while (fgets(buf, 4096, fp) != NULL)
    {
        aho_findtext(&aho, buf, strlen(buf));
    }

    printf("%lld\n", match_total);

    aho_destroy(&aho);
    fclose(fp);
}

void* __sample_code_thread(void *t)
{
	struct ahocorasick* aho = (struct ahocorasick*)t;

	printf("try: %s\n", "dabcacdfc");
	printf("total match:%u\n", aho_findtext(aho, "dabcacdfc", strlen("dabcacdfc")));

	pthread_exit((void*) t);
}


void sample_code_thread()
{
#define NUM_THREADS 10
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	int rc;
	void *status;
    long long int match_total = 0;

    struct ahocorasick aho;
    int id[10] = {0};

	/* Initialize ahocorasick */
    aho_init(&aho);
    id[0] = aho_add_match_text(&aho, "ab", 2);
    id[1] = aho_add_match_text(&aho, "c", 1);
    id[2] = aho_add_match_text(&aho, "a", 1);
    id[3] = aho_add_match_text(&aho, "acd", 3);

    aho_create_trie(&aho);
    aho_register_match_callback(&aho, callback_match_total, &match_total);

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(int tid=0; tid<NUM_THREADS; tid++) {
		printf("creating thread %d\n", tid);
		rc = pthread_create(&thread[tid], &attr, __sample_code_thread, (void *)&aho);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
            return;
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(int tid=0; tid<NUM_THREADS; tid++) {
		rc = pthread_join(thread[tid], &status);
		if (rc) {
            return;
		}
		printf("completed join with thread %d having a status of %ld\n", tid, (long)status);
	}

	printf("program completed.\n");
    aho_destroy(&aho);
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
	sample_code();
	sample_code2();
    sample_code3();
    //sample_code_thread();
    //sample_code_bench_file();
    return 0;
}
