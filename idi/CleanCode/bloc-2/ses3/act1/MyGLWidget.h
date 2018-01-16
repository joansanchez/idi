#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <iostream>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"
using namespace std;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mousePressEvent - Es cridat quan es prem el ratolí
    virtual void mousePressEvent(QMouseEvent *e);
    // mouseMoveEvent - Es cridat quan es mou el ratolí
    virtual void mouseMoveEvent(QMouseEvent *e);

  private:
    //constructoras
    void carregaShaders ();
    void createBuffers ();
    void carregaModel1();
    void carregaModel2();

    //resize
    void evitardeformacions(int w, int h);

    //pintar
    void modelTransform ();
    void pintaModel1();
    void modelTransformModel2();
    void pintaModel2();

    //camara
    void ini_camera();
    void projectTransform();
    void viewTransform();
    void calculaRadi();
    void calculaCapsaModel();

    // attribute locations
    GLuint vertexLoc, colorLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Model1, VBO_Model1Pos, VBO_Model1Col, VBO_Model1Norm, VBO_Model1Matamb, VBO_Model1Matdiff, VBO_Model1Matspec, VBO_Model1Matshin;
    GLuint VAO_Model2, VBO_Model2Pos, VBO_Model2Col;
    // Program
    QOpenGLShaderProgram *program;
    QPoint position;

    //camera vars
    //perspective
    float FOV, FOVini, ra, znear, zfar;
    //lookAt
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;
    float radiEsc;
    float distancia;
    glm::vec3 centre;
    float psi;
    float theta;

    //Model vars
    Model model1;
    float minxmodel, minymodel, minzmodel, maxxmodel, maxymodel, maxzmodel;
    glm::vec3 centrePatr;

    // Internal model vars
    float scale;
    float angle;
};
