#include <iostream>
#include <fstream>
#include <limits>
#include "GameofLife.h"

using namespace std;

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

GameofLife::GameofLife(int row, int col, string grid, char mode, string output, bool oF)
{
  curGrid = new char*[row];
  nextGrid = new char*[row];
  numRow = row;
  numCol = col;
  numGens = 0;
  isDead = false;
  isStable = false;
  m = tolower(mode, locale());
  o = output;
  outputFile = oF;
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
  if(addGrid(grid))
  {
    runSim();
  }
}

bool GameofLife::addGrid(string grid)
{
  if (grid.length() == (numRow * numCol))
  {
    int gridI = 0;
    for (int i = 0; i < numRow; ++i)
    {
      for (int j = 0; j < numCol; ++j)
      {
        if (tolower(grid[gridI]) == 'x' || tolower(grid[gridI]) == '-')
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

void GameofLife::runSim()
{
  if (!outputFile)
  {
    while (!isDead && !isStable)
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
      if (tolower(o[0]) == 'e')
      {
        cout << endl << "Press enter to print next generation" << endl;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
      }
      else
      {
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
      printFile();
      nextGen();
      checkGen();
      switchGen();
      checkDead();
      ++numGens;
    }
  }
  endGame();
  return;
}

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
            checkNC(i, j);
          }
          else
          {
            checkNB(i, j);
          }
        }
        else
        {
          checkNB(i, j);
        }
      }
      else
      {
        checkNM(i, j);
      }
    }
  }
}

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
  if (numSame == 5)
  {
    isStable = true;
  }
}

void GameofLife::switchGen()
{
  curGrid = nextGrid;
  nextGrid = new char*[numRow];
  for (int i = 0; i < numRow; ++i)
  {
    nextGrid[i] = new char[numCol];
    for (int j = 0; j < numCol; ++j)
    {
      nextGrid[i][j] = '-';
    }
  }
}

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
  else
  {
    printFile();
  }
  cout << "Simulation Successful" << endl;
  return;
}

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

void GameofLife::checkNM(int row, int col)
{
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

void GameofLife::checkNC(int row, int col)
{
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
        if (m == 'm')
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
        if (m == 'm')
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
        if (m == 'm')
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
        if (m == 'm')
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
        if (m == 'm')
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
        if (m == 'm')
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
        if (m == 'm')
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
        if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
      if (m == 'm')
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
