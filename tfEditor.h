#ifndef TFEDITOR_H
#define TFEDITOR_H

#include <QtGui>
#include <QtWidgets>

#include "tfDisplay.h"

class TfEditor : public QDockWidget
{
    Q_OBJECT

public:
    TfEditor(QWidget *parent = 0);

    void updateHistogram(unsigned char histogram[]);
private:
    QCheckBox *redCheckBox;
    QCheckBox *greenCheckBox;
    QCheckBox *blueCheckBox;
    QCheckBox *alphaCheckBox;

    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *resetButton;
    QPushButton *smoothButton;
    QPushButton *closeButton;

    TfDisplay tfDisplay;

public slots:
    void setRed();
    void setGreen();
    void setBlue();
    void setAlpha();
};

#endif
