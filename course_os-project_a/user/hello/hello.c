#include <stdio.h>
#include "../libc/arch/arm/syscall_arch.h"
#include <fs_syscalls.h>
#include <thread.h>
#include <assert.h>
#include <stdio.h>

void* f(void* arg)
{
   printf("this is the thread\n");

   assert(((int)arg) == 1);

   thread_exit(NULL);
}

void runThreadTest()
{
   thread_t thread;

   printf("before thread_create\n");

   int rc = thread_create(&thread, f, (void*) 1);
   assert(rc == 0);

   printf("after thread_create\n");

   while(1) { }
}
int main() {
	__syscall3(99, 0, 0, 0);

	printf("Hello world... from hello.c\n");

	printf("LET'S TEST %d\n", 10);

	int* mem = 0;
	mem = (int*) malloc(100);

	printf("malloc returned %x\n", mem);

	mem[0] = 1;
	mem[10] = 2;

	runThreadTest(); 

    free(mem);

    printf("hello success\n");

    while(1);
}
