#include <iostream>
#include <string>

using namespace std;

class GameofLife
{
  private:
    char **curGrid;
    char **nextGrid;
    bool isDead;
    bool isStable;
    bool outputFile;
    int numRow;
    int numCol;
    int numGens;
    int numSame;
    char m;
    string o;

  public:

    GameofLife();
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
