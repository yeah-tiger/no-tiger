#ifdef __GNUC__
#include <stdio.h>
typedef int int32_t;
#elif __MSC_VER__
#include <stdio.h>
typedef int int32_t;
#elif __MINGW32__
#include <stdio.h>
typedef int int32_t;
#endif

int32_t fib(int32_t n) {
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  }
  return fib(n - 1) + fib(n - 2);
}

int32_t main() {
  int32_t n = 10;
  printf("%d", fib(n));
  return 0;
}