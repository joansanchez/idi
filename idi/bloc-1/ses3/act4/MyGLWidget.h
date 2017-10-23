#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>

#define GLM_FORCE_RADIANS

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();

    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);

    virtual void keyPressEvent(QKeyEvent *e);
  private:
    void createBuffers ();
    void carregaShaders ();
    void carregaTriangle();
    void pinta_triangle();
    void modelTransform();

    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    GLuint transLoc;

    float scl;
    float degrees;

    glm::vec3 values;
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO[2];
};