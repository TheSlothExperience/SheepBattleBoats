#include "tfEditor.h"
#include <QGridLayout>
#include <QBoxLayout>

TfEditor::TfEditor(QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle("Transfer Function Awesomizer");
    redCheckBox = new QCheckBox("red");
    redCheckBox->setChecked(true);
    greenCheckBox = new QCheckBox("green");
    greenCheckBox->setChecked(true);
    blueCheckBox = new QCheckBox("blue");
    blueCheckBox->setChecked(true);
    alphaCheckBox = new QCheckBox("alpha");
    alphaCheckBox->setChecked(true);

    
    loadButton = new QPushButton("Load");
    saveButton = new QPushButton("Save");
    resetButton = new QPushButton("Reset");
    smoothButton = new QPushButton("Smooth");
    closeButton = new QPushButton("Close");


    QWidget *contents = new QWidget;
    QGridLayout *mainLayout = new QGridLayout(contents);
    mainLayout->addWidget(&tfDisplay, 0, 0, 3, 3, Qt::AlignCenter);
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
