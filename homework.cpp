// homework.cpp
// Ryan Stonebraker
// 11/30/2016
// source for homework.h, functions for Homework class

#include "homework.h"

#include <iostream>
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <tuple>
using std::get;
using std::tuple;
using std::make_tuple;

Homework::Homework (string name, string clsNm, vector <int> probs)
{
  _name = name;
  _class = clsNm;
  _probs = probs.size();
  for (unsigned i = 0; i < _probs; i++)
  {
    addProb(probs[i], true);
  }
}

Homework::Homework (string name, vector <int> probs)
{
  _name = name;
  _probs = probs.size();
  for (unsigned i = 0; i < _probs; i++)
  {
    addProb(probs[i], true);
  }

}

Homework::Homework (string name, string clsNm)
{
  _name = name;
  _class = clsNm;
}

bool Homework::find (int probNum)
{
  return binary_search(_pr.begin(), _pr.end(), probNum,
  [](tuple<int, bool> const &t1, tuple <int, bool> const &t2)
  {
    return get<0>(t1) < get<0>(t2);
  });
}

void Homework::addProb (int num, bool status)
{
  _pr.push_back (make_tuple(num, status));
  sortProbs();
}

void Homework::finProb (int probNum, int place)
{
  if (!find(probNum))
  {
    cerr << "Problem doesn't exist in homework!" << endl;
  }
  else
  {
    get<1>(_pr[place]) = false;
  }
}

void Homework::sortProbs ()
{
  sort(_pr.begin(), _pr.end(),
  [](tuple<int, bool> const &t1, tuple <int, bool> const &t2)
  {
    return get<0>(t1) < get<0>(t2);
  });
}
