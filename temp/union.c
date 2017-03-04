#include<stdio.h>
struct test{
	union{
	unsigned int f[4];
	struct{
		unsigned int a, b, c, d;
		};
	};
	unsigned int e;
};
int main(){
	struct test t1={1, 2, 3, 4};
	printf("%d\n", sizeof(t1)); 
	printf("%d, %d, %d, %d, %d", t1.a, t1.b, t1.c, t1.d, t1.e);
	printf("%d\n", sizeof(int));
	return 0;
}
