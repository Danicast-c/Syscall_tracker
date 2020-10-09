#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("Now going to sleep\n");
  sleep(2);
  printf("Came back, but sleeping again\n");
  sleep(2);
  printf("Bye-bye!\n");
  return 0;
}
