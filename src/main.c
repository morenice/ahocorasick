#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "ahocorasick.h"

void matched(void *arg, struct aho_match_t* m)
{
    long long int* match_total = (long long int*)arg;
    (*match_total)++;
    //printf("matched id %d position %llu length %d\n", m->id, m->pos, m->len);
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
    aho_register_match_callback(&aho, matched, &match_total);

    printf("try: %s\n", "dabcacdfc");
    printf("total match:%u\n", aho_findtext(&aho, "dabcacdfc", strlen("dabcacdfc")));

    aho_destroy(&aho);
}

void sample_code_bench()
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
    aho_register_match_callback(&aho, matched, &match_total);

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
    aho_register_match_callback(&aho, matched, &match_total);

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
	//sample_code();
    //sample_code_bench();
    sample_code_thread();
    return 0;
}
