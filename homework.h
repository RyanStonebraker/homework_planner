// homework.h
// Ryan Stonebraker
// 12/17/2016
// header file containing homework class

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <tuple>

enum {MONTH, DAY, YEAR};

class Homework {
public:
  Homework (std::string name, std::string clss, std::map<std::string, bool> problems, std::tuple <int, int, int> dueDt)
  {
    _nm = name;
    _clss = clss;
    _probs = problems;
    _dueDt = dueDt;
    ++nmHW;
  }
  Homework ()
  {
    ++nmHW;
  }
private:
  std::string _nm;
  std::string _clss;
  std::map<std::string, bool> _probs;
  std::tuple<int, int, int> _dueDt;
  static int nmHW;
public:
  const std::string name ()
  {
    return _nm;
  }
  const std::string course ()
  {
    return _clss;
  }
  static int hwAmt ()
  {
    return nmHW;
  }
  const std::map<std::string, bool> allProbs ()
  {
    return _probs;
  }
  void setProbs (const std::map <std::string, bool> & prbs)
  {
    _probs = prbs;
  }
  const bool probStat (std::string find)
  {
    return _probs[find];
  }
  void finProb (const std::string & prb)
  {
    _probs[prb] = true;
  }
  const std::string dueString ()
  {
    return std::to_string(std::get<MONTH>(_dueDt)) + "/" +
    std::to_string(std::get<DAY>(_dueDt)) + "/" + std::to_string(std::get<YEAR>(_dueDt));
  }
  const int dMonth ()
  {
    return std::get<MONTH>(_dueDt);
  }
  const int dDay ()
  {
    return std::get<DAY>(_dueDt);
  }
  const int dYear ()
  {
    return std::get<YEAR>(_dueDt);
  }
};
