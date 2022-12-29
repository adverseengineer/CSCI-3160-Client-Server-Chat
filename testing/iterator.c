

#include <stdio.h>
#include <stdlib.h>

#define ITERATOR(v, start, end) (size_t (v) = (start); (v) < (end); (v)++)

int main(int argc, char** argv) {

	for ITERATOR(i, 0, 1000) {
	
		printf("%zu\n", i);
	}
	
	return EXIT_SUCCESS;
}
