#include "mainwindow.h"
#include <QApplication>

// you found my secret comment.

#define RESTART_CODE 1000

int main(int argc, char *argv[]) {

  int return_code = 1000;
  while (return_code == RESTART_CODE) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return_code = a.exec();
  }

  return return_code;
}
