#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    void carregaTriangle2();
    void pinta_triangle();
    void pinta_triangle2();
    void modelTransformacioTriangle();
    void modelTransforma2();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    //Uniform locations
    GLuint transLoc;

    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO, VBOCOLOR;
    GLuint VAO2, VBO2, VBOCOLOR2;

    //atributs càlcul matriu de translació
    glm::vec3 posicio;
    float graus;
    float escalarX;
    float escalarY;
    float escalaGeneral;
    float girConjunt;
};
