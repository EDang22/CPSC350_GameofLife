#include <iostream>
#include <string>
#include "GameofLife.h"

using namespace std;

class GameDriver
{
  private:
    int row;
    int col;
    double dens;
    string grid;
    char mode;
    string output;
    bool oF;
    GameofLife game;

  public:

    GameDriver();
    void startGame();
    bool getGrid();
    void genGrid();
    void createGrid();
    void getMode();
};
