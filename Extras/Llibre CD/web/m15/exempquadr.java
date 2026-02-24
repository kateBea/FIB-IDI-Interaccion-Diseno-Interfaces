/**
 * exempquadr.java
 *
 * Exemple d'un quadrat que escolta events de teclat i de mouse.
 * Els events de teclat s'usen per a apropar i allunyar l'observador
 * de l'escena, aixi com per sortir del programa.
 * Els events de mouse s'usen per a moure l'observador al voltant de
 * l'eix y de l'escena.
 * La classe dibuix deriva de GLCanvas i crea un nou panel per a
 * la imatge.
 *
 */

import gl4java.swing.GLJPanel;
import gl4java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.applet.*;

/////////////////////////////////////////////////////////////////
public class exempquadr extends JApplet {

  JPanel paneldib;
  dibuix dib;

//---------------------------------------------------------------
public void init() {
  iniDibuix();
  getContentPane().add(dib);

  getContentPane().repaint();
}

//---------------------------------------------------------------
public void iniDibuix () {
  dib = new dibuix(300,300);
  paneldib = new JPanel();
  paneldib.add(dib);
}

///// DIBUIX
/////////////////////////////////////////////////////////////////
class dibuix extends GLJPanel
implements KeyListener, MouseMotionListener, MouseListener
{
  float r=10;        // distancia de l'origen a l'observador
  int posX=0;
  int angAlfa=0;

//---------------------------------------------------------------
public dibuix (int width, int height ) {
    /* Inicialitza GLCanvas amb width x hight pixels */
  super();

  setPreferredSize (new Dimension(width, height));

  setVisible (true);
}

//---------------------------------------------------------------
public void init() {
    /* Activa els events a escoltar */
  addKeyListener(this);
  addMouseMotionListener(this);
  addMouseListener(this);

    /* Inicialitzacio de GL4Java. */
  if (glj.gljMakeCurrent(true) == false)
  {
    System.out.println("-- Problemes amb gljMakeCurent --");
    return;
  }

  gl.glClearColor (0,0,0,1);                // color d'esborrat, negre
  gl.glEnable (GL_DEPTH_TEST);              // activa el Z-buffering

    /* definir el viewport */
  gl.glViewport (0, 0, 300, 300);
    /* definir la projeccio en perspectiva */
  gl.glMatrixMode (GL_PROJECTION);
  gl.glLoadIdentity ();
  glu.gluPerspective (90.0, 1.0, 0.5, 1000.0);
}

//---------------------------------------------------------------
public void display()
{
  Integer IangAlfa = new Integer(angAlfa);
  double rad = IangAlfa.doubleValue()*(2.0*java.lang.Math.PI)/360.0;

    /* esborrar el buffer de color i el Z-buffer */
  gl.glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* definir la posicio de l'observador */
  gl.glMatrixMode (GL_MODELVIEW);
  gl.glLoadIdentity ();
  glu.gluLookAt (r*java.lang.Math.sin(rad), 0., r*java.lang.Math.cos(rad),
	         0., 0., 0., 0., 1., 0.);
  gl.glColor3f (1, 1, 0);            // color groc
    /* enviar un quadrat a pintar */
  gl.glBegin (GL_POLYGON);       // poligon
  gl.glVertex3f (1, 1, 0);           // primer vertex
  gl.glVertex3f (-1, 1, 0);          // segon vertex
  gl.glVertex3f (-1, -1, 0);         // tercer vertex
  gl.glVertex3f (1, -1, 0);          // quart vertex
  gl.glEnd ();                   // fi del poligon
  gl.glFlush ();             // fi del frame

    /* intercanviar buffers (doble-buffering) */
  glj.gljSwap();
}

//---------------------------------------------------------------
// Methods required for the implementation of KeyListener
//---------------------------------------------------------------
public void keyPressed (KeyEvent evt) {
  switch (evt.getKeyChar())
  {
    case 'R': r--;   // la tecla 'R' decrementa distancia (zoom in)
              break;
    case 'r': r++;   // la tecla 'r' incrementa distancia (zoom out)
              break;
    case 'q': System.exit(0);  // sortir de l'aplicacio
  }
  repaint();
}

public void keyReleased (KeyEvent evt) {
}

public void keyTyped (KeyEvent evt) {
}

//---------------------------------------------------------------
// Methods required for the implementation of MouseListener
//---------------------------------------------------------------
public void mouseClicked (MouseEvent evt) {
}

public void mouseEntered (MouseEvent evt) {
requestFocus();
}

public void mouseExited (MouseEvent evt) {
}

public void mousePressed (MouseEvent evt) {
  posX=evt.getX();
}

public void mouseReleased (MouseEvent evt) {
}

public void mouseDragged(MouseEvent evt) {
  int difX=posX-evt.getX();
  angAlfa=angAlfa+difX;
  posX=evt.getX();

  repaint();
}

public void mouseMoved (MouseEvent evt) {
}

} // Fi CLASSE Dibuix

}// Fi Applet exempquadr

