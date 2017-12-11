#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"


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
    //mouseMoveEvent - Es creida quan es mou el mouse
    virtual void mouseMoveEvent(QMouseEvent *e);
    //mousePressEvent - Es crida quan es clica el mouse
    virtual void mousePressEvent(QMouseEvent *e);


  private:

    //funcions de càrrega
    void createBuffers ();
    void carregaShaders ();

    //funcions per a carregar VAOs
    void carregaModelHomer();
    void carregaTerra();

    //funcions per a pintar VAOs
    void pinta_homer();
    void pinta_terra();

    //funcions de transformació
    void modelTransform ();
    void modelTransformTerra();
    void evitaDeformacions(int w, int h);

    //funcions de visió
    void projectTransform();
    void viewTransform();
    void ini_camera();
    void calculcapsacontainer();
    void calculcapsaminima();
    void centrarmodel();


    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;

    // uniform locations
    GLuint transLoc;
    GLuint projLoc;
    GLuint viewLoc;

    // VAO i VBO names
    GLuint VAO_Patricio, VBO_PatricioPos, VBO_PatricioCol;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;

    // Program
    QOpenGLShaderProgram *program;


    // Internal vars

    //Variables de transformació
    QPoint position;
    float scale;
    float angle;

    //Paràmetres inicials
    float FOVini;

    //Variables Project Matrix
    float FOV;
    float ra;
    float znear;
    float zfar;

    //Variables View Matrix
    glm::vec3 pos;
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;
    float distancia;
    float psi;
    float fi;
    float theta;

    //Variables de model
    Model m;

    //Caixa minima//
    glm::vec3 vectorminim; //posició mínima de la capsa contenidora
    glm::vec3 vectormax; //posició màxima de la capsa contenidora
    glm::vec3 centrecaixa; //posició central de la capsa contenidora

    //esfera contenidora//
    float radi;  //radi de la capsa contenidora

};
