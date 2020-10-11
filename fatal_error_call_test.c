#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  int i = 0;
  free(&i);
  i;
  return 0;
}
