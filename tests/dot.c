int rand(int seed) { return (35121 * seed + 56437) % 56437; }

int main() {
  int vector1[9];
  int vector2[9];
  int i = 0;
  int seed = 245123;
  for (i = 0; i < 9; i = i + 1) {
    seed = vector1[i] = rand(seed);
    seed = vector2[i] = rand(seed);
    vector1[i] = vector1[i] % 15;
    vector2[i] = vector2[i] % 15;
  }
  println("random number results:");
  for (i = 0; i < 9; i = i + 1) {
    print(vector1[i]);
    print(", ");
    println(vector2[i]);
  }
  println("dot product");
  {
    int i;
    int vector3[9];
    while (i < 9) {
      vector3[i] = vector2[i] * vector1[i];
      println(vector3[i]);
      i = i + 1;
    }

  }
  return 0;
}