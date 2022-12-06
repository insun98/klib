#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kstring.h"

int nfail = 0;

void check(const char *what, const kstring_t *ks, const char *correct)
{
	if (ks->l != strlen(correct) || strcmp(ks->s, correct) != 0) {
		fprintf(stderr, "%s produced \"%.*s\" (\"%s\" is correct)\tFAIL\n", what, (int)(ks->l), ks->s, correct);
		nfail++;
	}
}

void test_kputw(kstring_t *ks, int n)
{
	char buf[16];

	ks->l = 0;
	kputw(n, ks);

	sprintf(buf, "%d", n);
	check("kputw()", ks, buf);
}

void test_kputl(kstring_t *ks, long n)
{
	char buf[24];

	ks->l = 0;
	kputl(n, ks);

	sprintf(buf, "%ld", n);
	check("kputl()", ks, buf);
}

static char *mem_gets(char *buf, int buflen, void *vtextp)
{
	const char **textp = (const char **) vtextp;

	const char *nl = strchr(*textp, '\n');
	size_t n = nl? nl - *textp + 1 : strlen(*textp);

	if (n == 0) return NULL;

	if (n > buflen-1) n = buflen-1;
	memcpy(buf, *textp, n);
	buf[n] = '\0';
	*textp += n;
	return buf;
}

void test_kgetline(kstring_t *ks, const char *text, ...)
{
	const char *exp;
	va_list arg;

	va_start(arg, text);
	while ((exp = va_arg(arg, const char *)) != NULL) {
		ks->l = 0;
		if (kgetline(ks, mem_gets, &text) != 0) kputs("EOF", ks);
		check("kgetline()", ks, exp);
	}
	va_end(arg);

	ks->l = 0;
	if (kgetline(ks, mem_gets, &text) == 0) check("kgetline()", ks, "EOF");
}

int main(int argc, char **argv)
{

	int data_size, element;
	char temp[256], line1[256], line2[256];

	kstring_t ks;
	ks.l = ks.m = 0;
	ks.s = NULL;
	
	scanf("%s", temp);
	data_size = atoi(temp);
	
	for(int i = 0; i < data_size; i++){
		scanf("%s", temp);
		element = atoi(temp);	
		test_kputw(&ks, element);
		test_kputl(&ks, element);
	}
	
	scanf("%s", line1);
	scanf("%s", line2);
	test_kgetline(&ks, line1, line2, NULL);

	return EXIT_SUCCESS;
}
