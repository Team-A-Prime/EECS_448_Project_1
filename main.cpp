#include "mainwindow.h"
#include <QApplication>

// you found my secret comment.

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
