#include "mainform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainForm form;
    form.show();
    form.adjustSize();
    form.setMaximumHeight(form.height());
    app.exec();
    return 0;
}
