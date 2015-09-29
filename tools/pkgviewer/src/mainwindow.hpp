// generated by Fast Light User Interface Designer (fluid) version 1.0302

#ifndef mainwindow_hpp
#define mainwindow_hpp
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Native_File_Chooser.H>
#include "progressbarwindow.hpp"
#include "wilfilewindow.hpp"
#include "previewwindow.hpp"
#include "misc.hpp"
#include <cstdint>
#include "wilimagepackage.hpp"
#include "aboutwindow.hpp"
#include <FL/Fl_Browser.H>
#include <algorithm>
#include <FL/fl_draw.H>

class MainWindow {
public:
  MainWindow();
private:
  Fl_Double_Window *m_Window;
  inline void cb_m_Window_i(Fl_Double_Window*, void*);
  static void cb_m_Window(Fl_Double_Window*, void*);
  Fl_Menu_Bar *m_MenuBar;
  static Fl_Menu_Item menu_m_MenuBar[];
  inline void cb_Open_i(Fl_Menu_*, void*);
  static void cb_Open(Fl_Menu_*, void*);
  inline void cb_Export_i(Fl_Menu_*, void*);
  static void cb_Export(Fl_Menu_*, void*);
  inline void cb_Export1_i(Fl_Menu_*, void*);
  static void cb_Export1(Fl_Menu_*, void*);
  inline void cb_Exit_i(Fl_Menu_*, void*);
  static void cb_Exit(Fl_Menu_*, void*);
  inline void cb_About_i(Fl_Menu_*, void*);
  static void cb_About(Fl_Menu_*, void*);
  Fl_Browser *m_Browser;
  inline void cb_m_Browser_i(Fl_Browser*, void*);
  static void cb_m_Browser(Fl_Browser*, void*);
public:
  void ShowAll();
  uint32_t SelectedImageIndex();
  void ActivateAll();
  void DeactivateAll();
};
#endif
