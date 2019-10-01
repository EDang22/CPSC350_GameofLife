/*
* Ethan Dang
* 2326380
* edang@chapman.edu
* CPSC 350-02
* Assignment 2
* GameofLife class: Runs simulation, generates future generation
* and compares, then switches generations. Ends game if grid is stable or dead
*/
#include <iostream>
#include <fstream>
#include <limits>
#include "GameofLife.h"

using namespace std;

// default constructor
GameofLife::GameofLife()
{
  curGrid = new char*[10];
  nextGrid = new char*[10];
  isDead = true;
  isStable = false;
  outputFile = false;
  numRow = 10;
  numCol = 10;
  numGens = 0;
  numSame = 0;
  m = 'c';
  o = 'p';
  for (int i = 0; i < numRow; ++i)
  {
    curGrid[i] = new char[numCol];
    nextGrid[i] = new char[numCol];
    for (int j = 0; j < numCol; ++j)
    {
      curGrid[i][j] = '-';
      nextGrid[i][j] = '-';
    }
  }
}

// overloaded constructor
GameofLife::GameofLife(int row, int col, string grid, char mode, string output, bool oF)
{
  curGrid = new char*[row];
  nextGrid = new char*[row];
  numRow = row;
  numCol = col;
  numGens = 0;
  isDead = false;
  isStable = false;
  m = tolower(mode);
  o = output;
  outputFile = oF;
  for (int i = 0; i < numRow; ++i) // initializes grid
  {
    curGrid[i] = new char[numCol];
    nextGrid[i] = new char[numCol];
    for (int j = 0; j < numCol; ++j)
    {
      curGrid[i][j] = '-';
      nextGrid[i][j] = '-';
    }
  }
  if(addGrid(grid)) // adds string grid to 2D array
  {
    runSim(); // runs simulation
  }
}

// Takes given string grid and adds to 2D array
bool GameofLife::addGrid(string grid)
{
  if (grid.length() == (numRow * numCol)) // ensures given grid is proper format
  {
    int gridI = 0;
    for (int i = 0; i < numRow; ++i)
    {
      for (int j = 0; j < numCol; ++j)
      {
        if (tolower(grid[gridI]) == 'x' || tolower(grid[gridI]) == '-') // ensures given grid is proper format
        {
          curGrid[i][j] = grid[gridI];
          ++gridI;
        }
        else
        {
          cerr << "Error: given grid is invalid" << endl;
          return false;
        }
      }
    }
  }
  else
  {
    cerr << "Error: given grid does not match desired grid" << endl;
    return false;
  }
  return true;
}

// Runs simulation, check method of output and prints to console or file
void GameofLife::runSim()
{
  if (!outputFile) // method of output is to console
  {
    while (!isDead && !isStable) // while grid is not dead or stable
    {
      cout << numGens << endl;
      for(int i = 0; i < numRow; ++i)
      {
        for (int j = 0; j < numCol; ++j)
        {
          cout << curGrid[i][j];
        }
        cout << endl;
      }
      if (tolower(o[0]) == 'e') // must press enter before next generation
      {
        cout << endl << "Press enter to print next generation" << endl;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
      }
      else // pause between generations
      {
        for (int i = 0; i < 10000000; ++i)
        {
        }
      }
      nextGen();
      checkGen();
      switchGen();
      checkDead();
      ++numGens;
    }
  }
  else
  {
    while (!isDead && !isStable)
    {
      printFile(); // prints to file
      nextGen();
      checkGen();
      switchGen();
      checkDead();
      ++numGens;
    }
  }
  endGame(); // ends simulation
  return;
}

// Generates grid for next generation, check if cell is in corner, border or middle
void GameofLife::nextGen()
{
  for (int i = 0; i < numRow; ++i)
  {
    for (int j = 0; j < numCol; ++j)
    {
      if (i == 0 || i == (numRow - 1) || j == 0 || j == (numCol - 1))
      {
        if  (i == 0 || i == numRow - 1)
        {
          if (j == 0 || j == numCol - 1)
          {
            checkNC(i, j); // corner cell
          }
          else
          {
            checkNB(i, j); // border cell
          }
        }
        else
        {
          checkNB(i, j); // border cell
        }
      }
      else
      {
        checkNM(i, j); // middle cell
      }
    }
  }
}

// Checks if next generation is identical to previous, if stable for 5 generations, game ends
void GameofLife::checkGen()
{
  for (int i = 0; i < numRow; ++i)
  {
    for (int j = 0; j < numCol; ++j)
    {
      if (curGrid[i][j] != nextGrid[i][j])
      {
        numSame = 0;
        return;
      }
    }
  }
  ++numSame;
  if (numSame == 5 || numGens == 5000) // Generation has stabilized or is in an infinite loop
  {
    isStable = true;
  }
}

// Switches next generation to current generation
void GameofLife::switchGen()
{
  curGrid = nextGrid;
  nextGrid = new char*[numRow]; // sets next generation to new array
  for (int i = 0; i < numRow; ++i)
  {
    nextGrid[i] = new char[numCol];
    for (int j = 0; j < numCol; ++j)
    {
      nextGrid[i][j] = '-';
    }
  }
}

// Checks if grid is dead
void GameofLife::checkDead()
{
  for (int i = 0; i < numRow; ++i)
  {
    for (int j = 0; j < numCol; ++j)
    {
      if (curGrid[i][j] != '-')
      {
        return;
      }
    }
  }
  isDead = true;
  return;
}

// Ends game, prints final generation
void GameofLife::endGame()
{
  if (!outputFile) //prints last generation, where it is stable or dead
  {
    cout << numGens << endl;
    for(int i = 0; i < numRow; ++i)
    {
      for (int j = 0; j < numCol; ++j)
      {
        cout << curGrid[i][j];
      }
      cout << endl;
    }
    cout << endl;
  }
  else // if output is to file
  {
    printFile();
  }
  cout << "Simulation Successful" << endl;
  return;
}

// Prints to file (path is assigned to string o)
void GameofLife::printFile()
{
  ofstream file;
  file.open(o, ofstream::out | ofstream::app);
  if (file.is_open())
  {
    file << numGens << endl;
    for(int i = 0; i < numRow; ++i)
    {
      for (int j = 0; j < numCol; ++j)
      {
        file << curGrid[i][j];
      }
      file << endl;
    }
    file.close();
  }
  else
  {
    cerr << "Error: unable to open file" << endl;
  }
}

// Checks neighbors of middle cells
// Neighbors of middle cells are constant through any game mode
void GameofLife::checkNM(int row, int col)
{
  // number of neighbors that are occupied
  int numNeigh = 0;
  if (curGrid[row - 1][col - 1] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row - 1][col] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row - 1][col + 1] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row][col - 1] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row][col + 1] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row + 1][col - 1] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row + 1][col] == 'X')
  {
    ++numNeigh;
  }
  if (curGrid[row + 1][col + 1] == 'X')
  {
    ++numNeigh;
  }
  // checks numNeigh and changes location cell based on numNeigh
  if (numNeigh == 2)
  {
    nextGrid[row][col] = curGrid[row][col];
  }
  else if (numNeigh <= 1 || numNeigh >= 4)
  {
    nextGrid[row][col] = '-';
  }
  else
  {
    nextGrid[row][col] = 'X';
  }
}

// Checks neighbors of corner cells based on game mode
// Corner cell has 3 constant neighbors through all game modes
void GameofLife::checkNC(int row, int col)
{
  // number of neighbors that are occupied
  int numNeigh = 0;

  // checks own corner in case of mirror mode as corner acts as 3 neighbors
  if (m == 'm' && (curGrid[row][col] == 'X'))
  {
    numNeigh += 3;
  }





  if (row == 0)
  {
    if (col == 0) // top left corner
    {
      if (curGrid[row][col + 1] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row + 1][col] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row + 1][col + 1] == 'X')
      {
        ++numNeigh;
      }
      if (m == 'd') // neighbors to check if in donut mode
      {
        if (curGrid[row][numCol - 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[row + 1][numCol - 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[numRow - 1][col] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[numRow - 1][col + 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[numRow - 1][numCol - 1] == 'X')
        {
          ++numNeigh;
        }
      }
    }





    else // top right corner
    {
      if (curGrid[row][col - 1] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row + 1][col] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row + 1][col - 1] == 'X')
      {
        ++numNeigh;
      }
      if (m == 'd') // neighbors to check if in donut mode
      {
        if (curGrid[row][0] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[row + 1][0] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[numRow - 1][col] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[numRow - 1][col - 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[numRow - 1][0] == 'X')
        {
          ++numNeigh;
        }
      }
    }
  }





  else
  {
    if (col == 0) // bottom left corner
    {
      if (curGrid[row][col + 1] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row - 1][col] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row - 1][col + 1] == 'X')
      {
        ++numNeigh;
      }
      if (m == 'd') // neighbors to check if in donut mode
      {
        if (curGrid[row][numCol - 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[row - 1][numCol - 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[0][col] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[0][col + 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[0][numCol - 1] == 'X')
        {
          ++numNeigh;
        }
      }
    }





    else // bottom right corner
    {
      if (curGrid[row][col - 1] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row - 1][col] == 'X')
      {
        if (m == 'm') // if mirror mode, this neighbor counts as 2
        {
          numNeigh += 2;
        }
        else
        {
          ++numNeigh;
        }
      }
      if (curGrid[row - 1][col - 1] == 'X')
      {
        ++numNeigh;
      }
      if (m == 'd') // neighbors to check if in donut mode
      {
        if (curGrid[row][0] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[row - 1][0] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[0][col] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[0][col - 1] == 'X')
        {
          ++numNeigh;
        }
        if (curGrid[0][0] == 'X')
        {
          ++numNeigh;
        }
      }
    }
  }




  // determines state of cell in next generation
  if (numNeigh == 2)
  {
    nextGrid[row][col] = curGrid[row][col];
  }
  else if (numNeigh <= 1 || numNeigh >= 4)
  {
    nextGrid[row][col] = '-';
  }
  else
  {
    nextGrid[row][col] = 'X';
  }
}

// Checks neighbors of border cells based on game mode
// Border cell has 5 constant neighbors through all game modes
void GameofLife::checkNB(int row, int col)
{
  int numNeigh = 0;

  // Checks if mirror mode as own border cell counts as its neighbor
  if (m == 'm' && (curGrid[row][col] == 'X'))
  {
    ++numNeigh;
  }





  if (row == 0) // Top Row
  {
    if (curGrid[row][col - 1] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row][col + 1] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row + 1][col - 1] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row + 1][col] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row + 1][col + 1] == 'X')
    {
      ++numNeigh;
    }
    if (m == 'd')
    {
      if (curGrid[numRow - 1][col] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[numRow - 1][col - 1] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[numRow - 1][col + 1] == 'X')
      {
        ++numNeigh;
      }
    }
  }





  else if (row == numRow - 1) // Bottom Row
  {
    if (curGrid[row][col - 1] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row][col + 1] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row - 1][col - 1] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row - 1][col] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row - 1][col + 1] == 'X')
    {
      ++numNeigh;
    }
    if (m == 'd')
    {
      if (curGrid[0][col] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[0][col - 1] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[0][col + 1] == 'X')
      {
        ++numNeigh;
      }
    }
  }





  else if (col == 0) // Left side
  {
    if (curGrid[row + 1][col] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row - 1][col] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row - 1][col + 1] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row][col + 1] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row + 1][col + 1] == 'X')
    {
      ++numNeigh;
    }
    if (m == 'd')
    {
      if (curGrid[row][numCol - 1] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[row - 1][numCol - 1] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[row + 1][numCol - 1] == 'X')
      {
        ++numNeigh;
      }
    }
  }





  else // Right side
  {
    if (curGrid[row + 1][col] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row - 1][col] == 'X')
    {
      if (m == 'm') // if mirror mode, this counts as two neighbors
      {
        numNeigh += 2;
      }
      else
      {
        ++numNeigh;
      }
    }
    if (curGrid[row - 1][col - 1] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row][col - 1] == 'X')
    {
      ++numNeigh;
    }
    if (curGrid[row + 1][col - 1] == 'X')
    {
      ++numNeigh;
    }
    if (m == 'd')
    {
      if (curGrid[row][0] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[row - 1][0] == 'X')
      {
        ++numNeigh;
      }
      if (curGrid[row + 1][0] == 'X')
      {
        ++numNeigh;
      }
    }
  }




  // Determines state of cell for next generation
  if (numNeigh == 2)
  {
    nextGrid[row][col] = curGrid[row][col];
  }
  else if (numNeigh <= 1 || numNeigh >= 4)
  {
    nextGrid[row][col] = '-';
  }
  else
  {
    nextGrid[row][col] = 'X';
  }
}
