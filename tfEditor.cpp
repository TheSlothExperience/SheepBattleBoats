#include "tfEditor.h"

#include <iostream>
#include <QGridLayout>
#include <QBoxLayout>
#include <QFile>
#include <QTextStream>

TfEditor::TfEditor(QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle("Transfer Function Awesomizer");
    redCheckBox = new QCheckBox("red");
    redCheckBox->setChecked(true);
    connect(redCheckBox, SIGNAL(toggled(bool)), this, SLOT(setRed(bool)));
    greenCheckBox = new QCheckBox("green");
    greenCheckBox->setChecked(true); 
    connect(greenCheckBox, SIGNAL(toggled(bool)), this, SLOT(setGreen(bool)));
    blueCheckBox = new QCheckBox("blue");
    blueCheckBox->setChecked(true);
    connect(blueCheckBox, SIGNAL(toggled(bool)), this, SLOT(setBlue(bool)));
    alphaCheckBox = new QCheckBox("alpha");
    alphaCheckBox->setChecked(true);
    connect(alphaCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAlpha(bool)));

    
    loadButton = new QPushButton("Load");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadTF()));
    saveButton = new QPushButton("Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveTF()));
    resetButton = new QPushButton("Reset");
    connect(resetButton, SIGNAL(clicked()), &tfDisplay, SLOT(resetTF()));
    smoothButton = new QPushButton("Smooth");
    connect(smoothButton, SIGNAL(clicked()), &tfDisplay, SLOT(smoothTF()));
    closeButton = new QPushButton("Close");


    QWidget *contents = new QWidget;
    QGridLayout *mainLayout = new QGridLayout(contents);
    mainLayout->addWidget(&tfDisplay, 0, 0, 3, 3, Qt::AlignCenter);
    connect(&tfDisplay, SIGNAL(tfChanged()), this, SLOT(changeTF()));
    mainLayout->addWidget(loadButton, 4, 0);
    mainLayout->addWidget(saveButton, 4, 1);
    mainLayout->addWidget(resetButton, 5, 0);
    mainLayout->addWidget(smoothButton, 5, 1);
    mainLayout->addWidget(closeButton, 6, 0);

    QBoxLayout *colorLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    colorLayout->addWidget(redCheckBox);
    colorLayout->addWidget(greenCheckBox);
    colorLayout->addWidget(blueCheckBox);
    colorLayout->addWidget(alphaCheckBox);

    mainLayout->addLayout(colorLayout, 4, 2, 3, 1);
    setWidget(contents);
}

void TfEditor::updateHistogram(unsigned char histogram[]) {
    tfDisplay.updateHistogram(histogram);
}

void TfEditor::changeTF() {
    emit tfChanged();
}

void TfEditor::saveTF() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Transfer Function"), ".");
    QFile f(fileName);
    if(f.open(QIODevice::WriteOnly))  {
	QTextStream out(&f);
	auto tf = tfDisplay.getTF();

	for(int i = 0; i < 256; i++) {
	    out << (int)tf[i][0] << " " << (int)tf[i][1] << " " <<(int) tf[i][2] << " " << (int)tf[i][3] << "\n";
	}
	f.close();
    }
}

void TfEditor::loadTF() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Transfer Function"), ".");
    QFile f(fileName);
    if(f.open(QIODevice::ReadOnly)) {
	QTextStream in(&f);

	unsigned char newTF[256][4];
	int r, g, b, a;
    
	for(int i = 0; i < 256; i++) {
	    in >> r >> g >> b >> a;
	    newTF[i][0] = (unsigned char)r;
	    newTF[i][1] = (unsigned char)g;
	    newTF[i][2] = (unsigned char)b;
	    newTF[i][3] = (unsigned char)a;
	}
	f.close();
	tfDisplay.setTF(newTF);
    }
}

void TfEditor::setRed(bool checked) {
    tfDisplay.red = checked;
}

void TfEditor::setGreen(bool checked) {
    tfDisplay.green = checked;
}

void TfEditor::setBlue(bool checked) {
    tfDisplay.blue = checked;
}

void TfEditor::setAlpha(bool checked) {
    tfDisplay.alpha = checked;
}
