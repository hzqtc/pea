#include "qclickablelabel.h"

void QClickableLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit clicked();
}
