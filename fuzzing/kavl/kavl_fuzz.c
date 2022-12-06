#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "kavl.h"

#define CALLOC(type, num) ((type*)calloc(num, sizeof(type)))
#define MAX_LEN 1024
struct my_node {
	int key;
	KAVL_HEAD(struct my_node) head;
};

#define my_cmp(p, q) (((p)->key > (q)->key) - ((p)->key < (q)->key))
KAVL_INIT(my, struct my_node, head, my_cmp)

int check(struct my_node *p, int *hh)
{
	int c = 1, h[2] = {0, 0};
	*hh = 0;
	if (p) {
		if (p->head.p[0]) c += check(p->head.p[0], &h[0]);
		if (p->head.p[1]) c += check(p->head.p[1], &h[1]);
		*hh = (h[0] > h[1]? h[0] : h[1]) + 1;
		if (h[1] - h[0] != (int)p->head.balance)
			fprintf(stderr, "%d - %d != %d at %c\n", h[1], h[0], p->head.balance, p->key);
		if (c != (int)p->head.size)
			fprintf(stderr, "%d != %d at %c\n", p->head.size, c, p->key);
		return c;
	} else return 0;
}


int main(int argc, char *argv[])
{
	char buf[MAX_LEN];
	int i, n, h;
	struct my_node *root = 0;
	struct my_node *p, *q, t;
	kavl_itr_t(my) itr;
	unsigned cnt;
	char line1[MAX_LEN] = {0,};
	char line2[MAX_LEN] = {0,};
	

	
	/* Read Data */
    n = fread(line1, sizeof(*line1), sizeof(line1)/sizeof(*line1), stdin);
 	if( n <= 0 || n > MAX_LEN ) exit (0);
	
	/* Generate two inputs */	
	strncpy(line2, line1 + (n/2), n);
   	line1[n/2] = '\0';	
	n = n - (n/2);
	
	/* insert */	
	for (i = 0; i < n; ++i) 
	{
		p = CALLOC(struct my_node, 1);
		p->key = line1[i];
		q = kavl_insert(my, &root, p, &cnt);
		if (p != q) free(p);
		check(root, &h);
	}
	/* erase */
	for (i = 0; i < sizeof(line2)/sizeof(*line2); ++i) {
		t.key = line2[i];
		q = kavl_erase(my, &root, &t, 0);
		if (q) free(q);
		check(root, &h);
	}

	kavl_itr_first(my, root, &itr);
	do {
		const struct my_node *r = kavl_at(&itr);
		free((void*)r);
	} while (kavl_itr_next(my, &itr));
	putchar('\n');
	return 0;
}
