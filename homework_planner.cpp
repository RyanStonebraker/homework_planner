// homework_planner.cpp
// Ryan Stonebraker
// 1/1/2017
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
#include <algorithm>
using std::equal_range;
#include <sys/ioctl.h>
#include <cmath>
using std::abs;
#include <cstdlib>
#include <random>
using std::mt19937;
using std::random_device;
using std::uniform_real_distribution;

// TODO: fix spacing issue with long names, fix continuation to next line also w/long
// names, add to planner mode and open new larger console window, view all command
// clears screen, add notifications to calendar, move from just console

// make a function that randomly chooses a homework assignment out of the nearest
// homework assignments due and shows it on screen to start

// sort by date due in view

// refactor part of code so that a separate function keeps column width
// given the parameter of a width to make more modular

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
        if (month > 12)
        {
          cout << "Month can't be greater than 12!" <<endl;
          return;
        }
        break;
      case 1:
        toInt >> day;
        if (day > 31)
        {
          cout << "Invalid Day!" << endl;
          return;
        }
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

void showProb (Homework hw, bool & top)
{
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  // Name .25, Course .2, Due Date .15, Problems .4
  string NameWrite = "Name:";
  string CourseWrite = "Course:";
  string DueWrite = "Due Date:";
  string ProblemWrite = "Problems:";

  for (unsigned i = 0, nw = NameWrite.size(), cw = CourseWrite.size(), dw = DueWrite.size(),
  pw = ProblemWrite.size(); i < w.ws_col; i++)
  {
    if (i <= w.ws_col*.2 - nw)
      NameWrite += " ";
    if (i <= w.ws_col*.2 - cw)
      CourseWrite += " ";
    if (i <= w.ws_col*.15 - dw)
      DueWrite += " ";
    if (i <= w.ws_col*.45 - pw)
      ProblemWrite += " ";
  }

  int totSz = NameWrite.size() + CourseWrite.size() + DueWrite.size() + ProblemWrite.size();
  int over = abs(w.ws_col-totSz);

  if (totSz > w.ws_col)
      ProblemWrite = ProblemWrite.substr(0, ProblemWrite.size() - over);
  if (top)
  {
    cout << NameWrite << CourseWrite << DueWrite << ProblemWrite << endl;
    top = false;
  }

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
    problems += prb.first/* + "-" + ((prb.second) ? "Unfinished" : "Finished")*/;
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
            continue;
          }
          current = name;
          if (name.size() > maxLen)
            name = name.substr(maxLen, name.size() - maxLen);
          if (name.size() <= maxLen)
            disNm = false;
          break;
        case 1:
        maxLen = CourseWrite.size();
          if (!disCls)
          {
            for (unsigned a = 0; a < maxLen; a++)
              cout << " ";
            continue;
          }
          current = course;
          if (course.size() > maxLen)
            course = course.substr(maxLen, course.size() - maxLen);
            if (course.size() <= maxLen)
              disCls = false;
          break;
        case 2:
          maxLen = DueWrite.size();
          if (!disDue)
          {
            for (unsigned a = 0; a < maxLen; a++)
              cout << " ";
            continue;
          }
          current = due;
          if (due.size() > maxLen)
            due = due.substr(maxLen, due.size() - maxLen);
          if (due.size() <= maxLen)
            disDue = false;
          break;
        case 3:
          maxLen = ProblemWrite.size();
          if (!disProbs)
            continue;
          current = problems;
          if (problems.size() > maxLen)
            problems = problems.substr(maxLen, problems.size() - maxLen);
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
    }
    cout << endl;
  }
  cout << endl;
}

void rmAssnmt (string nm, vector <Homework> & stored)
{
  vector <int> locs;
  for (unsigned i = 0, sz = stored.size(); i < sz; i++)
  {
    if (stored[i].name() == nm)
      locs.push_back(i);
  }
  if (locs.size() == 0)
  {
    if (nm == "all")
    {
      vector <Homework> blnk;
      while (true)
      {
      cout << "Are you sure you want to remove all assignments?(y/n): ";
      string entry;
      getline (cin, entry);
      if (entry == "y")
      {
        stored = blnk;
        break;
      }

      else if (entry == "n")
        break;

      else if (entry != "n" && entry != "y")
      {
        cout << "Bad Entry." << endl;
        continue;
      }
      }
    }
    else
      cout << "No Assignment to Remove!" << endl;
  }
  else if (locs.size() == 1)
  {
    stored.erase(stored.begin()+locs[0]);
  }
  else if (locs.size() > 1)
  {
    bool top = true;
    cout << "Multiple Assignments with this name found!" << endl;
    for (unsigned k = 0, size = locs.size(); k < size; k++)
    {
      cout << "(" << k+1 << "): " << endl;
      showProb(stored[locs[k]], top);
    }
    cout << "Which would you like to remove?: ";
    string temp, ofl;
    int val;
    getline (cin, temp);
    istringstream iss (temp);
    iss >> val;
    if (iss >> ofl)
    {
      cout << "Bad Entry!" << endl;
      return;
    }
    bool exists = false;
    for (unsigned a = 0, szL = locs.size(); a < szL; a++)
    {
      if (a == val -1)
        exists = true;
    }
    if (exists)
      stored.erase(stored.begin() + locs[val-1]);
    else
      cout << "Entry does not exist!" << endl;
  }
}

void viewAssnmt (const string & nm, vector <Homework> & stored)
{
  system("clear");
  bool top;
  if (nm == "all")
  {
    top = true;
    for (auto &hw : stored)
      showProb(hw, top);
  }
  else
  {
    top = true;
    vector <int> loc;
    for (unsigned i = 0, sz = stored.size(); i < sz; i++)
    {
      if (stored[i].name() == nm)
        loc.push_back(i);
    }
    if (loc.size() != 0)
    {
      for (auto &cLoc : loc)
        showProb(stored[cLoc], top);
    }
    else
      cout << "Could not find \"" << nm << "\"!" << endl;
  }
}

vector <int> findHw (const string & nm, vector <Homework> & stored)
{
  vector <int> loc;
  for (unsigned i = 0; i < stored.size(); i++)
  {
    if (stored[i].name() == nm)
      loc.push_back(i);
  }
  return loc;
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

void plannerMode (vector <Homework> & stored, bool & changed)
{
  // TODO: open new console w/specified height and width running plannerMode
  // function
  system ("clear");
  cout << "Welcome to Homework Planner!" << endl;
  cout << "In this mode, you easily have access to all of your stored \
  entries and can make changes to or add/remove any of them." << endl;

  while (true)
  {
    system("clear");
    viewAssnmt ("all", stored);
    cout << "add/remove/modify/(random Choose) which assignment?: ";
    string temp, entry, assn, ofl;
    getline (cin, temp);
    istringstream iss (temp);
    iss >> entry >> assn;
    if (!iss || iss >> ofl)
    {
      cout << "Bad Entry!" << endl;
      continue;
    }
    else if (entry == "add")
      addAssnmt (assn, stored, changed);

    else if (entry == "rm" || entry == "remove")
    {
      rmAssnmt (assn, stored);
      changed = true;
    }

    else if (entry == "modify" || entry == "mod")
    {
      cout << endl;
      viewAssnmt (assn, stored);
      cout << "What would you like to modify? (name/class/date/problems): ";
      string modMe;
      getline (cin, modMe);
      if (modMe == "name")
      {
        cout << "Enter new name: ";
        string nwNm;
        getline (cin, nwNm);

        vector <int> loc = findHw(assn, stored);
      }
    }
    else if ((entry == "Random" || entry == "random") && (assn == "Choose" || assn == "choose"))
    {
      random_device seed;
      mt19937 rndNum (seed());
      uniform_real_distribution <double> rng (0, stored.size()-1);

      int doHw = rng (rndNum);
      cout << "Start homework: " << stored[doHw].name() << " for " << stored[doHw].course();
      cin.ignore();
    }
    if (changed)
      writeHw (stored);
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
      plannerMode(stored, changed);
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
      rmAssnmt (assntNm, stored);
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
