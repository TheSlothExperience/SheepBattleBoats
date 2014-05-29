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

    void updateHistogram(unsigned char histogram[]);
protected:
    void paintEvent(QPaintEvent *event);

    unsigned char histogram[256];
    unsigned char transferFunction[256][4];
};

#endif
