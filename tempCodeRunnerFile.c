#include <stdio.h>

void function(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}



int main(void) {
	int x = 5;
	int y = 10;	
	function(&x,&y);
	if (x == 5) {
		printf("heaven is a good girl");
	}

	else {
		printf("u smell like shit");
	}
} 