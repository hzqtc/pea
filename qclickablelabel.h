#ifndef _PEA_CLICKABLELABEL_H_
#define _PEA_CLICKABLELABEL_H_

#include <QLabel>
#include <QMouseEvent>

class QClickableLabel: public QLabel
{
    Q_OBJECT

    public:
        QClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = 0)
            : QLabel(parent, f) {}
        QClickableLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0)
            : QLabel(text, parent, f) {}
        ~QClickableLabel() {}

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent *event);
};

#endif
