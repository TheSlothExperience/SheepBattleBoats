#include <math.h>
#include <iostream>
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
	painter.setPen(QColor(0, 180, 10));
	painter.drawPoint(i, 255 - transferFunction[i][1]);
	//blue
	painter.setPen(QColor(0, 0, 255));
	painter.drawPoint(i, 255 - transferFunction[i][2]);
	//alpha
	painter.setPen(QColor(0, 0, 0));
	painter.drawPoint(i, 255 - transferFunction[i][3]);
    }
}

void TfDisplay::smoothTF() {
    const double PI = 3.141592653589793238463;

    //Build the Gaussian kernel
    float variance = 0.5;
    float normalization = 1.0/sqrt(2 * PI * variance);
        
    static float gaussian[5];
    for(int i = -2; i <= 2; i++) {
	gaussian[i+2] = normalization * exp(- (float)(i * i) / (2 * variance));
    }

    static float acc_r[256];
    static float acc_g[256];
    static float acc_b[256];
    static float acc_a[256];

    //Handle the corner cases, by clamping the value (like GL_REPEAT)
    {
    	acc_r[0] = 0;
    	acc_g[0] = 0;
    	acc_b[0] = 0;
    	acc_a[0] = 0;
    	if(red) acc_r[0] += gaussian[0] * (float)transferFunction[0][0];
    	if(green) acc_g[0] += gaussian[0] * (float)transferFunction[0][1];
    	if(blue) acc_b[0] += gaussian[0] * (float)transferFunction[0][2];
    	if(alpha) acc_a[0] += gaussian[0] * (float)transferFunction[0][3];
	
    	if(red) acc_r[0] += gaussian[1] * (float)transferFunction[0][0];
    	if(green) acc_g[0] += gaussian[1] * (float)transferFunction[0][1];
    	if(blue) acc_b[0] += gaussian[1] * (float)transferFunction[0][2];
    	if(alpha) acc_a[0] += gaussian[1] * (float)transferFunction[0][3];

    	for(int x = 0; x <= 2; x++) {
    	    if(red) acc_r[0] += gaussian[x+2] * (float)transferFunction[0 + x][0];
    	    if(green) acc_g[0] += gaussian[x+2] * (float)transferFunction[0 + x][1];
    	    if(blue) acc_b[0] += gaussian[x+2] * (float)transferFunction[0 + x][2];
    	    if(alpha) acc_a[0] += gaussian[x+2] * (float)transferFunction[0 + x][3];
    	}
    }

    {
    	acc_r[1] = 0;
    	acc_g[1] = 0;
    	acc_b[1] = 0;
    	acc_a[1] = 0;
    	if(red) acc_r[1] += gaussian[0] * (float)transferFunction[0][0];
    	if(green) acc_g[1] += gaussian[0] * (float)transferFunction[0][1];
    	if(blue) acc_b[1] += gaussian[0] * (float)transferFunction[0][2];
    	if(alpha) acc_a[1] += gaussian[0] * (float)transferFunction[0][3];

    	for(int x = -1; x <= 2; x++) {
    	    if(red) acc_r[1] += gaussian[x+2] * (float)transferFunction[1 + x][0];
    	    if(green) acc_g[1] += gaussian[x+2] * (float)transferFunction[1 + x][1];
    	    if(blue) acc_b[1] += gaussian[x+2] * (float)transferFunction[1 + x][2];
    	    if(alpha) acc_a[1] += gaussian[x+2] * (float)transferFunction[1 + x][3];
    	}
    }

    //The common case
    for(int i = 2; i < 254; i++) {
	acc_r[i] = 0;
	acc_g[i] = 0;
	acc_b[i] = 0;
	acc_a[i] = 0;
	for(int x = -2; x <= 2; x++) {
	    if(red) acc_r[i] += gaussian[x+2] * (float)transferFunction[i + x][0];
	    if(green) acc_g[i] += gaussian[x+2] * (float)transferFunction[i + x][1];
	    if(blue) acc_b[i] += gaussian[x+2] * (float)transferFunction[i + x][2];
	    if(alpha) acc_a[i] += gaussian[x+2] * (float)transferFunction[i + x][3];
	}
    }

    //Again the rightmost corner cases
    {
    	acc_r[254] = 0;
    	acc_g[254] = 0;
    	acc_b[254] = 0;
    	acc_a[254] = 0;

    	for(int x = -2; x <= 1; x++) {
    	    if(red) acc_r[254] += gaussian[x+2] * (float)transferFunction[254 + x][0];
    	    if(green) acc_g[254] += gaussian[x+2] * (float)transferFunction[254 + x][1];
    	    if(blue) acc_b[254] += gaussian[x+2] * (float)transferFunction[254 + x][2];
    	    if(alpha) acc_a[254] += gaussian[x+2] * (float)transferFunction[254 + x][3];
    	}
	
    	if(red) acc_r[254] += gaussian[4] * (float)transferFunction[255][0];
    	if(green) acc_g[254] += gaussian[4] * (float)transferFunction[255][1];
    	if(blue) acc_b[254] += gaussian[4] * (float)transferFunction[255][2];
    	if(alpha) acc_a[254] += gaussian[4] * (float)transferFunction[255][3];
    }
    
    {
    	acc_r[255] = 0;
    	acc_g[255] = 0;
    	acc_b[255] = 0;
    	acc_a[255] = 0;

    	for(int x = -2; x <= 0; x++) {
    	    if(red) acc_r[255] += gaussian[x+2] * (float)transferFunction[255 + x][0];
    	    if(green) acc_g[255] += gaussian[x+2] * (float)transferFunction[255 + x][1];
    	    if(blue) acc_b[255] += gaussian[x+2] * (float)transferFunction[255 + x][2];
    	    if(alpha) acc_a[255] += gaussian[x+2] * (float)transferFunction[255 + x][3];
    	}
	
    	if(red) acc_r[255] += gaussian[3] * (float)transferFunction[255][0];
    	if(green) acc_g[255] += gaussian[3] * (float)transferFunction[255][1];
    	if(blue) acc_b[255] += gaussian[3] * (float)transferFunction[255][2];
    	if(alpha) acc_a[255] += gaussian[3] * (float)transferFunction[255][3];
	
    	if(red) acc_r[255] += gaussian[4] * (float)transferFunction[255][0];
    	if(green) acc_g[255] += gaussian[4] * (float)transferFunction[255][1];
    	if(blue) acc_b[255] += gaussian[4] * (float)transferFunction[255][2];
    	if(alpha) acc_a[255] += gaussian[4] * (float)transferFunction[255][3];
    }

    //Now copy the blurred results back
    for(int i = 0; i < 256; i++) {
	if(red) transferFunction[i][0] = acc_r[i];
	if(green) transferFunction[i][1] = acc_g[i];
	if(blue) transferFunction[i][2] = acc_b[i];
	if(alpha) transferFunction[i][3] = acc_a[i];
    }
    update();
    emit tfChanged();
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

void TfDisplay::resetTF() {
    for(int i = 0; i < 256; i++) {
	if(red) transferFunction[i][0] = i;
	if(green) transferFunction[i][1] = i;
	if(blue) transferFunction[i][2] = i;
	if(alpha) transferFunction[i][3] = i;
    }
    update(); 
    emit tfChanged(); 
}

void TfDisplay::setTF(unsigned char newTF[][4]) {
    for(int i = 0; i < 256; i++) {
	transferFunction[i][0] = newTF[i][0];
	transferFunction[i][1] = newTF[i][1]; 
	transferFunction[i][2] = newTF[i][2]; 
	transferFunction[i][3] = newTF[i][3]; 
    }
    update(); 
    emit tfChanged(); 
}
