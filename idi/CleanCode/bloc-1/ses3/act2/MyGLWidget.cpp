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

  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();

  //inicialització variables pròpies
  glm::mat4 TGinicial(1.0);
  glUniformMatrix4fv(transLoc,1, GL_FALSE, &TGinicial[0][0]);

  escalarX = 1.0;
  escalarY = 1.0;
  graus = 0;
  //fi d'inicialització variables pròpies
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  //modeifiquem la TG abans d'enviar a pintar el model
  modelTransformacioTriangle();
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
  // Obtenim identificador per a l'atribut "color" del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Obtenim identificador per al uniform "color" del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");

}


void MyGLWidget::carregaTriangle()
{
  //Enviem posició dels vèrtexs
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-0.5, -0.5, 0.0);
  Vertices[1] = glm::vec3(0.5, -0.5, 0.0);
  Vertices[2] = glm::vec3(0.0, 0.5, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  //Enviem color dels vèrtexs
  glm::vec3 Colors[3];  // Colors dels tres vèrtexs X, Y i Z
  Colors[0] = glm::vec3(1.0, 0.0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);

  // Creació del buffer amb les dades dels colors dels vèrtexs
  glGenBuffers(1, &VBOCOLOR);
  glBindBuffer(GL_ARRAY_BUFFER, VBOCOLOR);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

}

void MyGLWidget::modelTransformacioTriangle(){
  glm::mat4 TG(1.0);
  TG = glm::rotate(TG, graus, glm::vec3(0.0, 0.0, 1.0));
  TG = glm::scale(TG,glm::vec3(escalarX, escalarY, 1.0));
  glUniformMatrix4fv(transLoc,1,GL_FALSE, &TG[0][0]);
}

void MyGLWidget::pinta_triangle()
{
	// Activem l'Array a pintar
  glBindVertexArray(VAO);

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
  makeCurrent();
  switch(e->key()){
    case Qt::Key_Right:
      escalarX += 0.1;
      break;
    case Qt::Key_Left:
      escalarX -= 0.1;
      break;
    case Qt::Key_Up:
      escalarY += 0.1;
      break;
    case Qt::Key_Down:
      escalarY -= 0.1;
      break;
    case Qt::Key_R:
      graus += (float)(M_PI/4);
      break;
    case Qt::Key_E:
      graus -= (float)(M_PI/4);
      break;
    default: e->ignore();
  }
  update();
}
