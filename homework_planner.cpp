// homework_planner.cpp
// Ryan Stonebraker
// 12/17/2016
// Keeps track of homework

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <string>
using std::string;
using std::getline;
#include <map>
using std::map;
#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
#include <sstream>
using std::istringstream;
#include <vector>
using std::vector;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include "homework.h"

void plannerMode (bool & changed)
{

}

void addAssnmt (string nm, vector <Homework> & stored, bool & changed)
{
  string tmp, course, ofl;
  cout << "Class: ";
  getline (cin, tmp);
  istringstream iss (tmp);
  iss >> course;
  if (!iss || iss >> ofl)
    return;

  string dueDate;
  int month, day, year;
  cout << "Due Date (MM/DD/YYYY): ";
  getline (cin, dueDate);
  istringstream issD (dueDate);
  for(unsigned i = 0; i < 3; i++)
  {
    getline (issD, tmp, '/');
    istringstream toInt(tmp);
    switch (i)
    {
      case 0:
        toInt >> month;
        break;
      case 1:
        toInt >> day;
        break;
      case 2:
        toInt >> year;
        break;
      default: // if more than 2, then date improperly entered, return
        cout << "Wrong Format." << endl;
        return;
    }
    if (!toInt)
    {
      cout << "Error processing date!" << endl;
      return;
    }
  }
  tuple <int, int, int> date = make_tuple (month, day, year);

  string prbTmp, prb, overflow;
  map <string, bool> prbs;
  cout << "Enter Problem: ";
  while (true)
  {
    getline(cin, prbTmp);
    istringstream issP (prbTmp);
    if (!issP || prbTmp == "")
      break;
    issP >> prb;
    if (issP >> overflow)
    {
      cout << "Bad Problem Entry! Retry: ";
      continue;
    }
    prbs[prb] = true;
    cout << "Enter Problem: ";
  }

  Homework entry (nm, course, prbs, date);
  changed = true;
  stored.push_back (entry);
}

void rmAssnmt (string nm)
{

}

void viewAssnmt (string nm, vector <Homework> & stored)
{
  string NameWrite = "Name:            ";
  string CourseWrite = "Course:            ";
  string DueWrite = "Due Date:    ";
  string ProblemWrite = "Problems:                              ";
  cout << NameWrite << " " << CourseWrite << " " << DueWrite << " " << ProblemWrite << endl;
  if (nm == "all")
  {
    for (auto &hw : stored)
    {
      string name = hw.name();
      string course = hw.course();
      string due = hw.dueString();
      string problems;
      bool frst = true;
      for (auto &prb : hw.allProbs())
      {
        if (!frst)
          problems += ", ";
        else
          frst = false;
        problems += prb.first + "-" + ((prb.second) ? "Unfinished" : "Finished");
      }

      string current;
      int maxLen, lstEx = 0;
      bool fstEx = true;
      bool disNm = true, disCls = true, disDue = true, disProbs = true;

      while (name.size() > NameWrite.size() || course.size() > CourseWrite.size()
      || due.size() > DueWrite.size() || problems.size() > ProblemWrite.size() || fstEx || lstEx != 1)
      {
        if (fstEx)
          fstEx = false;
        if (name.size() <= NameWrite.size() && course.size() <= CourseWrite.size()
        && due.size() <= DueWrite.size() && problems.size() <= ProblemWrite.size())
          ++lstEx;
        for (unsigned i = 0; i < 4; i++)
        {
          switch (i)
          {
            case 0:
              maxLen = NameWrite.size();
              if (!disNm)
              {
                for (unsigned a = 0; a < maxLen; a++)
                  cout << " ";
                cout << " ";
                continue;
              }
              current = name;
              if (name.size() > maxLen)
                name = name.substr(maxLen);
              if (name.size() <= maxLen)
                disNm = false;
              break;
            case 1:
            maxLen = CourseWrite.size();
              if (!disCls)
              {
                for (unsigned a = 0; a < maxLen; a++)
                  cout << " ";
                cout << " ";
                continue;
              }
              current = course;
              if (course.size() > maxLen)
                course = course.substr(maxLen);
                if (course.size() <= maxLen)
                  disCls = false;
              break;
            case 2:
              maxLen = DueWrite.size();
              if (!disDue)
              {
                for (unsigned a = 0; a < maxLen; a++)
                  cout << " ";
                cout << " ";
                continue;
              }
              current = due;
              if (due.size() > maxLen)
                due = due.substr(maxLen);
              if (due.size() <= maxLen)
                disDue = false;
              break;
            case 3:
              maxLen = ProblemWrite.size();
              if (!disProbs)
                continue;
              current = problems;
              if (problems.size() > maxLen)
                problems = problems.substr(maxLen);
              break;
          }
          if (current.size() > maxLen)
            cout << current.substr(0, maxLen);

          if (current.size() <= maxLen)
          {
            cout << current;
            int addSp = maxLen - current.size();
            for (unsigned k = 0; k < addSp; k++)
              cout << " ";
          }
          cout << " ";
        }
        cout << endl;
      }
      cout << endl;
    }
  }
}

vector <Homework> readHw ()
{
  ifstream rdr (".homework.txt");
  vector <Homework> rtrnHW;

  string strLn;
  while (getline(rdr, strLn))
  {
    string name;
    string course;
    map <string, bool> problems;
    tuple <int, int, int> dueDate;
    int tmp1, tmp2, tmp3;
    istringstream sRdr(strLn);

    sRdr >> name >> course >> tmp1 >> tmp2 >> tmp3;
    dueDate = make_tuple(tmp1, tmp2, tmp3);

    string pStr;
    while (sRdr >> pStr)
    {
      bool pB;
      sRdr >> pB;
      problems[pStr] = pB;
    }
    Homework storedHw (name, course, problems, dueDate);
    rtrnHW.push_back(storedHw);
  }
  return rtrnHW;
}

void writeHw (vector <Homework> stored)
{
  ofstream wrtr (".homework.txt");

  for (unsigned i = 0, sz = stored.size(); i < sz; i++)
  {
    wrtr << stored[i].name() << " " << stored[i].course() << " " << stored[i].dMonth()
    << " " << stored[i].dDay() << " " << stored[i].dYear() << " ";

    map <string, bool> prbs = stored[i].allProbs();

    for (auto &n : prbs)
    {
      wrtr << n.first << " " << n.second << " ";
    }

    wrtr << endl;
  }
}

int main (int argc, char* argv[])
{
  if (argc != 2 && argc != 3)
  {
    cout << "Usage: ./homework_planner planner/(add/rm/view ASSIGNMENT/all)" << endl;
    return 1;
  }

  vector<Homework> stored = readHw();

  bool changed = false;

  if (argc == 2)
  {
    string mode = argv[1];
    if (mode == "planner")
      plannerMode(changed);
    else
      cout << "Usage: ./homework_planner planner/(add/rm/view ASSIGNMENT/all)" << endl;
    return 0;
  }

  else if (argc == 3)
  {
    string action = argv[1];
    string assntNm = argv[2];

    if (action == "add")
    {
      if (assntNm == "all")
      {
        cout << "ERROR. Cannot add all." << endl;
        return 2;
      }
      addAssnmt (assntNm, stored, changed);
    }

    else if (action == "rm")
    {
      rmAssnmt (assntNm);
      changed = true;
    }

    else if (action == "view")
      viewAssnmt (assntNm, stored);
    else
    {
      cout << "Usage: ./homework_planner planner/(add/rm/view ASSIGNMENT/all)" << endl;
      return 1;
    }
  }

  if (changed)
    writeHw(stored);
}
