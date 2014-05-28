#ifndef TFDISPLAY_H
#define TFDISPLAY_H

#include <QtGui>
#include <QtWidgets>

class TfDisplay : public QWidget
{
    Q_OBJECT

public:
    TfDisplay(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif
