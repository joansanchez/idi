#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT
  public slots:
  void canviarCamera();
  void setred(int red);
  void setgreen(int green);
  void setblue(int blue);

  signals:
  void canviCameraPrograma();


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
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void ini_camera();
    void calculaRadi();
    void modelTransformTerra ();
    void modelTransformPatricio ();
    void modelTransformPatricio2();
    void pintaModel();
    void calculaCapsaModel ();
    void evitardeformacions(int w, int h);

    // VAO i VBO names
    GLuint VAO_Patr, VBO_PatrPos, VBO_PatrNorm, VBO_PatrMatamb, VBO_PatrMatdiff, VBO_PatrMatspec, VBO_PatrMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc, posFocus;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // model
    Model patr;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centrePatr;
    float escala;
    float escala2;


    // radi de l'escena
    float radiEsc;
    float distancia;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleX, angleY;
    bool perspectiva;

    //variables de càmera
    float FOV, FOVini, ra, znear, zfar;
    glm::vec3 OBS, VRP, UP;
    bool tipusCamera;

    //variables de translació
    float movimentX;

    //variables il·luminacio
    glm::vec3 matriuaux;

    //variables de materials
    glm::vec3 colorMaterial;
};
