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

  public slots:
    void canviarzoom(int valor);
    void canviarmodel();
    void canviaroptica();
    void modificartheta(int valortheta);
    void modificarpsi(int valorpsi);
    void canviarterra(int red, int green, int blue);

  signals:
    void canviartheta(int valorthetamod);
    void canviarpsi(int valorpsimod);

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
    void carregaModel2();
    void carregaTerra();

    //funcions per a pintar VAOs
    void pinta_homer();
    void pinta_model2();
    void pinta_terra();

    //funcions de transformació
    void modelTransform ();
    void modelTransformTerra();
    void evitaDeformacions(int w, int h);

    //funcions de visió
    void projectTransform();
    void projectTransformaxo();
    void viewTransform();
    void viewTransformAxo();
    void ini_camera();
    void ini_camera_axo();
    void ini_camera_sense_FOVupdate();
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
    GLuint VAO_Model2, VBO_Model2Pos, VBO_Model2Col;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;

    // Program
    QOpenGLShaderProgram *program;



    // Internal vars
    int modelAPintar; // permet triar entre dos models
    int tipusCamera;

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
    float left, right, top, bottom;
    int valorzoom;
    float one_degree;

    //Variables View Matrix
    glm::vec3 pos;
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;
    float distancia;
    float psi;
    float fi;
    float theta;
    int psigraus;
    int thetagraus;

    //Variables de model
    Model model1;
    Model model2;
    Model m; //model auxiliar


    //Caixa minima aux//
    glm::vec3 vectorminim; //posició mínima de la capsa contenidora
    glm::vec3 vectormax; //posició màxima de la capsa contenidora
    glm::vec3 centrecaixa; //posició central de la capsa contenidora

    //Caixa minima model1//
    glm::vec3 vectorminim1; //posició mínima de la capsa contenidora
    glm::vec3 vectormax1; //posició màxima de la capsa contenidora
    glm::vec3 centrecaixa1; //posició central de la capsa contenidora

    //Caixa minima model 2//
    glm::vec3 vectorminim2; //posició mínima de la capsa contenidora
    glm::vec3 vectormax2; //posició màxima de la capsa contenidora
    glm::vec3 centrecaixa2; //posició central de la capsa contenidora

    //esfera contenidora//
    float radi;  //radi de la capsa contenidora aux
    float radi1;
    float radi2;
};
