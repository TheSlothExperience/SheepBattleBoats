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
    QPainter painter(this);
    painter.setPen(QColor(200, 200, 200));
    for(int i = 0; i < 256; i ++) {
	painter.drawLine(QLineF(i, 255, i, 255 - histogram[i]));
    }

    
    for(int i = 0; i < 256; i ++) {
	//red
	painter.setPen(QColor(255, 0, 0));
	painter.drawLine(QLineF(i, 255, i, 255 - transferFunction[i][0]));
	//green
	painter.setPen(QColor(0, 255, 0));
	painter.drawLine(QLineF(i, 255, i, 255 - transferFunction[i][1]));
	//blue
	painter.setPen(QColor(0, 0, 255));
	painter.drawLine(QLineF(i, 255, i, 255 - transferFunction[i][2]));
	//alpha
	painter.setPen(QColor(0, 0, 0));
	painter.drawLine(QLineF(i, 255, i, 255 - transferFunction[i][3]));
    }
}

void TfDisplay::updateHistogram(unsigned char h[]) {
    for(int i = 0; i < 256; i++) {
	histogram[i] = h[i];
    }
}
