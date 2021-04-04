#include <lsl_cpp.h>

#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <vector>

#include "buffer.hpp"
#include "window.hpp"

using namespace lsl;

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  th::Window window;
  window.show();

  return QApplication::exec();
}
