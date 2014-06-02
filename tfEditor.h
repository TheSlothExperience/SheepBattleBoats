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

    TfDisplay tfDisplay;

    bool getMIP() {return mip;}
private:
    bool mip = false;
    
    QCheckBox *redCheckBox;
    QCheckBox *greenCheckBox;
    QCheckBox *blueCheckBox;
    QCheckBox *alphaCheckBox;
    
    QCheckBox *mipCheckBox;

    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *resetButton;
    QPushButton *smoothButton;
    QPushButton *closeButton;

signals:
    void tfChanged();

public slots:
    void changeTF();

    void saveTF();
    void loadTF();

    void setMIP(bool checked);
    
    void setRed(bool checked);
    void setGreen(bool checked);
    void setBlue(bool checked);
    void setAlpha(bool checked);
};

#endif
