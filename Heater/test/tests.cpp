#include <iostream>
#include <cstdlib>

#include "../DigitalFilter.h"
using namespace std;

int main() {
  cout << "run tests:" << endl;
  DigitalFilter df(0);
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(5):" << (int)df.doFilter(5) << endl;
  cout << "df.dofilter(-1):" << (int)df.doFilter(-1) << endl;
  cout << "df.dofilter(0):" << (int)df.doFilter(0) << endl;
  cout << "df.dofilter(0):" << (int)df.doFilter(0) << endl;
  cout << "df.dofilter(0):" << (int)df.doFilter(0) << endl;
  cout << "df.dofilter(0):" << (int)df.doFilter(0) << endl;
  cout << "df.dofilter(0):" << (int)df.doFilter(0) << endl;
}
