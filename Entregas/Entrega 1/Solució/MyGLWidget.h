#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef struct {
    GLuint vao;
    glm::vec3 centre;
    float angle;
    glm::vec3 pos;
} Part;

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

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void inicialitzaTGsTangram ();

    void pintaTangram ();
    void pintaTauler ();

    glm::vec3 calculCentre (glm::vec3 *Verts, int nverts);

    void creaBuffers (); 
    void creaBuffersTangram ();        
    void creaBuffersTauler ();        
    
    void carregaShaders();
    
    void modelTransformPart (glm::vec3 centre, glm::vec3 posicio, float gir);
    void modelTransformTauler ();
    
    // program
    QOpenGLShaderProgram *program;

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniforms
    GLuint TGLoc;

    // VAOs
    GLuint VAO_Part[7], VAO_Tauler;

    // viewport
    GLint ample, alt;    
    
    Part tangram[7];

    // Peça seleccionada
    int partSeleccionada = -1;

    bool ajuda = false;
};
