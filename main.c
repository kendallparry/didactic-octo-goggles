#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	//check if the usage is correct, if not, print instructions
	if (argc < 2 || argc>3){
		printf("Usage: ls2 <path> [exact-match-pattern]\n");
		return 0;
	}
	char* directory = argv[1];
	//2 args in command line, use mode 1
	if (argc == 2){
		lsMode1(directory, 0);
	}
	//3 args in command line, use mode 2 to search for pattern
	else {
		char* pattern = argv[2];
		stack_t* stack = initstack();
		lsMode2(directory, pattern, 0, stack);
		printstack(stack);
		free(stack);
	}

	return 0;
}
