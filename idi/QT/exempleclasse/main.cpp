#include <QApplication>
#include "Test.h"

int main (int argc, char ** argv){
    QApplication app (argc, argv);
    Test form;
    form.show();
    return app.exec();
}
