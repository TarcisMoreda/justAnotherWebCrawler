#include "jawc.h"
#include <stdio.h>

int main()
{
  if (!jawc_init())
  {
    fprintf(stderr, "Failed to initialize jawc.\n");
    return 1;
  }

  printf("jawc init successful.\n");

  jawc_destroy();
}