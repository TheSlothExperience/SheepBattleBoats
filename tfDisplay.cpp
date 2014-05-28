#include "tfDisplay.h"

TfDisplay::TfDisplay(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFixedSize(256, 256);
}
      
QSize TfDisplay::sizeHint() const
{
    return QSize(256, 256);
}

QSize TfDisplay::minimumSizeHint() const
{
    return QSize(256, 256);
}


void TfDisplay::paintEvent(QPaintEvent *event) {
    
}
