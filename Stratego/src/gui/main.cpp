#include <QApplication>
#include <QMessageBox>
#include <QScreen>

#include "vcstuff.h"

using namespace stratego;

int main(int argc, char *argv[]){
    Config::setDynamicResources(argv[0]);

    QApplication a(argc, argv);
    Model* model {new Stratego};
    Controller controller {model};
    int ret;

    controller.start();
    ret = a.exec();
    delete model;

    return ret;
}
