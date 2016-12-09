// homework_plan.cpp
// Ryan Stonebraker
// 11/30/2016
// Homework organization

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
#include <string>
using std::string;
using std::getline;
#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
#include <algorithm>
using std::binary_search;
using std::sort;
#include "homework.h"

void loadHw (vector <Homework> & asnmt, string fName)
{
  ifstream reader (fName);
  if (!reader)
  {
    cout << "Empty File. Creating New..." << endl;
    return;
  }

  while (!reader.eof())
  {
    Homework entry;
    string buffer;
    string temp;
    string tmpStat;
    getline (reader, buffer);
    istringstream iss (buffer);
    for (unsigned i = 0; iss >> temp; i++)
    {
      istringstream strNum (temp);
      istringstream valRd (tmpStat);
      int tmpNum;
      bool pStat = false;
      switch (i)
      {
        // TODO add Due Date
        case 0:
          entry.setName(temp);
          break;
        case 1:
          entry.setClass(temp);
          break;
        case 2:
          strNum >> tmpNum;
          if (!strNum || tmpNum < 0)
          {
            cerr << "ERROR READING " << entry.aName() << endl;
            return;
          }
          entry.setProbs(tmpNum);
          break;
        default:
          if (temp != "1" && temp != "0")
            tmpStat = temp;
          else
          {
            pStat = (temp == "1") ? true : false;
            valRd >> tmpNum;
            if (!valRd || tmpNum < 0)
            {
              cerr << "ERROR READING " << entry.aName() << " PROBLEMS." << endl;
            }
            entry.addProb(tmpNum, pStat);
          }
          break;
      }
    }
    if (entry.aName() != "" && entry.aClass() != "" && entry.aProbs())
      asnmt.push_back (entry);
  }

}

void addA (vector<Homework> & asnmt, string assignment)
{
  cout << "Enter class and number of problems: ";
  string entered, classEnt, ofl;
  int prob;
  getline (cin, entered);
  istringstream iss(entered);
  iss >> classEnt;
  iss >> prob;
  if (!iss || iss >> ofl || prob < 1)
  {
    cerr << "ERROR parsing entry!" << endl;
    return;
  }

  vector <int> actProbs;
  for (unsigned i = 0; i < prob; i++)
  {
    cout << "Problem #: ";
    int tmpPrb;
    string tmp, ofl;
    getline (cin, tmp);
    istringstream iss2(tmp);
    iss2 >> tmpPrb;
    if (!iss2 || iss2 >> ofl || tmpPrb < 0)
    {
      cerr << "ERORR READING PROBLEM NUMBER. Retry." << endl;
      --i;
      continue;
    }
    actProbs.push_back(tmpPrb);
  }

  Homework asmt (assignment, classEnt, actProbs);
  asmt.setProbs(prob);

  asnmt.push_back(asmt);
}

void removeA (vector<Homework> & asnmt, string assignment)
{
  int loc;
  bool found = false;
  for (unsigned i = 0; i < asnmt.size(); i++)
  {
    if (asnmt[i].aName() == assignment)
    {
      loc = i;
      found = true;
    }
  }
  if (!found)
  {
    cout << "Could not find assignment!" << endl;
    return;
  }
  else
  {
    Homework temp = asnmt[asnmt.size()-1];
    asnmt[asnmt.size()-1] = asnmt[loc];
    asnmt[loc] = temp;
    asnmt.pop_back();
  }
}

void viewA (vector<Homework> & asnmt, string assignment)
{
  cout << "|Assignment:" << "|   |" << "Class:" << "|   |" << "Problems:|" << endl;
  for (unsigned i = 0; i < asnmt.size(); i++)
  {
    if  (assignment == "all")
    {
      cout << "|" << asnmt[i].aName() << "|  |" << asnmt[i].aClass() << "|  |";
      for (unsigned k = 0; k < asnmt[i].aProbs(); k++)
      {
        tuple <int, bool> tmpP = asnmt[i].viewProb(k);
        cout << get<0>(tmpP) << "-" << ((!get<1>(tmpP)) ? "Finished" : "Unfinished");
        if (k != asnmt[i].aProbs()-1)
          cout << ", ";
      }
      cout << "|"<< endl;
    }
    else
    {
      if (asnmt[i].aName() == assignment)
      {
        cout << asnmt[i].aName() << "   " << asnmt[i].aClass() << "   ";
        for (unsigned k = 0; k < asnmt[i].aProbs(); k++)
        {
          tuple <int, bool> tmpP = asnmt[i].viewProb(k);
          cout << get<0>(tmpP) << ", " << ((!get<1>(tmpP)) ? "Finished" : "Unfinished") << ", ";
        }
        cout << endl;
      }
    }
  }
}

void writeHw (vector <Homework> & asnmt, string fName)
{
  ofstream writer (fName);
  for (unsigned i = 0; i < asnmt.size(); i++)
  {
    Homework hw = asnmt[i];
    writer << hw.aName() << " " << hw.aClass() << " " << hw.aProbs() << " ";
    for (unsigned i = 0; i < hw.aProbs(); i++)
    {
      writer << get<0>(hw.viewProb(i)) << " " << get<1>(hw.viewProb(i)) << " ";
    }
    writer << endl;
  }
}


int main (int argc, char* argv[])
{
  if (argc != 3)
  {
    cout << "USAGE: ./homework_plan task(add/rm/view) ASSIGNMENT/all" << endl;
    return 1;
  }

  string task = argv[1];
  string assignment = argv[2];

  if (task != "add" && task != "rm" && task != "view")
  {
    cout << "USAGE: ./homework_plan task(add/rm/view) ASSIGNMENT/all" << endl;
    return 2;
  }

  string fName = ".homework.txt";

  vector <Homework> asnmt;
  loadHw (asnmt, fName);

  if (task == "add")
    addA (asnmt, assignment);
  else if (task == "rm")
    removeA (asnmt, assignment);
  else if (task == "view")
    viewA (asnmt, assignment);

  if (asnmt.size() > 0)
    writeHw (asnmt, fName);
}
