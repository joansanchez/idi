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

//////////////////////////////////
//     Funciones básicas        //
//////////////////////////////////

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

  //modelTransformModel2();
  //pintaModel2();

  glBindVertexArray (0);
}


void MyGLWidget::resizeGL (int w, int h)
{
  evitardeformacions(w, h);
  glViewport(0, 0, w, h);
}

//////////////////////////////////
//     Carga de shaders         //
//////////////////////////////////

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

  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");


  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

//////////////////////////////////
//     Carga de buffers         //
//////////////////////////////////

void MyGLWidget::createBuffers ()
{
  carregaModel1();
  carregaModel2();

  glBindVertexArray (0);
}

void MyGLWidget::carregaModel1(){
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  model1.load("../../../../bloc-2/models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();

  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Model1);
  glBindVertexArray(VAO_Model1);

  // Creació dels buffers del model model1
  // Buffer de posicions
  glGenBuffers(1, &VBO_Model1Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Pos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  //buffers de color
  glGenBuffers(1, &VBO_Model1Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Col);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_Model1Norm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Norm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_Model1Matamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Matamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_Model1Matdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Matdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_Model1Matspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Matspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_Model1Matshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model1Matshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3, model1.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  calculaCapsaModel();
  //calculaRadi();
}

void MyGLWidget::carregaModel2(){
  //Enviem posició dels vèrtexs del terra
  glm::vec3 Vertices[4];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(1.0, -1.0, 1.0);
  Vertices[1] = glm::vec3(-1.0, -1.0, 1.0);
  Vertices[2] = glm::vec3(1.0, -1.0, -1.0);
  Vertices[3] = glm::vec3(-1.0, -1.0, -1.0);


  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO_Model2);
  glBindVertexArray(VAO_Model2);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO_Model2Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model2Pos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  //Enviem color dels vèrtexs
  glm::vec3 Colors[4];  // Colors dels tres vèrtexs X, Y i Z
  Colors[0] = glm::vec3(1.0, 1.0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 1.0);
  Colors[2] = glm::vec3(1.0, 0.0, 1.0);
  Colors[3] = glm::vec3(0.0, 0.0, 1.0);

  // Creació del buffer amb les dades dels colors dels vèrtexs
  glGenBuffers(1, &VBO_Model2Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model2Col);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
}

//////////////////////////////////
// Funciones de redimensionado  //
//////////////////////////////////

void MyGLWidget::evitardeformacions(int w, int h){
  ra = float(w)/float(h);
  if (ra < 1.0f) FOV = 2 * atan(tan(FOVini/2)/ra);
  projectTransform();
}

//////////////////////////////////
//     Funciones de pintado     //
//////////////////////////////////

void MyGLWidget::modelTransform ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  //transform = glm::rotate(transform, angle, glm::vec3(0.0, 1.0, 0.0));
  //transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, -centrePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::pintaModel1(){
  // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_Model1);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, model1.faces().size()*3);
}

void MyGLWidget::modelTransformModel2 ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::pintaModel2(){
  // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_Model2);

  // pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//////////////////////////////////
//     Funciones de camara      //
//////////////////////////////////

void MyGLWidget::ini_camera(){
  distancia = 2*radiEsc;
  FOV = FOVini = (float)asin(radiEsc/distancia)*2;
  ra = 1.0f;
  znear = radiEsc;
  zfar = 3.0*radiEsc;
  projectTransform();

  OBS = glm::vec3(0, 0, distancia);
  VRP = glm::vec3(0, 0, 0);
  UP = glm::vec3(0, 1, 0);
  psi = theta = 0.0f;
  viewTransform();
}

void MyGLWidget::projectTransform(){
  //perspective(FOV,ra,znear, zfar)
  glm::mat4 Proj = glm::perspective(FOV, ra, znear, zfar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(){
  glm::mat4 euler (1.0f);
  euler = glm::translate(euler, glm::vec3(0.f, 0.f, -distancia));
  euler = glm::rotate(euler,theta,glm::vec3(1.0,0.0,0.0));
  euler = glm::rotate(euler,-psi,glm::vec3(0.0,1.0,0.0));
  euler = glm::translate(euler, -VRP);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &euler[0][0]);
}

void MyGLWidget::calculaRadi(){
  float miny = std::min(0.0f, minymodel); //en aquest cas es poden calcular a ma, ni no sabem els valors calculaCapsaModel
  float maxy = std::max(0.0f, maxymodel);
  float minz = std::min(0.0f, minzmodel);
  float maxz = std::max(0.0f, maxzmodel);
  float minx = std::min(0.0f, minxmodel);
  float maxx = std::max(0.0f, maxxmodel);
  centre = glm::vec3((maxx+minx)/2.0,(maxy+miny)/2.0,(maxz+minz)/2.0);
  radiEsc = sqrt(pow(centre.x-maxx,2) + pow(centre.y-maxy,2) + pow(centre.z-maxz,2));
  cout << "radi: " <<radiEsc <<endl;
}


void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minxmodel, minymodel, minzmodel, maxxmodel, maxymodel, maxzmodel;
  minxmodel = maxxmodel = model1.vertices()[0];
  minymodel = maxymodel = model1.vertices()[1];
  minzmodel = maxzmodel = model1.vertices()[2];
  for (unsigned int i = 3; i < model1.vertices().size(); i+=3)
  {
    if (model1.vertices()[i+0] < minxmodel)
      minxmodel = model1.vertices()[i+0];
    if (model1.vertices()[i+0] > maxxmodel)
      maxxmodel = model1.vertices()[i+0];
    if (model1.vertices()[i+1] < minymodel)
      minymodel = model1.vertices()[i+1];
    if (model1.vertices()[i+1] > maxymodel)
      maxymodel = model1.vertices()[i+1];
    if (model1.vertices()[i+2] < minzmodel)
      minzmodel = model1.vertices()[i+2];
    if (model1.vertices()[i+2] > maxzmodel)
      maxzmodel = model1.vertices()[i+2];
  }
  //escala = 1.0/(maxymodel-minymodel);
  //escala2 = 2.5/(maxymodel-minymodel);
  centrePatr[0] = (minxmodel+maxxmodel)/2.0; centrePatr[1] = (minymodel+maxymodel)/2.0; centrePatr[2] = (minzmodel+maxzmodel)/2.0;
  radiEsc = sqrt(pow(centrePatr[0]-maxxmodel,2) + pow(centrePatr[1]-maxymodel,2) + pow(centrePatr[2]-maxzmodel,2));

  //std::cout<< "escala: "<<escala<< ", dimensions: " <<(maxxmodel-minxmodel)*escala <<", "<<(maxymodel-minymodel)*escala<<", "<<(maxzmodel-minzmodel)*escala<<std::endl;

}


//////////////////////////////////
//     Funciones de teclado     //
//////////////////////////////////

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
    case Qt::Key_R: { // escalar a més petit
      angle += (float)M_PI/4.0;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}
