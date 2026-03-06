#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>

const int N = 9;

std::mt19937 g(static_cast<unsigned int>(std::time(0)));

void printBoard(int board[N][N]) {
    std::cout << "     0 1 2   3 4 5   6 7 8" << std::endl;
    std::cout << "    -------------------------" << std::endl;
    for (int r = 0; r < N; r++) {
        if (r == 3 || r == 6) std::cout << "    -------------------------" << std::endl;
        std::cout << r << " |  ";
        for (int c = 0; c < N; c++) {
            if (c == 3 || c == 6) {
                std::cout << "| ";
            }
            if (board[r][c] == 0) {
                std::cout << ". ";
            }
            else {
                std::cout << board[r][c] << " ";
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "    -------------------------" << std::endl;
}

bool isValid(int board[N][N], int row, int col, int num) {
    for (int i = 0; i < N; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    return true;
}

bool fillBoard(int board[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                std::vector<int> numbers;
                for (int i = 1; i <= 9; ++i) {
                    numbers.push_back(i);
                }
                std::shuffle(numbers.begin(), numbers.end(), g);
                for (size_t i = 0; i < numbers.size(); ++i) {
                    int num = numbers[i];
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (fillBoard(board)) {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void removeNumbers(int board[N][N], int count) {
    std::uniform_int_distribution<int> dist(0, N * N - 1);
    while (count > 0) {
        int cell = dist(g);
        int r = cell / N;
        int c = cell % N;
        if (board[r][c] != 0) {
            board[r][c] = 0;
            count--;
        }
    }
}

int main() {
    int board[N][N] = {0};
    int solution[N][N] = {0};
    std::cout << "Generating a new Sudoku board..." << std::endl;
    fillBoard(board);
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            solution[i][j] = board[i][j];
        }
    }
    int count;
    while (true) {
        std::cout << "Select Difficulty:\n" << "1.Easy\n" << "2.Medium\n" << "3.Hard\n";
        int difficult;
        std::cin >> difficult;
        if(difficult == 1) {
            count = 30;
            std::cout << "Easy\n";
            break;
        } else if (difficult == 2) {
            count = 45;
            std::cout << "Medium\n";
            break;
        } else if (difficult == 3) {
            count = 60;
            std::cout << "Hard\n";
            break;
        } else {
            std::cout << "Input Error. Select 1, 2 or 3!\n";
        }
    }
    removeNumbers(board, count);
    int r, c, val, hint = 3;
    while (true) {
        std::cout << std::endl;
        printBoard(board);
        bool hasZero = false;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == 0) {
                    hasZero = true;
                }
            }
        }

        if (!hasZero) {
            std::cout << "CONGRATULATIONS! You solved the Sudoku!" << std::endl;
            break;
        }

        std::cout << "\nEnter row, column, and value (or -1 to exit, -2 for hint(you have " << hint << " hints)):";
        std::cin >> r;
        if (r == -1) {
            break;
        }
        if(r == -2 && hint != 0) {
            while (true) {
                std::uniform_int_distribution<int> dist(0, N * N - 1);
                int cell = dist(g);
                int r = cell / N;
                int c = cell % N;
                if (board[r][c] == 0) {
                    board[r][c] = solution[r][c];
                    hint--;
                    std::cout << "You use the hint! Now you have " << hint << " hints!\n" << "Filled cell [" << r << "][" << c << "] with " << board[r][c] << std::endl;
                    break;
                }
            }
            continue;
        } else if (r == -2 && hint == 0) {
            std::cout << "You use all your hints!\n";
            continue;
        }
        std::cin >> c >> val;
        std::cout << std::endl;
        if (r < 0 || r >= N || c < 0 || c >= N) {
            std::cout << "Error: Invalid coordinates!" << std::endl;
            continue;
        }

        if (isValid(board, r, c, val)) {
            board[r][c] = val;
            std::cout << "Move accepted!" << std::endl;
        } else {
            std::cout << "Error: This number violates Sudoku rules!" << std::endl;
        }
    }

    std::cout << "Game over. See you next time!" << std::endl;
    return 0;
}