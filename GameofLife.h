/*
* Ethan Dang
* 2326380
* edang@chapman.edu
* CPSC 350-02
* Assignment 2
* GameofLife header class
*/
#include <iostream>
#include <string>

using namespace std;

class GameofLife
{
  private:
    char **curGrid; // current generation
    char **nextGrid; // next generation
    bool isDead; // true if grid is dead
    bool isStable; // true if grid is stable for 5 generations
    bool outputFile; // true if user wishes to output to file
    int numRow;
    int numCol;
    int numGens; // generation number
    int numSame; // number of generations the grid has been stable
    char m; // mode
    string o; // output

  public:

    // constructors
    GameofLife(); // default
    GameofLife(int row, int col, string grid, char mode, string output, bool oF);

    void runSim();
    bool addGrid(string grid);
    void nextGen();
    void checkGen();
    void switchGen();
    void checkDead();
    void endGame();

    void printFile();

    void checkNC(int row, int col);
    void checkNB(int row, int col);
    void checkNM(int row, int col);
  };
