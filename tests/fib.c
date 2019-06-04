int fib(int n) {
  if (n < 0) {
    return -1;
  }
  if (n <= 1) {
    return n;
  }
  return fib(n - 1) + fib(n - 2);
}

int main() {
  int n;
  println("Please input a number between 0 to 10");
  input(n);
  print("Your input number is: ");
  println(n);
  if (n < 0) {
    println("Your input number is too small");
    return 1;
  } else if (n > 10) {
    println("Your input number is too big");
    return 1;
  } else {
    println("Your input number is just fine");
    print("The Nth fibonacci number is: ");
    print(fib(n));
  }
  return 0;
}