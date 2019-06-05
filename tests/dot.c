int rand(int seed) { return (35121 * seed + 56437) % 56437; }

int dot(int in1[9], int in2[9], int out[9], int n) {
  int i = 0;
  for (i = 0; i < n; i = i + 1) {
    out[i] = in1[i] * in2[i];
  }
  return 0;
}

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
    int i = 0;
    int vector3[9];
    int res = dot(vector1, vector2, vector3, 9);
    if (res == 0) {
      while (i < 9) {
        println(vector3[i]);
        i = i + 1;
      }
    } else {
      println("error occurred");
    }
  }
  return 0;
}