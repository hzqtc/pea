#include "mainform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainForm form;
    form.show();
    app.exec();
    return 0;
}
