#include <stdio.h>
#include <stdlib.h>
#include "malloc_struct.h"

int main(void){
	void *ma, *mb;

	setbuf(stdout, NULL);

	ma = malloc(0x18);	// A
	mb = malloc(0x18);	// B
	printf("ma = %p, mb = %p\n", ma, mb);

	// fill up tcache
	for(int i=0; i<7; i++)
		free(calloc(1, 0x18));

	free(ma);
	free(mb);
	// fasbinsは追加チェックのときに
	// 先頭から入れたいから、先頭と
	// 入れたいものが同一でなければ問答無用で入れてしまう
	free(ma);		// vuln vulnerability 脆弱性

	printf("1st calloc : %p\n", calloc(1, 0x18));
	printf("2nd calloc : %p\n", calloc(1, 0x18));
	printf("3rd calloc : %p\n", calloc(1, 0x18));
}
