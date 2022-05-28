
#ifndef cubic_hpp
#define cubic_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
struct plane {
    int number;
    int up;
    int right;
    int down;
    int left;
    int back;
};
class Cube {
private:
    int n;
    std::vector<std::vector<std::vector<int>>> cube;
    std::vector<plane> planes;
    std::vector<std::pair<int, int>> moves;
    std::string str_moves;
public:
    Cube(int n = 3);
    Cube (const Cube&);
    Cube& operator= (const Cube& cubik);
    
    void rotatePlane(int i, int times);
    void randomGenerate();
    bool check() const;
   
    void implement();
    void improve_moves();
    void addMove(std::pair<int, int> move);
    
    int number() const;
    std::string getMoves();
    const std::vector<std::vector<std::vector<int>>>& getCube() const;
    
    int countWrongEdges();
    int countWrongFaceLets();
    friend std::string movesToString(std::vector<std::pair<int, int>>&);
    friend std::ostream& operator<< (std::ostream& out, const Cube& cube);
    friend std::istream& operator>> (std::istream& in, Cube& cube);
};
std::ostream& operator<< (std::ostream& out, const Cube& cube);
std::istream& operator>> (std::istream& in, Cube& cube);
std::string movesToString(const std::vector<std::pair<int, int>>&);
std::vector<std::pair<int, int>> moves(std::string str_moves);
char planeToChar(int i);
char timesToChar(int i);
int charToPlane(char i);
int charToTimes(char i);

#endif
