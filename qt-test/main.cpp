#include <QApplication>
#include <QPushButton>
#include "view.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    View view;
    view.show();
    return QApplication::exec();
}
