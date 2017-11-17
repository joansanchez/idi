#include "disco.h"

disco::disco(QWidget *parent) : QLCDNumber(parent){

}
void disco::canviarvalor(int val){
  valor = val;
  reescriu();
}

void disco::valor0(){
  valor = 0;
  reescriu();
}

void disco::reescriu(){
  emit display(valor);
  if (valor == 0) {
    emit setStyleSheet("color: green");
  }
  else if (valor%2 == 0){
    emit setStyleSheet("color: blue");
  }
  else {
    emit setStyleSheet("color: red");
  }
}
