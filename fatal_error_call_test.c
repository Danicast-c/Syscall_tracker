#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("Now doing a division by zero\n");
  int one = 1;
  int zero = 0;
  printf("1/0 = %d\n", one / zero);
  return 0;
}
