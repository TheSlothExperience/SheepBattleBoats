#ifndef MULTISLIDER_H
#define MULTISLIDER_H

#include <QSlider>

class MultiSlider : public QSlider
{
	Q_OBJECT

public:
	explicit MultiSlider(QWidget* parent = 0);
	explicit MultiSlider(Qt::Orientation orientation, QWidget* parent = 0);
	virtual ~MultiSlider();

private:
	std::vector<int> positions;
protected:
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void paintEvent(QPaintEvent* event);
};

#endif
