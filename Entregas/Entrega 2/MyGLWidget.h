#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
    virtual void keyPressEvent (QKeyEvent *event);

    void paintGL() override;
    void RoadTransform(glm::vec3 pos, float angle) override;
    void PipeTransform() override;
    void TerraTransform() override;
    void CarTransform(float radi, float angle) override;
    void carregaShaders() override;

    void iniEscena() override;
    void iniCamera() override;
    void viewTransform() override;
    void projectTransform() override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void animaCotxes();

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    void avancaCotxes();
    void reiniciaEscenaICamera();

private:

    // Uniform que multiplica el color original del model.
    GLint colorMulLoc;

    // Parametres camera
    float psi;
    float theta;
    float dist;

    float angleCotxeBlau;
    float angleCotxeVerd;
    float angleCotxeVermell;

    // Si està cert la camara està situada dins del cotxe verd
    bool primeraPersona;

    QTimer timer;

    // Constants
    inline static const float kRadiBlau = 6.0f;
    inline static const float kRadiVerd = 7.5f;
    inline static const float kRadiVermell = 9.0f;

    inline static const float kPasBlauGraus = 2.0f;
    inline static const float kPasVerdGraus = 3.0f;
    inline static const float kPasVermellGraus = 4.0f;

    inline static const float kRadiExteriorCircuit = 10.0f;
    inline static const float kEscalaCircuit = 5.0f;
    inline static const float kEscalaCanonada = 3.0f;

    inline static const float kAlturaCameraCotxe = 0.5f;
    inline static const float kAvancCameraCotxe = 0.5f;
};
