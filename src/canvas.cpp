/******************************************
*** Definition of class clCanvas
***
*** Body File
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <iostream>
#include <math.h>
#include <vector>
#include <wx/wx.h>

//#include "mywx.h"
#include "canvas.h"
#include "exceptiontree.h"
#include "list.h"

/*
** Definition of the event table
*/
BEGIN_EVENT_TABLE(clCanvas, wxGLCanvas)
  EVT_SIZE(clCanvas::OnSize)
  EVT_PAINT(clCanvas::OnPaint)
  EVT_CHAR(clCanvas::OnChar)
  EVT_LEFT_DOWN(clCanvas::OnMouseLeftEvent)
  EVT_RIGHT_DOWN(clCanvas::OnMouseRightEvent)
  EVT_MOUSEWHEEL(clCanvas::OnMouseWheelEvent)
  EVT_MOUSE_EVENTS(clCanvas::OnMouseEvent)
  EVT_ERASE_BACKGROUND(clCanvas::OnEraseEvent)
END_EVENT_TABLE()

/*
** Constructor and Destructor
*/
clCanvas::clCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, enDrawSetting dSet, GLenum dMode):
                   wxGLCanvas(parent, id, pos, size, style, name)
{
  try
  {
    // Set refrence to parentframe
    parentFrame = parent;

    // Intialise variables
    glInitialised = false;

    xMax = 0.0f; xMin = 0.0f;
    yMax = 0.0f; yMin = 0.0f;
    zMax = 0.0f; zMin = 0.0f;

    xRot = 0.0f;
    yRot = 0.0f;
    zRot = 0.0f;

    xTrans = 0.0f;
    yTrans = 0.0f;
    zTrans = 0.0f;

    scaleFactor = 1.0;

    drawSetting = dSet;
    drawMode    = dMode;

    vertexArray    = NULL;
    highLightArray = NULL;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clCanvas::clCanvas");
    throw(EX);
  }
}

clCanvas::~clCanvas(void)
{
  if(vertexArray)    delete vertexArray;
  if(highLightArray) delete highLightArray;
}

void clCanvas::SetMaxima(const GLfloat minX, const GLfloat maxX, const GLfloat minY, const GLfloat maxY, const GLfloat minZ, const GLfloat maxZ)
{
  xMin = minX;
  xMax = maxX;
  yMin = minY;
  yMax = maxY;
  zMin = minZ;
  zMax = maxZ;
}

void clCanvas::SetRotations(const GLfloat rotX, const GLfloat rotY, const GLfloat rotZ)
{
  xRot = rotX;
  yRot = rotY;
  zRot = rotZ;
}

void clCanvas::SetTranslations(const GLfloat transX, const GLfloat transY, const GLfloat transZ)
{
  xTrans = transX;
  yTrans = transY;
  zTrans = transZ;
}

void clCanvas::SetScaleFactor(const GLfloat factorScale)
{
  scaleFactor = factorScale;
}

void clCanvas::SetVertices(std::vector<GLfloat> *arrayVertex, std::vector<int> *arrayHighLight)
{
  if(vertexArray)    delete vertexArray;
  if(highLightArray) delete highLightArray;

  vertexArray    = arrayVertex;
  highLightArray = arrayHighLight;
}

/*
** Methods for drawing
*/
void clCanvas::InitGL(void)
{
    // white light
    static const GLfloat light0_pos[4]   = { -50.0f, 50.0f, 0.0f, 0.0f };
    static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

    // cold blue light
    static const GLfloat light1_pos[4]   = {  50.0f, 50.0f, 0.0f, 0.0f };
    static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };

    /* remove back faces */
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    /* speedups */
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

    /* light */
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_color);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void clCanvas::SetUpRC(void)
{
  try
  {
    // Find minimum values for the viewing volume
    GLfloat minView, maxView;
    minView = (xMin+xTrans) < (yMin+yTrans) ? (xMin+xTrans) : (yMin+yTrans);
    minView = minView < (zMin+zTrans) ?  minView : (zMin+zTrans);
    maxView = (xMax+xTrans) > (yMax+yTrans) ? (xMax+xTrans) : (yMax+yTrans);
    maxView = maxView > (zMax+zTrans) ?  maxView : (zMax+zTrans);

    minView = minView - VIEW_FACTOR*(maxView-minView);
    maxView = maxView + VIEW_FACTOR*(maxView-minView);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(minView, maxView, minView, maxView, maxView, minView);
    glOrtho(minView, maxView, minView, maxView, minView, maxView);

    // Set the model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(xTrans, yTrans, zTrans);

    // Set others
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);     // Set background to black
    glEnable(GL_DEPTH_TEST);                  // Enable depth test for performance
    glFrontFace(GL_CW);                       // Front face of polygons is defined counter clockwise
    glEnable(GL_CULL_FACE);                   // Only draw the front faces of the polygons
    glShadeModel(GL_FLAT);                    // Don't use complex shading
//    glShadeModel(GL_SMOOTH);                    // Don't use complex shading
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clCanvas::SetUpRC");
    throw(EX);
  }
}

void clCanvas::DrawIt(void)
{
  try
  {
    switch(drawSetting)
    {
      case(dsPoints):
        glPolygonMode(GL_FRONT, GL_POINT);
        glPolygonMode(GL_BACK, GL_POINT);
        break;
      case(dsWireFrame):
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        break;
      case(dsFilled):
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        break;
    }

    DrawContent();
    DrawAxes();

    // Restore the transformations
    glPopMatrix();

    // Flush the drawing commands
    glFlush();
    SwapBuffers();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clCanvas::DrawIt");
    throw(EX);
  }
}

void clCanvas::DrawContent(void)
{
  try
  {
    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Save matrix state and perform the rotation
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glTranslatef(xTrans, yTrans, zTrans);

    // Draw content
    glBegin(drawMode);
      // FIRST the vertices to be highlighted
      for(int h=0; h<highLightArray->size(); h++)
      {
        double red   = 1.0;
        double green = 0.0;
        double blue  = 0.0;

        int v = highLightArray->at(h);

        if(drawSetting == dsFilled)
        {
          if(red   > 0.1) red   = 1.0 - (v%5)/10.0;
          if(green > 0.1) green = 1.0 - (v%5)/10.0;
          if(blue  > 0.1) blue  = 1.0 - (v%5)/10.0;
        }

        glColor3f(red, green, blue);
        glVertex3f(vertexArray->at(v*3 + 0), vertexArray->at(v*3 + 1), vertexArray->at(v*3 + 2));
      }

      // Next all remaining vertices
      for(int v=0; v<vertexArray->size(); v=v+3)
      {
        double red   = 0.0;
        double green = 0.0;
        double blue  = 1.0;

        if(drawSetting == dsFilled)
        {
          if(red   > 0.1) red   = 1.0 - (v%5)/10.0;
          if(green > 0.1) green = 1.0 - (v%5)/10.0;
          if(blue  > 0.1) blue  = 1.0 - (v%5)/10.0;
        }

        glColor3f(red, green, blue);
        glVertex3f(vertexArray->at(v + 0), vertexArray->at(v + 1), vertexArray->at(v + 2));
      }
    glEnd();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clCanvas::DrawContent");
    throw(EX);
  }
}

void clCanvas::DrawAxes(void)
{
  float maxX = (1+VIEW_FACTOR)*xMax;
  float maxY = (1+VIEW_FACTOR)*yMax;
  float maxZ = (1+VIEW_FACTOR)*zMax;

  float maxL = (maxX + maxY + maxZ)/3.0f;
  float arrowLength = ARROW_SIZE*maxL;

  // Draw the lines
  glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(maxX, 0.0f, 0.0f);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, maxY, 0.0f);

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, maxZ);
  glEnd();

  // Draw the arrowheads
  glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex3f(maxX, 0.0f, 0.0f);
    glVertex3f(maxX-arrowLength, 0.0f, +arrowLength/2.0f);
    glVertex3f(maxX-arrowLength, 0.0f, -arrowLength/2.0f);

    glVertex3f(0.0f, maxY, 0.0f);
    glVertex3f(0.0f, maxY-arrowLength, +arrowLength/2.0f);
    glVertex3f(0.0f, maxY-arrowLength, -arrowLength/2.0f);

    glVertex3f(0.0f, 0.0f, maxZ);
    glVertex3f(+arrowLength/2.0f, 0.0f, maxZ-arrowLength);
    glVertex3f(-arrowLength/2.0f, 0.0f, maxZ-arrowLength);
  glEnd();
}

/*
** Methods for handling the events
*/
void clCanvas::OnPaint(wxPaintEvent& event)
{
  // OnPaint handlers must always create a wxPaintDC
  wxPaintDC dc(this);

  SetCurrent();

  if(!glInitialised)
  {
    //InitGL();
    SetUpRC();
    glInitialised = true;
  }

  DrawIt();
}

void clCanvas::OnSize(wxSizeEvent& event)
{
  int w, h;
  GetClientSize(&w, &h);
  glViewport(0, 0, (GLint)(w), (GLint)(h));

//  SetCurrent();
}

void clCanvas::OnChar(wxKeyEvent& event)
{
}

void clCanvas::OnMouseLeftEvent(wxMouseEvent& event)
{
  event.Skip();
}

void clCanvas::OnMouseRightEvent(wxMouseEvent& event)
{
  event.Skip();
}

void clCanvas::OnMouseWheelEvent(wxMouseEvent& event)
{
  if (event.GetWheelRotation() < 0)
  {
    scaleFactor /= 1.2f;
  }
  else
  {
    scaleFactor *= 1.2f;
  }

  Refresh(FALSE);
}

void clCanvas::OnMouseEvent(wxMouseEvent& event)
{
  static bool   dragging = false;
  static float  xOld;
  static float  yOld;

  if(event.LeftIsDown() && event.ControlDown())
  {
    // Pan
    if(!dragging)
    {
      dragging = true;
    }
    else
    {
      xTrans = +(event.GetX()-xOld);
      yTrans = +(event.GetY()-yOld);

      Refresh(false);
    }

    xOld = event.GetX();
    yOld = event.GetY();
  }
  else if(event.LeftIsDown() && event.ShiftDown())
  {
    // Zoom
    if(!dragging)
    {
      dragging = true;
    }
    else
    {
      float dx     = +(event.GetX()-xOld)/xOld;
      float dy     = -(event.GetY()-yOld)/yOld;
      float factor = (dx+dy)/fabs(dx+dy);

      if (factor < 0.0f) scaleFactor /= 1.2f;
      else               scaleFactor *= 1.2f;
      Refresh(false);
    }

    xOld = event.GetX();
    yOld = event.GetY();
  }
  else if(event.LeftIsDown() && !event.ControlDown() && !event.ShiftDown())
  {
    // Rotate
    if(!dragging)
    {
      dragging = true;
    }
    else
    {
      zRot += (xOld - event.GetX());
      xRot += (yOld - event.GetY());
      Refresh(false);
    }

    xOld = event.GetX();
    yOld = event.GetY();
  }
  else
  {
    dragging = false;
  }
}

void clCanvas::OnEraseEvent(wxEraseEvent& event)
{
  // Do nothing, avoid flashing.
}

void clCanvas::ReDraw(void)
{
  DrawIt();
}
