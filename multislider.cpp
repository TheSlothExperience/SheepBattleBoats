#include "multislider.h"

MultiSlider::MultiSlider(QWidget* parent)
	: QSlider(parent)
{

}

MultiSlider::MultiSlider(Qt::Orientation orientation, QWidget* parent)
	: QSlider(orientation, parent)
{

}

MultiSlider::~MultiSlider()
{

}

void MultiSlider::keyPressEvent(QKeyEvent* event)
{
	QSlider::keyPressEvent(event);
}

void MultiSlider::mousePressEvent(QMouseEvent* event)
{

}

void MultiSlider::mouseMoveEvent(QMouseEvent* event)
{

}

void MultiSlider::mouseReleaseEvent(QMouseEvent* event)
{

}

void MultiSlider::paintEvent(QPaintEvent* event)
{

}
