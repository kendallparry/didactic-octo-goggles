#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 For testing valgrind
*/
int main(int argc, char *argv[]) {
  int *buf1 = malloc(sizeof(int) * 20);

  for (int i = 0; i < 20; i++) {
    buf1[i] = 0;
  }
  buf1[20] = 0; // <---- Line 11 BUG: out-of-bounds (valid indices are 0..19)
  return 0;
}