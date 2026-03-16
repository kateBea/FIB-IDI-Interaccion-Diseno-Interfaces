#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
  inicialitzaTGsTangram ();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  pintaTangram ();
  pintaTauler ();
      
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::inicialitzaTGsTangram () {
  for (int i=0; i<7; i++) {
    tangram[i].angle = 0.0;
    tangram[i].pos = glm::vec3(0.0);
  }
}	

void MyGLWidget::pintaTangram () {
  for (int i=0; i<7; i++) {
    glBindVertexArray (VAO_Part[i]);
    modelTransformPart (tangram[i].centre, tangram[i].pos, tangram[i].angle);
    // Les parts 3 i 5 són quadrilàters, la resta triangles
    if ((i == 2) || (i == 4))
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    else
      glDrawArrays(GL_TRIANGLES, 0, 3);        	
  }
}	

void MyGLWidget::pintaTauler () {
  glBindVertexArray (VAO_Tauler);
  modelTransformTauler ();
  glDrawArrays(GL_LINE_LOOP, 0, 4);
}	

void MyGLWidget::modelTransformTauler ()
{
  glm::mat4 TG (1.0f);
  glUniformMatrix4fv (TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPart (glm::vec3 centre, glm::vec3 posicio, float gir)
{
  glm::mat4 TG (1.0f);
  glUniformMatrix4fv (TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_1:
        break;
    case Qt::Key_2:
        break;
    case Qt::Key_3:
        break;
    case Qt::Key_4:
        break;
    case Qt::Key_5:
        break;
    case Qt::Key_6:
        break;
    case Qt::Key_7:
        break;
    case Qt::Key_Up:
    	break;
    case Qt::Key_Down: 
    	break;
    case Qt::Key_Right: 
    	break;
    case Qt::Key_Left: 
    	break;
    case Qt::Key_A: 
    	break;
    case Qt::Key_D: 
    	break;	
    case Qt::Key_H: 
    	break;
    case Qt::Key_R: 
    	break;	
    default: event->ignore(); break;
  }
  update();
}

glm::vec3 MyGLWidget::calculCentre (glm::vec3 *Verts, int nverts){
  return (glm::vec3(0.0));
}

void MyGLWidget::creaBuffers (){
  creaBuffersTauler ();
  creaBuffersTangram ();
}

void MyGLWidget::creaBuffersTauler () {
  // Construïm la geometria de la part 1
  glm::vec3 Verts[4];  // vèrtexs amb X, Y i Z
  Verts[0] = glm::vec3(-0.8, -0.6, 0);
  Verts[1] = glm::vec3( 0.8, -0.6, 0);
  Verts[2] = glm::vec3( 0.8, 0.6, 0);
  Verts[3] = glm::vec3( -0.8, 0.6, 0);
  
  glm::vec3 Col[3];
  for(int i=0; i<3; i++) {
      Col[i] = glm::vec3(0, 0, 0);  // negre
  }

  // Generem i activem el VAO del Tauler
  glGenVertexArrays(1, &VAO_Tauler);
  glBindVertexArray(VAO_Tauler);

  GLuint VBOs[2];
  glGenBuffers(2, VBOs);


  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col), Col, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTangram (){
  // Demanem idents per als 7 VAOs...
  glGenVertexArrays(7, VAO_Part);
  // Els assignem al tangram
  for (int i=0; i<7; i++)
    tangram[i].vao = VAO_Part[i];

  // ... i per als 14 VBOs
  GLuint VBOs[14];
  glGenBuffers(14, VBOs);

  // Construïm la geometria de la part 1
  glm::vec3 Verts_1[3];  // vèrtexs amb X, Y i Z
  Verts_1[0] = glm::vec3(-0.8, -0.6, 0);
  Verts_1[1] = glm::vec3( 0.0, 0.0, 0);
  Verts_1[2] = glm::vec3( -0.8, 0.6, 0);
  
  glm::vec3 Col_1[3];
  for(int i=0; i<3; i++) {
      Col_1[i] = glm::vec3(1, 0, 1);  // magenta
  }

  tangram[0].centre = calculCentre (Verts_1, 3);

  // Activem el VAO Part 1
  glBindVertexArray(VAO_Part[0]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_1), Verts_1, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_1), Col_1, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Construïm la geometria de la part 2
  glm::vec3 Verts_2[3];  // vèrtexs amb X, Y i Z
  Verts_2[0] = glm::vec3(-0.8, 0.6, 0);
  Verts_2[1] = glm::vec3( 0.0, 0.0, 0);
  Verts_2[2] = glm::vec3( 0.8, 0.6, 0);
  
  glm::vec3 Col_2[3];
  for(int i=0; i<3; i++) {
      Col_2[i] = glm::vec3(1, 0.5, 0);  // taronja
  }

  tangram[1].centre = calculCentre (Verts_2, 3);

  // Activem el VAO Part 2
  glBindVertexArray(VAO_Part[1]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_2), Verts_2, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_2), Col_2, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Construïm la geometria de la part 3
  glm::vec3 Verts_3[4];  // vèrtexs amb X, Y i Z
  Verts_3[0] = glm::vec3(-0.8, -0.6, 0);
  Verts_3[1] = glm::vec3( 0.0, -0.6, 0);
  Verts_3[2] = glm::vec3( -0.4, -0.3, 0);
  Verts_3[3] = glm::vec3( 0.4, -0.3, 0);
  
  glm::vec3 Col_3[4];
  for(int i=0; i<4; i++) {
      Col_3[i] = glm::vec3(0, 0, 1);  // blau
  }

  tangram[2].centre = calculCentre (Verts_3, 4);

  // Activem el VAO Part 3
  glBindVertexArray(VAO_Part[2]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_3), Verts_3, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_3), Col_3, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Construïm la geometria de la part 4
  glm::vec3 Verts_4[3];  // vèrtexs amb X, Y i Z
  Verts_4[0] = glm::vec3(-0.4, -0.3, 0);
  Verts_4[1] = glm::vec3( 0.4, -0.3, 0);
  Verts_4[2] = glm::vec3( 0.0, 0.0, 0);
  
  glm::vec3 Col_4[3];
  for(int i=0; i<3; i++) {
      Col_4[i] = glm::vec3(0, 1, 0);  // verd
  }

  tangram[3].centre = calculCentre (Verts_4, 3);

  // Activem el VAO Part 4
  glBindVertexArray(VAO_Part[3]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_4), Verts_4, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[7]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_4), Col_4, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Construïm la geometria de la part 5
  glm::vec3 Verts_5[4];  // vèrtexs amb X, Y i Z
  Verts_5[0] = glm::vec3( 0.0, 0.0, 0);
  Verts_5[1] = glm::vec3( 0.4, -0.3, 0);
  Verts_5[2] = glm::vec3( 0.4, 0.3, 0);
  Verts_5[3] = glm::vec3( 0.8, 0.0, 0);
  
  glm::vec3 Col_5[4];
  for(int i=0; i<4; i++) {
      Col_5[i] = glm::vec3(1, 0, 0);  // vermell
  }

  tangram[4].centre = calculCentre (Verts_5, 4);

  // Activem el VAO Part 5
  glBindVertexArray(VAO_Part[4]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[8]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_5), Verts_5, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[9]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_5), Col_5, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Construïm la geometria de la part 6
  glm::vec3 Verts_6[3];  // vèrtexs amb X, Y i Z
  Verts_6[0] = glm::vec3( 0.4, 0.3, 0);
  Verts_6[1] = glm::vec3( 0.8, 0.0, 0);
  Verts_6[2] = glm::vec3( 0.8, 0.6, 0);
  
  glm::vec3 Col_6[3];
  for(int i=0; i<3; i++) {
      Col_6[i] = glm::vec3(0, 1, 1);  // cian
  }

  tangram[5].centre = calculCentre (Verts_6, 3);

  // Activem el VAO Part 6
  glBindVertexArray(VAO_Part[5]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[10]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_6), Verts_6, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[11]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_6), Col_6, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Construïm la geometria de la part 7
  glm::vec3 Verts_7[3];  // vèrtexs amb X, Y i Z
  Verts_7[0] = glm::vec3( 0.0, -0.6, 0);
  Verts_7[1] = glm::vec3( 0.8, -0.6, 0);
  Verts_7[2] = glm::vec3( 0.8, 0.0, 0);
  
  glm::vec3 Col_7[3];
  for(int i=0; i<3; i++) {
      Col_7[i] = glm::vec3(1, 1, 0);  // groc
  }

  tangram[6].centre = calculCentre (Verts_7, 3);

  // Activem el VAO Part 7
  glBindVertexArray(VAO_Part[6]);

  // Buffer amb les posicions dels vèrtexs
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[12]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Verts_7), Verts_7, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[13]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Col_7), Col_7, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificadors per als atributs “vertex” i "color" del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 
}
