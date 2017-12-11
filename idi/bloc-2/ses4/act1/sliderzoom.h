#include <QSlider>
#include "MyGLWidget.h"

class sliderzoom: public QSlider
{
  Q_OBJECT
  public slots:
    void canviarzoom(int valor);
}
