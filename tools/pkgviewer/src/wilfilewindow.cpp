// generated by Fast Light User Interface Designer (fluid) version 1.0303

#include "wilfilewindow.hpp"

WilFileWindow::WilFileWindow() {
  { m_Window = new Fl_Double_Window(497, 442);
    m_Window->user_data((void*)(this));
    { m_Browser = new Fl_Browser(0, 0, 505, 450);
      m_Browser->type(2);
      m_Browser->labelfont(4);
      m_Browser->textfont(4);
    } // Fl_Browser* m_Browser
    m_Window->end();
  } // Fl_Double_Window* m_Window
}

void WilFileWindow::ShowAll() {
  m_Window->show();
}
