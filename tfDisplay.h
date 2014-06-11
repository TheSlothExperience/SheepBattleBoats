#ifndef TFDISPLAY_H
#define TFDISPLAY_H

#include <QtGui>
#include <QtWidgets>
#include <QPoint>

class TfDisplay : public QWidget
{
	Q_OBJECT

public:
	TfDisplay(QWidget *parent = 0);

	bool red = true;
	bool green = true;
	bool blue = true;
	bool alpha = true;
	bool drawing = false;
    
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void updateHistogram(unsigned char histogram[]);
	unsigned char (&getTF())[256][4] {return transferFunction;}
	void setTF(unsigned char newTF[][4]);

private:
	QPoint lastPoint;
signals:
	void tfChanged();
    
protected:
	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	unsigned char histogram[256];
	unsigned char transferFunction[256][4];

public slots:
	void smoothTF();
	void resetTF();
};

#endif
