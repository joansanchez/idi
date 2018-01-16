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

  private:
    void carregaShaders ();
    void createBuffers ();
    void carregaModel1();

    //pintar
    void modelTransform ();
    void pintaModel1();

    //camara
    void ini_camera();
    void projectTransform();
    void viewTransform();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Model1, VBO_Model1Pos, VBO_Model1Col;
    // Program
    QOpenGLShaderProgram *program;


    //camera vars
    //perspective
    float FOV, ra, znear, zfar;
    //lookAt
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    //Model vars
    Model model1;

    // Internal model vars
    float scale;
    float angle;
};
