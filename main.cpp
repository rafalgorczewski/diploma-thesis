#include <lsl_cpp.h>

#include <QApplication>
#include <QPushButton>

#include <vector>
#include <iostream>

#include "buffer.hpp"
#include "window.hpp"

using namespace lsl;

int main(int argc, char *argv[]) {
  setenv("DISPLAY", "172.23.32.1:0", true);
  setenv("LIBGL_ALWAYS_INDIRECT", "1", true);

  QApplication a(argc, argv);

  th::Window window;
  window.show();

  return QApplication::exec();
}
