#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent = 0):QLabel(parent)
{
  this->setAutoFillBackground(true);
  red = green = blue = 0;
  QString aux = QString("rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");
}

void MyLabel::changered(int redvalue)
{
  red = redvalue;
  this->setAutoFillBackground(true);
  QString aux = QString("rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");
}

void MyLabel::changegreen(int greenvalue)
{
  green = greenvalue;
  this->setAutoFillBackground(true);
  QString aux = QString("rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");
}

void MyLabel::changeblue(int bluevalue)
{
  blue = bluevalue;
  this->setAutoFillBackground(true);
  QString aux = QString("rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");

}

void MyLabel::send(){
  emit setcolours(red, green, blue);
}
