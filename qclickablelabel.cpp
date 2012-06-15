#include "qclickablelabel.h"

QClickableLabel::QClickableLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
}

QClickableLabel::QClickableLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
}

QClickableLabel::~QClickableLabel()
{
}

void QClickableLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit clicked();
}
