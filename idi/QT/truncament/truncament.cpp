#include "truncament.h"

truncament::truncament(QWidget *parent):QLabel(parent){

}

void truncament::enviartext(QString text){
  noustring = text;
  reescriu();
}

void truncament::enviarvalor(int val){
  valoratruncar = val;
  reescriu();
}

void truncament::reescriu(){
  QString aux = noustring;
  aux.truncate(valoratruncar);
  emit setText(aux);
}
