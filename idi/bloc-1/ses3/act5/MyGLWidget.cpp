#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  scl = 0.5;
  scy = 0.5;
  degrees = 0.0;
  values = glm::vec3(0.0,0.0,0.0);
  x = glm::vec3(1.0,0.0,0.0);
  y = glm::vec3(0.0,1.0,0.0);
  z = glm::vec3(0.0,0.0,1.0);

  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();

  glm::mat4 TG(1.0); //Matriu identidat
  TG = glm::translate (TG,values);
  glUniformMatrix4fv(transLoc,1,GL_FALSE,&TG[0][0]);
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  modelTransform();
  pinta_triangle();

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  carregaTriangle();

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  transLoc = glGetUniformLocation (program->programId(), "matTG");
}


void MyGLWidget::carregaTriangle()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.5, -0.5, 0.0);
  Vertices[1] = glm::vec3(0.5, -0.5, 0.0);
  Vertices[2] = glm::vec3(0.0, 0.5, 0.0);

  glm::vec3 Colors[3];  // Tres vèrtexs amb X, Y i Z
  Colors[0] = glm::vec3(1.0, 0.0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(2, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer amb els colors dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::pinta_triangle()
{
	// Activem l'Array a pintar
  glBindVertexArray(VAO);

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::modelTransform(){
  glm::mat4 matTG(1.0); //Matriu identidat
  matTG = glm::translate (matTG,values);
  matTG = glm::rotate(matTG,degrees,z);
  matTG = glm::scale(matTG,glm::vec3(scl,scy,1.0));
  glUniformMatrix4fv(transLoc,1,GL_FALSE,&matTG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
  makeCurrent();
  switch(e->key()){
    case Qt::Key_S:
      scl += 0.1;
      break;
    case Qt::Key_D:
      scy += 0.1;
      break;
    case Qt::Key_X:
      scl -= 0.1;
      break;
    case Qt::Key_C:
      scy -= 0.1;
      break;
    case Qt::Key_R:
      degrees += (float)(M_PI/4);
      break;
    case Qt::Key_E:
      degrees -= (float)(M_PI/4);
      break;
    case Qt::Key_Left :
      values[0] -= 0.1;
      break;
    case Qt::Key_Right :
      values[0] += 0.1;
      break;
    case Qt::Key_Up :
      values[1] +=0.1;
      break;
    case Qt::Key_Down :
      values[1] -= 0.1;
      break;
    default: e->ignore();
  }
  update();
}
