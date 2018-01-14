#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
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

    //captura les tecles del teclat
    virtual void keyPressEvent(QKeyEvent *e);

  private:
    void createBuffers ();
    void carregaShaders ();
    void carregaTriangle();
    void pinta_triangle();
    void modelTransformacioTriangle();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    //Uniform locations
    GLuint transLoc;

    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO, VBOCOLOR;

    //matrius
    glm::vec4 matriuTranslacio;
};
