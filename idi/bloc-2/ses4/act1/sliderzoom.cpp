#include "sliderzoom.h"

sliderzoom::sliderzoom(QWidget *parent):QSlider(parent){

}

void sliderzoom::canviarzoom(int valor){
  float FOVaux = valor/M_PI;
  FOV = FOVaux;
  projectTransform();
}
