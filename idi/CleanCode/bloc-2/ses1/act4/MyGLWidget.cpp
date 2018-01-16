#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glEnable(GL_DEPTH_TEST); //algorisme d'eliminació de parts amagades ATENCIÓ IMPORTANT!!!
  ini_camera();
}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform ();
  pintaModel1();

  glBindVertexArray (0);
}


void MyGLWidget::resizeGL (int w, int h)
{
  glViewport(0, 0, w, h);
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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

void MyGLWidget::createBuffers ()
{
  carregaModel1();

  glBindVertexArray (0);
}

void MyGLWidget::carregaModel1(){
  model1.load("../../../../bloc-2/models/HomerProves.obj");

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Model1);
  glBindVertexArray(VAO_Model1);

  glGenBuffers(1, &VBO_Model1Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Pos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_vertices(), GL_STATIC_DRAW);
  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Model1Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Col);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_matdiff(), GL_STATIC_DRAW);
  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
}

void MyGLWidget::modelTransform ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::pintaModel1(){
  // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_Model1);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, model1.faces().size()*3);
}

void MyGLWidget::ini_camera(){
  FOV = (float)M_PI/2.0f;
  ra = 1.0f;
  znear = 0.4f;
  zfar = 3.0f;
  projectTransform();

  OBS = glm::vec3(0, 0, 1);
  VRP = glm::vec3(0, 0, 0);
  UP = glm::vec3(0, 1, 0);
  viewTransform();
}

void MyGLWidget::projectTransform(){
  //perspective(FOV,ra,znear, zfar)
  glm::mat4 Proj = glm::perspective(FOV, ra, znear, zfar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(){
  glm::mat4 View = glm::lookAt(OBS, VRP, UP);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}
