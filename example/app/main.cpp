#include <QApplication>
#include <QPushButton>

#include "viewmodel/viewmodel.hpp"
#include "window/window.hpp"
#include "common_parameter.h"
#include "app.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto pVM = std::make_shared<Viewmodel>();
    Window window;

    init(pVM.get(),&window);

    pVM->setInitNumber(init_time());
    //test
    window.view_.onClick();

    window.view_.show();
    return QApplication::exec();
}
