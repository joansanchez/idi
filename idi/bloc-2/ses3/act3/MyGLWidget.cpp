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
    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    carregaShaders();
    createBuffers();
    glEnable(GL_DEPTH_TEST);
    ini_camera();
}

void MyGLWidget::paintGL ()
{
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    modelTransform();
    pinta_homer();
    modelTransform2();
    pinta_homer();
    modelTransformTerra();
    pinta_terra();

    glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h)
{
    evitaDeformacions(w, h);
    glViewport(0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
    carregaModelHomer();
    carregaTerra();
    calculcapsacontainer();
    calculcapsaminima();
    calculesfera();
    centrarmodel();
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
    m.load("../../models/Patricio.obj");

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    glGenBuffers(1, &VBO_PatricioPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glGenBuffers(1, &VBO_PatricioCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PatricioCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

void MyGLWidget::carregaTerra(){
    glm::vec3 posicio[4] = {
            glm::vec3(2.0, 0.0, 2.0),
            glm::vec3(-2.0, 0.0, 2.0),
            glm::vec3(2.0, 0.0, -2.0),
            glm::vec3(-2.0, 0.0, -2.0)
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
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
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
    glm::mat4 Proj = glm::ortho(left, right,bottom,top,znear,zfar);
    //glm::mat4 Proj = glm::perspective(FOV,ra,znear,zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::viewTransform(){
    glm::mat4 ViewMat = glm::lookAt(OBS,VRP,UP);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&ViewMat[0][0]);
}

void MyGLWidget::ini_camera(){
    FOV = FOVini = (float)atan(radi/distancia)*2;

    ra = 1.0f;
    znear = distancia-radi;
    zfar = distancia+radi;
    left = bottom = -2.0;
    right = top = 2.0;
    projectTransform();

    //OBS = glm::vec3(0,0,3);
    VRP = glm::vec3(0,0,0);
    OBS = glm::vec3(0,3,distancia);
    //OBS = VRP;
    //OBS[2] += distancia;
    UP = glm::vec3(0,1,0);
    viewTransform();
}

void MyGLWidget::calculcapsacontainer(){ //container homer

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
    std::cout << "("<< vectormax[0] << ", " << vectormax[1] << ", " << vectormax[2] << ")" << std::endl;

}

void MyGLWidget::calculcapsaminima(){
    //Càlcul centre i centre de la base de la capsa contenidora per a posicionament de hommer
    centrecaixa[0] = basecentrecaixa[0] = (vectormax[0]+vectorminim[0])/2;
    centrecaixa[1] = (vectormax[1]+vectorminim[1])/2;
    centrecaixa[2] = basecentrecaixa[2] = (vectorminim[2]+vectormax[2])/2;
    basecentrecaixa[1] = vectorminim[1];
    //fi càlcul de la capsa
}

void MyGLWidget::calculesfera(){
    //donat que sabem on es posicionen tots els objectes de l'escena, ho podem fer a ma
    vectorminimesfera = glm::vec3(-2,0,-2);
    vectormaxesfera = glm::vec3(2,1,2);

    //R=dist((xmin,ymin,zmin),(xmax,ymax,zmax))/2
    float xaux = pow((vectormaxesfera[0] - vectorminimesfera[0]),2);
    float yaux = pow((vectormaxesfera[1] - vectorminimesfera[1]),2);
    float zaux = pow((vectormaxesfera[2] - vectorminimesfera[2]),2);
    float sumaux = xaux + yaux + zaux;
    radi = sqrt(sumaux);
    //fi càlcul radi

    distancia = radi * 2;
    std::cout << "radi: "<< radi << std::endl;
    std::cout << "centre caixa: ("<< centrecaixa[0]<<", " << centrecaixa[1]<<", "<<centrecaixa[2]<<")"<< std::endl;

    //radi = dist((vectorminim[0], vectorminim[1], vectorminim[2]),(vectormax[0], vectormax[1], vectormax[2]))/2;
}

void MyGLWidget::centrarmodel(){
    glm::mat4 matTG(1.0); //Matriu identidat
    glm::vec3 values = centrecaixa;
    values*= -1.0f;
    std::cout << "moviment: ("<<values[0]<<", "<< values[1]<< ", "<<values[2]<< ")"<< std::endl;
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
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    //transform = glm::rotate(transform,angle,glm::vec3(0.0,1.0,0.0));
    transform = glm::translate (transform,glm::vec3(1.0, 0.0, 1.0));
    transform = glm::scale(transform, glm::vec3(1/(vectormax[1]-vectorminim[1]))); //alçada 1
    transform = glm::translate (transform,-basecentrecaixa); //centrar el model a partir de la seva capsa mínma
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform2 ()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::translate (transform,glm::vec3(-1.0, 0.0, -1.0));
    transform = glm::rotate(transform,(float)M_PI,glm::vec3(0.0,1.0,0.0));
    transform = glm::scale(transform, glm::vec3(1/(vectormax[1]-vectorminim[1]))); //alçada 1
    transform = glm::translate (transform,-basecentrecaixa); //centrar el model a partir de la seva capsa mínma
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
/*
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
          psi += (float)M_PI/40;
          viewTransform();

        }
        else{
          psi -= (float)M_PI/40;
          viewTransform();

        }
        if(e->y() > position.y()){
          theta += (float)M_PI/40;
          viewTransform();
        }
        else{
          theta -= (float)M_PI/40;
          viewTransform();
        }
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
} */
