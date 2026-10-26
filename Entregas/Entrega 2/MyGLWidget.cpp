#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#include <cmath> // Per M_PI

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::MyGLWidget(QWidget *parent)
  : LL2GLWidget(parent),
    colorMulLoc(-1),
    psi(0.0f),
    theta(float(M_PI) / 4.0f),
    dist(1.0f),
    angleCotxeBlau(0.0f),
    angleCotxeVerd(0.0f),
    angleCotxeVermell(0.0f),
    primeraPersona(false),
    timer(this)
{
  connect(&timer, SIGNAL(timeout()), this, SLOT(animaCotxes()));
}

void MyGLWidget::carregaShaders()
{
  LL2GLWidget::carregaShaders();
  colorMulLoc = glGetUniformLocation(program->programId(), "colorMul");
}

void MyGLWidget::RoadTransform(glm::vec3 pos, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG, angle, glm::vec3(0.0f, 1.0f, 0.0f));
  TG = glm::scale(TG, glm::vec3(kEscalaCircuit * escalaModels[ROAD]));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(kEscalaCanonada * escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::CarTransform(float radi, float angle)
{
  glm::mat4 TG(1.0f);

  const glm::vec3 pos(radi * std::cos(angle), 0.0f, -radi * std::sin(angle));

  TG = glm::translate(TG, pos);

  // El model ja mira cap a -Z, nomes cal el gir del recorregut.
  TG = glm::rotate(TG, angle, glm::vec3(0.0f, 1.0f, 0.0f));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::translate(TG, -centreBaseModels[CAR]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::TerraTransform()
{
  glm::mat4 TG(1.0f);

  // Model va de 4 a -4 a X i Z per tant és 8x8, escalem el model per 2.5
  // per anar de 10 a -10 així tenim terra de 20x20, ja és al centra llavors no cal traslladar-lo
  TG = glm::scale(TG, glm::vec3(2.5f, 2.5f, 2.5f));

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL()
{
  program->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Circuit
  glUniform3f(colorMulLoc, 1.0f, 1.0f, 1.0f);
  glBindVertexArray(VAO_models[ROAD]);

  RoadTransform(glm::vec3( 5.0f, 0.01f,  5.0f), glm::radians( 270.0f));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);

  RoadTransform(glm::vec3( 5.0f, 0.01f, -5.0f), glm::radians( 0.0f));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);

  RoadTransform(glm::vec3(-5.0f, 0.01f, -5.0f), glm::radians(90.0f));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);
  
  RoadTransform(glm::vec3(-5.0f, 0.01f,  5.0f), glm::radians(180.0f));
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);

  //Canonada centre.
  glBindVertexArray(VAO_models[PIPE]);
  PipeTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size() * 3);

  // Cotxe
  glBindVertexArray(VAO_models[CAR]);

  glUniform3f(colorMulLoc, 0.0f, 0.0f, 1.0f); // blau
  CarTransform(kRadiBlau, angleCotxeBlau);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size() * 3);

  glUniform3f(colorMulLoc, 0.0f, 1.0f, 0.0f); // verd
  CarTransform(kRadiVerd, angleCotxeVerd);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size() * 3);

  glUniform3f(colorMulLoc, 1.0f, 0.0f, 0.0f); // vermell
  CarTransform(kRadiVermell, angleCotxeVermell);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size() * 3);

  // Terra
  glUniform3f(colorMulLoc, 1.0f, 1.0f, 1.0f);
  glBindVertexArray(VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray(0);
}

void MyGLWidget::iniEscena()
{
  const glm::vec3 pMin(-kRadiExteriorCircuit, 0.0f, -kRadiExteriorCircuit);
  const glm::vec3 pMax( kRadiExteriorCircuit, 3.0f,  kRadiExteriorCircuit);

  centreEscena = (pMin + pMax) * 0.5f;
  radiEscena = glm::length(pMax - centreEscena);

  angleCotxeBlau = 0.0f;
  angleCotxeVerd = 0.0f;
  angleCotxeVermell = 0.0f;
}

void MyGLWidget::iniCamera()
{
  primeraPersona = false;

  psi = 0.0f;
  theta = glm::radians(45.0f);

  fov = glm::radians(45.0f);

  // sin(FOV/2) = R / dist  =>  dist = R / sin(FOV/2)
  dist = radiEscena / std::sin(fov * 0.5f);

  znear = dist - radiEscena;
  zfar  = dist + radiEscena;

  viewTransform();
  projectTransform();
}

void MyGLWidget::viewTransform()
{
  glm::mat4 view(1.0f);
  if (!primeraPersona)
  {
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -dist));
    view = glm::rotate(view, theta, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, -psi, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -centreEscena);
  }
  else
  {
    const float a = angleCotxeVerd;
    const glm::vec3 centreCotxe(kRadiVerd * std::cos(a), 0.0f, -kRadiVerd * std::sin(a));

    const glm::vec3 dir(-std::sin(a),0.0f, -std::cos(a));

    obs = centreCotxe - kAvancCameraCotxe * dir + glm::vec3(0.0f, kAlturaCameraCotxe, 0.0f);

    vrp = obs + dir;
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(obs, vrp, up);
  }

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::projectTransform()
{
  glm::mat4 Proj(1.0f);
  if (primeraPersona)
  {
    Proj = glm::perspective(glm::radians(60.0f), ra, 0.25f, zfar);
  }
  else
  {
    float fovResize = fov;
    if (ra < 1.0f)
    {
      fovResize = 2.0f * std::atan(std::tan(fov * 0.5f) / ra);
    }

    Proj = glm::perspective(fovResize, ra, znear, zfar);
  }

  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

  const int nouX = e->x();
  const int nouY = e->y();

  if (!primeraPersona && DoingInteractive == ROTATE)
  {
    const int deltaX = nouX - xClick;
    const int deltaY = nouY - yClick;

    // Mouse a la dreta => psi disminueix => camera cap a l'esquerra.
    psi -= float(deltaX) * factorAngleY;

    // En Qt pujar el ratoli dona deltaY negatiu,
    // volem camera en sentit contrari
    theta += float(deltaY) * factorAngleX;

    viewTransform();
  }

  xClick = nouX;
  yClick = nouY;

  update();
}

void MyGLWidget::avancaCotxes()
{
  angleCotxeBlau += glm::radians(kPasBlauGraus);
  angleCotxeVerd += glm::radians(kPasVerdGraus);
  angleCotxeVermell += glm::radians(kPasVermellGraus);

  // La camera del cotxe verd ha de moure's amb ell.
  if (primeraPersona)
    viewTransform();
}

void MyGLWidget::animaCotxes()
{
  makeCurrent();
  avancaCotxes();
  update();
}

void MyGLWidget::reiniciaEscenaICamera()
{
  timer.stop();

  iniEscena();
  iniCamera();
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      avancaCotxes();
      break;
    }
    case Qt::Key_C: { 
      primeraPersona = !primeraPersona;
      viewTransform();
      projectTransform();
      break;
        }
    case Qt::Key_R: { 
      reiniciaEscenaICamera();
      break;
        }  
    case Qt::Key_T: { 
      if (timer.isActive())
        timer.stop();
      else
        timer.start(1);
      break;
        }          
    default: event->ignore(); break;
  }
  update();
}