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
  degrees = 0.0;
  values = glm::vec3(0.0,0.0,0.0);
  x = glm::vec3(1.0,0.0,0.0);
  y = glm::vec3(0.0,1.0,0.0);
  z = glm::vec3(0.0,0.0,1.0);

  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  modelTransform();
  pinta_triangle();

  modelTransform2();
  pinta_triangle2();

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
  carregaTriangle2();
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
  transLoc = glGetUniformLocation (program->programId(), "matTG");

  glm::mat4 TG(1.0); //Matriu identidat
  TG = glm::translate (TG,glm::vec3(0.0,0.0,0.0));
  glUniformMatrix4fv(transLoc,1,GL_FALSE,&TG[0][0]);
}


void MyGLWidget::carregaTriangle()
{
  //Creamos casa utilizando triangles-strip. Ziga-zaga
  glm::vec3 Vertices[4];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(0.5, -0.5, 0.0);
  Vertices[1] = glm::vec3(-0.5, -0.5, 0.0);
  Vertices[2] = glm::vec3(0.5, 0.5, 0.0);
  Vertices[3] = glm::vec3(-0.5, 0.5, 0.0);

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



}

void MyGLWidget::carregaTriangle2(){
  glm::vec3 Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(0.5, 0.5, 0.0);
  Vertices2[1] = glm::vec3(-0.5, 0.5, 0.0);
  Vertices2[2] = glm::vec3(0, 1, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


}

void MyGLWidget::pinta_triangle()
{
	// Activem l'Array a pintar
  glBindVertexArray(VAO);

  //Pintem l'escena en format GL_TRINAGLES_STRIP(casita)
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void MyGLWidget::pinta_triangle2()
{
  glBindVertexArray(VAO2);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::modelTransform(){

    glm::mat4 matTG(1.0); //Matriu identidat
    matTG = glm::translate (matTG,values);
    matTG = glm::rotate(matTG,degrees,z);
    std::cout << "1: " << degrees << std::endl;
    matTG = glm::scale(matTG,glm::vec3(scl,scl,scl));
    glUniformMatrix4fv(transLoc,1,GL_FALSE,&matTG[0][0]);
}

void MyGLWidget::modelTransform2(){

    glm::mat4 matTG(1.0); //Matriu identidat
    matTG = glm::translate (matTG,values);
    matTG = glm::rotate(matTG,-1.0f*degrees,z);
    std::cout << "2: " << -1.0f*degrees << std::endl;
    matTG = glm::scale(matTG,glm::vec3(scl,scl,scl));
    glUniformMatrix4fv(transLoc,1,GL_FALSE,&matTG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
    makeCurrent();
    switch(e->key()){
      case Qt::Key_R :
            degrees += (float)(M_PI/6);
            break;

        case Qt::Key_E :
            degrees -= (float)(M_PI/6);
            break;
        default: e->ignore();
    }
    update();
}
