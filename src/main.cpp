#include "artie.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  artie artie;
  artie.show();
  return app.exec();
}
