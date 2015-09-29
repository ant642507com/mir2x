// generated by Fast Light User Interface Designer (fluid) version 1.0303

#ifndef wilfilewindow_hpp
#define wilfilewindow_hpp
#include <FL/Fl.H>
#include <string>
#include <algorithm>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Browser.H>

class WilFileWindow {
public:
  WilFileWindow();
private:
  Fl_Double_Window *m_Window;
  Fl_Browser *m_Browser;
public:
  void ShowAll();
};
#endif
