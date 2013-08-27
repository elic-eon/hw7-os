#include<stdio.h>

int main()
{
  int n, i;
  scanf("%d", &n);
  printf("d %d\n", n);

  for (i = 1; i <= n; i++)
  {
    printf("1 %d\n", i);
  }
  for (i = 1; i <= n; i++)
  {
    printf("2 %d%s", (i%2) ? n-(i/2) : i/2, (i%10 != 0) ? " " : "\n");
  }
  printf("9\n");
  return 0;
}
