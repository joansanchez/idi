TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ../../../../bloc-2/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp ../../../../bloc-2/Model/model.cpp
