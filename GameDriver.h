/*
* Ethan Dang
* 2326380
* edang@chapman.edu
* CPSC 350-02
* Assignment 2
* GameDriver header class
*/
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
    bool oF; // true if user wises to output to file
    GameofLife game; // runs simulation

  public:

    // constructor
    GameDriver();

    // methods
    void startGame();
    bool getGrid();
    void genGrid();
    void createGrid();
    void getMode();
};
