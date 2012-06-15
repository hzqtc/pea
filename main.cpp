#include "mainform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainForm form;
    form.show();
    form.setMaximumSize(form.size());
    app.exec();
    return 0;
}
