#include "cubic.hpp"
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

std::vector<std::vector<std::vector<int>>> defaultFill(int n) {
    std::vector<std::vector<std::vector<int>>> cubik;
    cubik = std::vector<std::vector<std::vector<int>>>(6, std::vector<std::vector<int>>(n, std::vector<int>(n)));
    for (int color = 0; color < 6; color++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cubik[color][i][j] = color;
            }
        }
    }
    return cubik;
}

Cube::Cube(int n_) : n(n_),
planes({
    {0, 5, 3, 2, 1, 4},
    {1, 0, 2, 4, 5, 3},
    {2, 0, 3, 4, 1, 5},
    {3, 0, 5, 4, 2, 1},
    {4, 2, 3, 5, 1, 0},
    {5, 4, 3, 0, 1, 2}}), cube(defaultFill(n_)) {}
Cube::Cube (const Cube& cube) : n(cube.n), planes(cube.planes), cube(cube.cube), moves(cube.moves), str_moves(cube.str_moves) {}

Cube& Cube::operator= (const Cube& cubik) {
    if (&cubik == this) return *this;
    n = cubik.n;
    cube = cubik.cube;
    planes = cubik.planes;
    moves = cubik.moves;
    return *this;
}
void Cube::rotatePlane(int k, int times) {
    //clockwise
    std::vector<std::vector<std::vector<int>>> result = cube;
    for (int t = 0; t < times; ++t) {
        //rotating k-th plane
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int t = n - j - 1;
                result[k][i][t] = cube[k][j][i];
            }
        }
        //rotating surrounding planes
        switch (k) {
            case 0:
                for (int i = 0; i < n; i++) {
                    result[2][0][n - i - 1] = cube[3][0][n - 1 - i];
                    result[1][0][n - i - 1] = cube[2][0][n - i - 1];
                    result[5][n - 1][i] = cube[1][0][n - i - 1];
                    result[3][0][n - 1 - i] = cube[5][n - 1][i];
                }
                break;
            case 1:
                for (int i = 0; i < n; i++) {
                    result[2][i][0] = cube[0][i][0];
                    result[4][i][0] = cube[2][i][0];
                    result[5][i][0] = cube[4][i][0];
                    result[0][i][0] = cube[5][i][0];
                }
                break;
            case 2:
                for (int i = 0; i < n; i++) {
                    result[3][i][0] = cube[0][n - 1][i];
                    result[4][0][n - i - 1] = cube[3][i][0];
                    result[1][n - i - 1][n - 1] = cube[4][0][n - i - 1];
                    result[0][n - 1][i] = cube[1][n - i - 1][n - 1];
                }
                break;
            case 3:
                for (int i = 0; i < n; i++) {
                    result[2][n - 1 - i][n - 1] = cube[4][n - 1 - i][n - 1];
                    result[0][n - i - 1][n - 1] = cube[2][n - i - 1][n - 1];
                    result[5][n - i - 1][n - 1] = cube[0][n - i - 1][n - 1];
                    result[4][n - i - 1][n - 1] = cube[5][n - i - 1][n - 1];
                }
                break;
            case 4:
                for (int i = 0; i < n; i++) {
                    result[3][n - 1][i] = cube[2][n - 1][i];
                    result[5][0][n - i - 1] = cube[3][n - 1][i];
                    result[1][n - 1][i] = cube[5][0][n - i - 1];
                    result[2][n - 1][i] = cube[1][n - 1][i];
                }
                break;
            case 5:
                for (int i = 0; i < n; i++) {
                    result[3][n - i - 1][n - 1] = cube[4][n - 1][i];
                    result[0][0][n - i - 1] = cube[3][n - i - 1][n - 1];
                    result[1][i][0] = cube[0][0][n - i - 1];
                    result[4][n - 1][i] = cube[1][i][0];
                }
                break;
        }
        cube = result;
    }
}
void Cube::randomGenerate() {
    
    //random transformation
    srand(static_cast<unsigned int>(time(nullptr)));
    int number = rand() % 30 + 1;
    moves = std::vector<std::pair<int, int>>(number);
    for (int i = 0; i < number; i++) {
        moves[i].first = rand() % 6;
        moves[i].second = rand() % 3 + 1;
        rotatePlane(moves[i].first, moves[i].second);
    }
    str_moves = movesToString(moves);
    
}
bool Cube::check() const {
    int m = n / 2;
    for (int i = 0; i < 6; i++) {
        if (cube[i][m][m] != i) {
            return false;
        }
    }
    std::vector<int> colors(6, 0);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                colors[cube[i][j][k]]++;
                if (colors[cube[i][j][k]] > n * n) return false;
            }
        }
    }
    
    for (int j = 0; j < 6; j++) {
        switch (j) {
            case 0:
                for (int i = 0; i < n; i++) {
                    if (cube[2][0][i] == planes[cube[0][n - 1][i]].back) return false;
                    if (cube[1][0][i] == planes[cube[0][i][0]].back) return false;
                    if (cube[5][n - 1][i] == planes[cube[0][0][i]].back) return false;
                    if (cube[3][0][n - 1 - i] == planes[cube[0][i][n - 1]].back) return false;
                }
                break;
            case 1:
                for (int i = 0; i < n; i++) {
                    if (cube[0][i][0] == planes[cube[1][0][i]].back) return false;
                    if (cube[2][i][0] == planes[cube[1][i][n - 1]].back) return false;
                    if (cube[4][i][0] == planes[cube[1][n - 1][n - 1- i]].back) return false;
                    if (cube[5][n - i - 1][0] == planes[cube[1][i][0]].back) return false;
                }
                break;
            case 2:
                for (int i = 0; i < n; i++) {
                    if (cube[0][n - 1][i] == planes[cube[2][0][i]].back) return false;
                    if (cube[3][i][0] == planes[cube[2][i][n - 1]].back) return false;
                    if (cube[4][0][i] == planes[cube[2][n - 1][i]].back) return false;
                    if (cube[1][i][n - 1] == planes[cube[2][i][0]].back) return false;
                }
                break;
            case 3:
                for (int i = 0; i < n; i++) {
                    if (cube[0][n - 1 - i][n - 1] == planes[cube[3][0][i]].back) return false;
                    if (cube[5][n - i - 1][n - 1] == planes[cube[3][i][n - 1]].back) return false;
                    if (cube[4][i][n - 1] == planes[cube[3][n - 1][i]].back) return false;
                    if (cube[2][i][n - 1] == planes[cube[3][i][0]].back) return false;
                }
                break;
            case 4:
                for (int i = 0; i < n; i++) {
                    if (cube[2][n - 1][i] == planes[cube[4][0][i]].back) return false;
                    if (cube[3][n - 1][i] == planes[cube[4][i][n - 1]].back) return false;
                    if (cube[5][0][i] == planes[cube[4][n - 1][i]].back) return false;
                    if (cube[1][n - 1][i] == planes[cube[4][n - i - 1][0]].back) return false;
                }
                break;
            case 5:
                for (int i = 0; i < n; i++) {
                    if (cube[4][n - 1][i] == planes[cube[5][0][i]].back) return false;
                    if (cube[3][n - i - 1][n - 1] == planes[cube[5][i][n - 1]].back) return false;
                    if (cube[0][0][i] == planes[cube[5][n - 1][i]].back) return false;
                    if (cube[1][n - 1 - i][0] == planes[cube[5][i][0]].back) return false;
                }
                break;
        }
    }
    
    return true;
}
void Cube::implement() {
    for (auto move : moves) {
        rotatePlane(move.first, move.second);
    }
}
void Cube::addMove(std::pair<int, int> move) {
    if (moves.size() != 0 && moves.back().first == move.first) {
        moves.back().second += move.second;
        moves.back().second %= 4;
    } else {
        moves.push_back(move);
    }
    if (moves.back().second == 0) {
        moves.erase(moves.end() - 1);
    }
    rotatePlane(move.first, move.second);
    str_moves = movesToString(moves);
}
int Cube::number() const {
    return n;
}
const std::vector<std::vector<std::vector<int>>>& Cube::getCube() const {
    return cube;
}
std::ostream& operator<< (std::ostream& out, const Cube& cube) {
    out << cube.n << std::endl;
    std::string moves = movesToString(cube.moves);
    out << moves;
    return out;
}
std::istream& operator>> (std::istream& in, Cube& cube) {
    in >> cube.n;
    in >> cube.str_moves;
    cube.moves = moves(cube.str_moves);
    cube.implement();
    return in;
}

int Cube::countWrongEdges() {
    int res = 0;
    for (int j = 0; j < 6; j++) {
        for (int i = 1; i < n - 1; i++) {
            if (cube[j][0][i] != j) {
                res += 2;
            }
            if (cube[j][i][0] != j) {
                res += 2;
            }
            if (cube[j][i][n - 1] != j) {
                res += 2;
            }
            if (cube[j][n - 1][i] != j) {
                res += 2;
            }
        }
    }
    return res;
}
int Cube::countWrongFaceLets() {
    int res = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (cube[i][j][k] != i) {
                    res++;
                }
            }
        }
    }
    return res;
}
std::string Cube::getMoves() {
    return str_moves;
}
std::string movesToString(const std::vector<std::pair<int, int>>& moves) {
    std::string result = "";
    for (int i = 0; i < moves.size(); i++) {
        std::pair<int, int> move = moves[i];
        while (i < moves.size() - 1 && moves[i + 1].first == move.first) {
            move.second += moves[i + 1].second;
            i++;
        }
        if (move.second % 4 != 0) {
            result += planeToChar(move.first);
            if (move.second % 4 != 1) {
                result += timesToChar(move.second % 4);
            }
        }
    }
    return result;
}
std::vector<std::pair<int, int>> moves(std::string str_moves) {
    std::vector<std::pair<int, int>> result;
    std::pair<int, int> move;
    for (int i = 0; i < str_moves.length(); i++) {
        move.first = charToPlane(str_moves[i]);
        move.second = 1;
        if (i + 1 < str_moves.length()) {
            move.second = charToTimes(str_moves[i + 1]);
            if (move.second != 1) i++;
        }
        result.push_back(move);
    }
    return result;
}


char planeToChar(int i) {
    if (i == 0) return 'U';
    if (i == 1) return 'L';
    if (i == 2) return 'F';
    if (i == 3) return 'R';
    if (i == 4) return 'D';
    if (i == 5) return 'B';
    return 0;
}

char timesToChar(int i) {
    if (i == 2) return '2';
    if (i == 3) return '\'';
    return 0;
}
int charToPlane(char i) {
    if (i == 'U') return 0;
    if (i == 'L') return 1;
    if (i == 'F') return 2;
    if (i == 'R') return 3;
    if (i == 'D') return 4;
    if (i == 'B') return 5;
    return 0;
}
int charToTimes(char i) {
    if (i == '2') return 2;
    if (i == '\'' || i == 'i') return 3;
    return 1;
}
