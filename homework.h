// homework.h
// Ryan Stonebraker
// 11/30/2016
// header for homework.cpp, includes Homework class

#ifndef FILE_HOMEWORK_H_INCLUDED
#define FILE_HOMEWORK_H_INCLUDED

#include <iostream>
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
using std::get;
using std::make_tuple;

class Homework {
public:
  Homework (string name, string clsNm, vector <int> probs);
  Homework (string name, vector <int> probs);
  Homework (string name, string clsNm);
  Homework (string name)
  {
    _name = name;
  }
  Homework ()
  {
  }
private:
  string _name = "";
  string _class = "";
  int _probs = 0;
  vector <tuple<int, bool> > _pr;
  bool _keep = true;
  bool find (int probNum);
public:
  string aName ()
  {
    return _name;
  }
  string aClass ()
  {
    return _class;
  }
  int aProbs ()
  {
    return _probs;
  }
  void setName (string name)
  {
    _name = name;
  }
  void setClass (string cls)
  {
    _class = cls;
  }
  void setProbs (int probsNum)
  {
    _probs = probsNum;
  }
  bool status ()
  {
    return _keep;
  }
  void remove ()
  {
    _keep = false;
  }
  void addProb (int num, bool status);
  void finProb (int probNum, int place);
  void sortProbs ();
  tuple<int, bool> viewProb (int i)
  {
    return _pr[i];
  }
};

#endif
