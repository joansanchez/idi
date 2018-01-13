#include "MyGLWidget.h"

#include <iostream>


MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
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
  psi = 0.0f;
  theta = 0.0f;
  modelAPintar = 0;
  tipusCamera = 0;
  one_degree = (float)M_PI/180;
  valorzoom = 90;
  psigraus = 0;
  thetagraus = 0;
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glEnable(GL_DEPTH_TEST);
  if (tipusCamera == 0)ini_camera();
  else ini_camera_axo();
}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform();

  if (modelAPintar==0)pinta_homer();
  else pinta_model2();
  //modelTransformTerra();
  //pinta_terra();

  glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  evitaDeformacions(w, h);
  glViewport(0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
    modelAPintar = 0;
    carregaModelHomer();
    calculcapsacontainer();
    calculcapsaminima();
    centrarmodel();

    modelAPintar = 1;
    carregaModel2();//TODO: calculcapsacontainer, calculcapsaminima, centrarmodel
    calculcapsacontainer();
    calculcapsaminima();
    centrarmodel();

    //carregaTerra();
    glBindVertexArray (0);
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


/*
        ***********************
        *                     *
        *    MIS FUNCIONES    *
        *                     *
        ***********************

*/

/*
 ************************************
 *                                  *
 *   FUNCIONES DE CARREGA DE VAOs   *
 *                                  *
 ************************************
*/

void MyGLWidget::carregaModelHomer()
{
    model1.load("../../models/Patricio.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    glGenBuffers(1, &VBO_PatricioPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_PatricioCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model1.faces().size()*3*3, model1.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

void MyGLWidget::carregaModel2()
{
    model2.load("../../models/legoman.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Model2);
    glBindVertexArray(VAO_Model2);

    glGenBuffers(1, &VBO_Model2Pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Model2Pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model2.faces().size()*3*3, model2.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_Model2Col);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Model2Col);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model2.faces().size()*3*3, model2.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

void MyGLWidget::carregaTerra(){
  glm::vec3 posicio[4] = {
    glm::vec3(1.0, -1.0, 1.),
    glm::vec3(-1.0, -1.0, 1.0),
    glm::vec3(1.0, -1.0, -1.0),
    glm::vec3(-1.0, -1.0, -1.0)
  };
  glm::vec3 Colors[3];  // Tres vèrtexs amb X, Y i Z
  Colors[0] = glm::vec3(1.0, 0.0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);

  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
}

/*
 ********************************
 *                              *
 *   FUNCIONES DE PINTAR VAOs   *
 *                              *
 ********************************
*/

void MyGLWidget::pinta_homer()
{
        // Activem el VAO per a pintar el homer
    glBindVertexArray (VAO_Patricio);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, model1.faces().size()*3);
}

void MyGLWidget::pinta_model2()
{
        // Activem el VAO per a pintar el homer
    glBindVertexArray (VAO_Model2);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, model2.faces().size()*3);
}

void MyGLWidget::pinta_terra()
{
        // Activem el VAO per a pintar el homer
    glBindVertexArray (VAO_Terra);

    // pintem
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
/*
 ***************************
 *                         *
 *   FUNCIONES DE VISIÓN   *
                           *
 ***************************
*/

void MyGLWidget::projectTransform(){
    //FOV, ra, znear, zfar
    std::cout<< "perspectiva" << std::endl;
    glm::mat4 Proj = glm::perspective(FOV,ra,znear,zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::projectTransformaxo(){
  std::cout<< "axon" << std::endl;
  std::cout<< "left: " << left<< " right: " <<right << " bottom: " << bottom <<" top: " <<top<< " znear: " <<znear << " zfar: " <<zfar<<std::endl;
  glm::mat4 Proj = glm::ortho(left, right, bottom, top, znear, zfar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform(){
    //glm::mat4 ViewMat = glm::lookAt(OBS,VRP,UP);
    glm::mat4 euler (1.0f);
    euler = glm::translate(euler, glm::vec3(0.f, 0.f, -distancia));
    euler = glm::rotate(euler,theta,glm::vec3(1.0,0.0,0.0));
    euler = glm::rotate(euler,psi,glm::vec3(0.0,1.0,0.0));
    euler = glm::translate(euler, -VRP);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&euler[0][0]);
}


void MyGLWidget::ini_camera(){
    radi = ((modelAPintar==0)?radi1:radi2);
    distancia = 2*radi;
    FOV = FOVini = (float)atan(radi/distancia)*2;

    ra = 1.0f;
    znear = distancia-radi;
    zfar = distancia+radi;
    projectTransform();

    //OBS = glm::vec3(0,0,3);
    VRP = glm::vec3(0,0,0);
    OBS = VRP;
    OBS[2] += distancia;
    UP = glm::vec3(0,1,0);
    viewTransform();
}
void MyGLWidget::ini_camera_sense_FOVupdate(){
    radi = ((modelAPintar==0)?radi1:radi2);
    distancia = 2*radi;

    ra = 1.0f;
    znear = distancia-radi;
    zfar = distancia+radi;
    projectTransform();

    //OBS = glm::vec3(0,0,3);
    VRP = glm::vec3(0,0,0);
    OBS = VRP;
    OBS[2] += distancia;
    UP = glm::vec3(0,1,0);
    viewTransform();
}

void MyGLWidget::ini_camera_axo(){
    radi = ((modelAPintar==0)?radi1:radi2);
    distancia = 2*radi;
    FOV = FOVini = (float)atan(radi/distancia)*2;

    ra = 1.0f;
    znear = distancia-radi;
    zfar = distancia+radi;
    left = bottom = -radi;
    right = top = radi;
    projectTransformaxo();

    //OBS = glm::vec3(0,0,3);
    VRP = glm::vec3(0,0,0);
    OBS = VRP;
    OBS[2] += distancia;
    UP = glm::vec3(0,1,0);
    viewTransform();
}


void MyGLWidget::calculcapsacontainer(){
  m = ((modelAPintar==0)?model1:model2);
  vectorminim[0] = vectormax[0] = (float)m.vertices()[0];
  vectorminim[1] = vectormax[1] = (float)m.vertices()[1];
  vectorminim[2] = vectormax[2] = (float)m.vertices()[2];

  for (unsigned int i = 0; i < m.vertices().size(); i += 3){
    //la x
    if ((float)m.vertices()[i] > vectormax[0]) vectormax[0] = (float)m.vertices()[i];
    else if ((float)m.vertices()[i] < vectorminim[0]) vectorminim[0] = (float)m.vertices()[i];

    //la y
    if ((float)m.vertices()[i+1] > vectormax[1]) vectormax[1] = (float)m.vertices()[i+1];
    else if ((float)m.vertices()[i+1] < vectorminim[1]) vectorminim[1] = (float)m.vertices()[i+1];

    //la z
    if ((float)m.vertices()[i+2] > vectormax[2]) vectormax[2] = (float)m.vertices()[i+2];
    else if ((float)m.vertices()[i+2] < vectorminim[2])vectorminim[2] = (float)m.vertices()[i+2];
  }
  if (modelAPintar == 0){
    vectorminim1 = vectorminim;
    vectormax1 = vectormax;
  }
  else {
    vectorminim2 = vectorminim;
    vectormax2 = vectormax;
  }
}

void MyGLWidget::calculcapsaminima(){
  centrecaixa[0] = (vectormax[0]+vectorminim[0])/2;
  centrecaixa[1] = (vectormax[1]+vectorminim[1])/2;
  centrecaixa[2] = (vectorminim[2]+vectormax[2])/2;
  float xaux = pow((vectormax[0] - vectorminim[0]),2);
  float yaux = pow((vectormax[1] - vectorminim[1]),2);
  float zaux = pow((vectormax[2] - vectorminim[2]),2);
  float sumaux = xaux + yaux + zaux;
  radi = sqrt(sumaux);
  if (modelAPintar == 0){
    radi1 = radi;
    centrecaixa1 = centrecaixa;
  }
  else {
    radi2 = radi;
    centrecaixa2 = centrecaixa;
  }

  //radi = dist((vectorminim[0], vectorminim[1], vectorminim[2]),(vectormax[0], vectormax[1], vectormax[2]))/2;
}

void MyGLWidget::centrarmodel(){
  glm::mat4 matTG(1.0); //Matriu identidat
  glm::vec3 values = ((modelAPintar==0)?centrecaixa1:centrecaixa2);
  values*= -1.0f;
  matTG = glm::translate (matTG,values);
  glUniformMatrix4fv(transLoc,1,GL_FALSE,&matTG[0][0]);
}

/*
 ***********************************
 *                                 *
 *   FUNCIONES DE TRANSFORMACIÓN   *
 *                                 *
 ***********************************
*/

void MyGLWidget::modelTransform ()
{
  centrecaixa =((modelAPintar==0)?centrecaixa1:centrecaixa2);
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform,angle,glm::vec3(0.0,1.0,0.0));
  transform = glm::translate (transform,-centrecaixa);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::evitaDeformacions(int w, int h){
  ra = float(w)/float(h);
  if (ra < 1.0f) FOV = 2 * atan(tan(FOVini/2)/ra);
  projectTransform();
}

/*
 ********************************
 *                              *
 *   FUNCIONES DE INTERACCIÓN   *
 *                              *
 ********************************
*/
void MyGLWidget::mousePressEvent(QMouseEvent *e)
{

    position = e->pos();

}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    if(e->buttons() & Qt::LeftButton && ! ( e->modifiers() & ( Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier ))) {
        if(e->x() > position.x())
        {
          if (psigraus == 360) psigraus = 0;
          ++psigraus;
          psi = (float)psigraus*one_degree;
          viewTransform();

        }
        else{
          if (psigraus == 0) psigraus = 359;
          else --psigraus;
          psi = (float)psigraus*one_degree;
          viewTransform();

        }
        if(e->y() > position.y()){
          if (thetagraus == 360) thetagraus = 0;
          else ++thetagraus;
          theta = (float)thetagraus*one_degree;
          viewTransform();
        }
        else{
          if (thetagraus == 0) thetagraus = 359;
          else --thetagraus;
          theta = (float)thetagraus*one_degree;
          viewTransform();
        }

        std::cout << "thetagraus: " << thetagraus << std::endl;
        std::cout << "psigraus: " << psigraus << std::endl;
        emit canviartheta(thetagraus);
        emit canviarpsi(psigraus);
    }
    position = e->pos();
    update();
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
    case Qt::Key_R: { //rotacio 45 graus
      angle += (float)(M_PI/4);
      break;
    }
      case Qt::Key_Z:{ //Zoom-in
          FOV -= 0.1;
          projectTransform();
          break;
      }
      case Qt::Key_X:{ //Zoom-out
          FOV += 0.1;
          projectTransform();
          break;
      }
    default: event->ignore(); break;
  }
  update();
}


/******************************
Operacions Designer
*******************************/
void MyGLWidget::canviarzoom(int valor){
  makeCurrent();
  if (tipusCamera == 0){
    float FOVaux = ((180-valor)/180.0) * M_PI;

    FOV = FOVaux;
    projectTransform();
  }
  else {
    int valaux = valor/20;
    if (valor > valorzoom){
      std::cout << "faig més zoom: " <<valor << std::endl;
      left += valaux;
      bottom += valaux;
      right -= valaux;
      top -= valaux;
    }
    else {
      std::cout << "faig menys zoom: " <<valor << std::endl;
      left -= valaux;
      bottom -= valaux;
      right += valaux;
      top += valaux;
    }
    projectTransformaxo();
  }
  valorzoom = valor;
  std::cout << "valorzoom: " <<valorzoom << std::endl;
  update();
}

void MyGLWidget::canviarmodel(){
  makeCurrent();
  if (tipusCamera == 0){
    if (modelAPintar == 0) modelAPintar = 1;
    else modelAPintar = 0;
    ini_camera_sense_FOVupdate();
  }
  else {
    if (modelAPintar == 0) modelAPintar = 1;
    else modelAPintar = 0;
    ini_camera_axo();
  }
  update();
}

void MyGLWidget::canviaroptica(){
  makeCurrent();
  if (tipusCamera == 0){
    tipusCamera = 1;
    ini_camera_axo();
    canviarzoom (valorzoom);
  }
  else {
    tipusCamera = 0;
    projectTransform();
    canviarzoom (valorzoom);
  }
  update();
}

void MyGLWidget::modificartheta(int valortheta){
  makeCurrent();
  theta = valortheta * one_degree;
  thetagraus = valortheta;
  viewTransform();
  update();
}

void MyGLWidget::modificarpsi(int valorpsi){
  makeCurrent();
  psi  = valorpsi * one_degree;
  psigraus = valorpsi;
  viewTransform();
  update();
}