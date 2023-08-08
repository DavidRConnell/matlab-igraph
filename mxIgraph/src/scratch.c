#include <stdbool.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  void *a;
  bool b[] = { 0, 1, 1, 0 };

  a = b;

  printf("%g\n", (float)((bool *)a)[0]);
}
