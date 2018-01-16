#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  angleY = 0.0;
  angleX = 0.0;
  valorZoom = 4.0;
  perspectiva = true;
  DoingInteractive = NONE;
  radiEsc = sqrt(16);//TODO: modificar
  angleGir = 0.0;
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
  glEnable (GL_DEPTH_TEST);
  carregaShaders ();
  createBuffers ();
  ini_camera();

  //pasem matriu especular i diferencial de la vaca, color gris brillant
  glm::vec3 aux = glm::vec3(1,1,1);
  glUniform3fv(matspecvaca,1,&aux[0]);
  aux = glm::vec3(0.4);
  glUniform3fv(matdiffvaca,1,&aux[0]);
  glUniform1i(vacaLoc,0);
  glUniform1i(frangesLoc,0);
  franges = false;
  glUniform1i(focusLoc,1);
  focus = true;
}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 12);

  modelTransformVaca();
  pintaVaca();

  modelTransformPatricio ();
  pintaModel();

  //modelTransformPatricio2();
  //pintaModel();

  glBindVertexArray(0);
}

void MyGLWidget::pintaModel(){
  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
}
void MyGLWidget::pintaVaca(){
  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Vaca);

  // Pintem l'escena
  glUniform1i(vacaLoc,1);
  glDrawArrays(GL_TRIANGLES, 0, vaca.faces().size()*3);
  glUniform1i(vacaLoc,0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  evitardeformacions(w, h);
  glViewport(0, 0, w, h);
}

void MyGLWidget::evitardeformacions(int w, int h){
  ra = float(w)/float(h);
  if (ra < 1.0f) FOV = 2 * atan(tan(FOVini/2)/ra);
  projectTransform();
}

void MyGLWidget::createBuffers ()
{
  carregaPatricio();
  carregaVaca();
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[12] = {
	glm::vec3(-2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(2.0, 1.0, -2.0)
  };

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
	norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
  };

  // Definim el material del terra
  glm::vec3 amb(0.2,0,0.2);
  glm::vec3 diff(0.8,0,0.8);
  glm::vec3 spec(0,0,0);
  float shin = 100;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaPatricio(){
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  patr.load("./models/Patricio.obj");


  // Calculem la capsa contenidora del model
  calculaCapsaModel ();

  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_PatrPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_PatrNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_PatrMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_PatrMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_PatrMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_PatrMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3, patr.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
}

void MyGLWidget::carregaVaca(){
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  vaca.load("./models/cow.obj");


  // Calculem la capsa contenidora del model
  calculaCapsaModel2 ();

  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Vaca);
  glBindVertexArray(VAO_Vaca);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_VacaPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glGenBuffers(1, &VBO_VacaNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_VacaMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glGenBuffers(1, &VBO_VacaMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glGenBuffers(1, &VBO_VacaMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3*3, vaca.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glGenBuffers(1, &VBO_VacaMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_VacaMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vaca.faces().size()*3, vaca.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
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

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  matdiffvaca = glGetUniformLocation (program->programId(), "matdiffvaca");
  matspecvaca = glGetUniformLocation (program->programId(), "matspecvaca");
  vacaLoc = glGetUniformLocation (program->programId(), "vaca");
  frangesLoc = glGetUniformLocation (program->programId(), "frangesLoc");
  focusLoc = glGetUniformLocation (program->programId(), "focusLoc");
}

void MyGLWidget::modelTransformVaca ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(1,-1,0));
  TG = glm::translate(TG, glm::vec3(0,0.25,0));
  TG = glm::rotate(TG,angleGir, glm::vec3(0,1,0));
  TG = glm::rotate(TG,float(-90.0 / 180.0 * M_PI), glm::vec3(0,1,0));
  TG = glm::rotate(TG,float(-90.0 / 180.0 * M_PI), glm::vec3(1,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModel2, escalaModel2, escalaModel2));
  TG = glm::translate(TG, -centreVaca);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació

  TG = glm::translate(TG, glm::vec3(1.0, -0.5, 0.0));
  TG = glm::rotate(TG,angleGir, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(0.0, 0.125, 0.0)); //hem de translate l'alçada del patricio
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centrePatr);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformPatricio2 ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(0.0, 2.0, 0.0)); //hem de translate l'alçada del patricio
  TG = glm::rotate(TG, (float)M_PI, glm::vec3(0.0, 0.0, 1.0));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centrePatr);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::ini_camera(){
  FOV = FOVini = (float)M_PI/3.0; //esto se tendría que calcular como dicen las diapos pero no sale
  ra = 1.0f;
  znear = radiEsc/2.0;
  zfar = 3.0*radiEsc;
  projectTransform();

  OBS = glm::vec3 (-1, 1, -1);
  VRP = glm::vec3 (1,-0.375, 0.0);
  UP = glm::vec3(0,1,0);
  viewTransform();
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (perspectiva)
    Proj = glm::perspective(FOV, ra, znear, zfar);
  else
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, radiEsc, 3.0f*radiEsc);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View;  // Matriu de posició i orientació
  View = glm::lookAt(OBS, VRP, UP);


  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escala = 0.25/(maxy-miny);
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;
  centreBasePatr[0] = (minx+maxx)/2.0; centreBasePatr[1] = miny; centreBasePatr[2] = (minz+maxz)/2.0;
  //std::cout<< "escala: "<<escala<< ", dimensions: " <<(maxx-minx)*escala <<", "<<(maxy-miny)*escala<<", "<<(maxz-minz)*escala<<std::endl;

}

void MyGLWidget::calculaCapsaModel2 ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = vaca.vertices()[0];
  miny = maxy = vaca.vertices()[1];
  minz = maxz = vaca.vertices()[2];
  for (unsigned int i = 3; i < vaca.vertices().size(); i+=3)
  {
    if (vaca.vertices()[i+0] < minx)
      minx = vaca.vertices()[i+0];
    if (vaca.vertices()[i+0] > maxx)
      maxx = vaca.vertices()[i+0];
    if (vaca.vertices()[i+1] < miny)
      miny = vaca.vertices()[i+1];
    if (vaca.vertices()[i+1] > maxy)
      maxy = vaca.vertices()[i+1];
    if (vaca.vertices()[i+2] < minz)
      minz = vaca.vertices()[i+2];
    if (vaca.vertices()[i+2] > maxz)
      maxz = vaca.vertices()[i+2];
  }
  escalaModel2 = 0.5/(maxz-minz); //vigilar donat que no sabem com està posicionat el model inicial
  centreVaca[0] = (minx+maxx)/2.0; centreVaca[1] = (miny+maxy)/2.0; centreVaca[2] = (minz+maxz)/2.0;
  centreBaseVaca[0] = (minx+maxx)/2.0; centreBaseVaca[1] = miny; centreBaseVaca[2] = (minz+maxz)/2.0;
  //std::cout<< "escala model 2: "<<escalaModel2<< ", dimensions: " <<(maxx-minx)*escalaModel2 <<", "<<(maxy-miny)*escalaModel2<<", "<<(maxz-minz)*escalaModel2<<std::endl;

}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_O: { // canvia òptica entre perspectiva i axonomètrica
      perspectiva = !perspectiva;
      projectTransform ();
      break;
    }
    case Qt::Key_R: { //giren els dos models
      angleGir -= (float)M_PI/6.0;
      break;
    }
    case Qt::Key_X: { //giren els dos models
      if (franges){
        franges = false;
        glUniform1i(frangesLoc,0);
      }
      else {
        franges = true;
        glUniform1i(frangesLoc,1);
      }
      break;
    }
    case Qt::Key_L: {
      if (focus){ //cambiar a camara
        focus = false;
        glUniform1i(focusLoc,0);
        emit canviarACamera();
      }
      else { //cambiar a escena
        focus = true;
        glUniform1i(focusLoc,1);
        emit canviarAEscena();
      }
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  else if (e->button() & Qt::RightButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) {
        DoingInteractive = ZOOM;
      }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    if (e->x() > xClick){
      angleY -=  M_PI / 40.0;
      viewTransform ();
    }
    else {
      angleY +=  M_PI / 40.0;
      viewTransform ();
    }
    if (e->y() > yClick){
      angleX -=  M_PI / 40.0;
      viewTransform ();
    }
    else{
      angleX +=  M_PI / 40.0;
      viewTransform ();
    }
  }
  else if (DoingInteractive == ZOOM){
    if (e->x() > xClick){
      valorZoom += 0.01;
      FOV = (float)M_PI/valorZoom;
      projectTransform();
    }
    else {
      valorZoom-= 0.01;
      FOV = (float)M_PI/valorZoom;
      projectTransform();
    }
    if (e->y() > yClick){
      valorZoom-= 0.01;
      FOV = (float)M_PI/valorZoom;
      projectTransform();
    }
    else if (e->y() <= yClick){
      valorZoom += 0.01;
      FOV = (float)M_PI/valorZoom;
      projectTransform();
    }
    int valorAEnviar = int(150*0.261799/FOV);
    std::cout<<"valor a enviar " << valorAEnviar <<std::endl;
    emit zoomcanviat(valorAEnviar);
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::canviarzoom(int valor){
  makeCurrent();

  valorZoom = float(valor*8.0/100);
  FOV = (float)M_PI/valorZoom;
  //std::cout<<"entra " << FOV<<std::endl;
  projectTransform();
  update();
}

void MyGLWidget::focusescena(){
  makeCurrent();
  focus = true;
  glUniform1i(focusLoc,1);
  update();
}

void MyGLWidget::focuscamera(){
  makeCurrent();
  focus = false;
  glUniform1i(focusLoc,0);
  update();
}
