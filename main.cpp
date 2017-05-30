#include "screenshoter.h"

#include <QApplication>

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    Screenshoter w;
    w.show();

    return a.exec();
}
