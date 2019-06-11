void print_board(int board[9]) {
  int row = 0;
  int col = 0;

  for (row = 0; row < 3; row = row + 1) {
    println("-------");
    for (col = 0; col < 3; col = col + 1) {
      print("|");
      if (board[row * 3 + col] == 1) {
        print("x");
      } else if (board[row * 3 + col] == 2) {
        print("c");
      } else {
        print(" ");
      }
      if (col == 2) {
        println("|");
      }
    }
    if (row == 2) {
      println("-------");
    }
  }
  return;
}

int check_win(int board[9]) {
  if (board[0] != 0 && board[0] == board[1] && board[1] == board[2]) {
    return board[0];
  }
  if (board[3] != 0 && board[3] == board[4] && board[4] == board[5]) {
    return board[3];
  }
  if (board[6] != 0 && board[6] == board[7] && board[7] == board[8]) {
    return board[6];
  }
  if (board[0] != 0 && board[0] == board[3] && board[3] == board[6]) {
    return board[0];
  }
  if (board[1] != 0 && board[1] == board[4] && board[4] == board[7]) {
    return board[1];
  }
  if (board[2] != 0 && board[2] == board[5] && board[5] == board[8]) {
    return board[2];
  }
  if (board[0] != 0 && board[0] == board[4] && board[4] == board[8]) {
    return board[0];
  }
  if (board[2] != 0 && board[2] == board[4] && board[4] == board[6]) {
    return board[2];
  }
  return 0;
}

int main() {
  int board[9];
  int i;
  for (i = 0; i < 9; i = i + 1) {
    board[i] = 0;
  }
  int player = 0;
  int cnt = 0;
  bool is_win = false;
  int win = 0;
  int x;
  int y;
  print_board(board);
  while (!is_win && cnt < 9) {
    if (player == 0) {
      println("Cross's turn:");
    } else {
      println("Circle's turn:");
    }
    input(x);
    input(y);
    if (player == 0) {
      board[x * 3 + y] = 1;
    } else {
      board[x * 3 + y] = 2;
    }
    player = (player + 1) % 2;
    win = check_win(board);
    if (win != 0) {
      is_win = true;
    }
    cnt = cnt + 1;
    print_board(board);
  }
  if (win == 1) {
    println("Cross Wins!");
  } else if (win == 2) {
    println("Circle Wins!");
  } else {
    println("Nobody Wins!");
  }
  return 0;
}