#include "tfDisplay.h"

TfDisplay::TfDisplay(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFixedSize(256, 256);
    
    for(int i = 0; i < 256; i++) {
	transferFunction[i][0] = i;
	transferFunction[i][1] = i;
	transferFunction[i][2] = i;
	transferFunction[i][3] = i;
    }
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
	painter.drawPoint(i, 255 - transferFunction[i][0]);
	//green
	painter.setPen(QColor(0, 255, 0));
	painter.drawPoint(i, 255 - transferFunction[i][1]);
	//blue
	painter.setPen(QColor(0, 0, 255));
	painter.drawPoint(i, 255 - transferFunction[i][2]);
	//alpha
	painter.setPen(QColor(0, 0, 0));
	painter.drawPoint(i, 255 - transferFunction[i][3]);
    }
}

void TfDisplay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
	int x = event->x();
	int y = 255 - event->y();

	if(red) transferFunction[x][0] = y;
	if(green) transferFunction[x][1] = y;
	if(blue) transferFunction[x][2] = y;
	if(alpha) transferFunction[x][3] = y;
	drawing = true;
	update();
	emit tfChanged();
    }
}

void TfDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing) {
	int x = event->x();
	int y = 255 - event->y();

	if(red) transferFunction[x][0] = y;
	if(green) transferFunction[x][1] = y;
	if(blue) transferFunction[x][2] = y;
	if(alpha) transferFunction[x][3] = y;
	update();
	emit tfChanged();
    }
}

void TfDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
	drawing = false;
    }
}

void TfDisplay::updateHistogram(unsigned char h[]) {
    for(int i = 0; i < 256; i++) {
	histogram[i] = h[i];
    }
    update();
}
