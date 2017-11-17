#include "MyLineEdit.h"

//constructor
MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent){
    //blabla
}

//implementació slots
void MyLineEdit::tractaReturn(){
    //implementació
    emit enviaText(text());
}
