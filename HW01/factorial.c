#include <stdio.h>

int main()
{
  double n = 3154000000000000;
  int i = 1;
  while(n > i){
    n = n / i;
    printf("%f\n",n);
    i++;
  }
  printf("%d\n", i - 1);

  return 0;
}
