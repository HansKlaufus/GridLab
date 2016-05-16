/****************************************
*** Declaration of class clCanvas
***
*** Header File
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "array.h"
#include "macro.h"

enum enContentType {ctGeometry, ctSurfaceGrid, ctBoundaryGrid, ctFieldGrid};
enum enDrawSetting {dsPoints, dsWireFrame, dsFilled};
enum enDrawMode    {dmQuads, dmTriangles};

class clCanvas: public wxGLCanvas
{
private:
  bool            glInitialised;             // Is GL initialised
  enContentType   contentType;               // What must be draw
  enDrawSetting   drawSetting;               // How must it be drawn
  GLenum          drawMode;                  // GL_TRIANGLES, GL_QUADS, etc.

  wxWindow        *parentFrame;              // Pointer parent window

  std::vector<GLfloat> *vertexArray;             // Array containing the vertices
  std::vector<int>     *highLightArray;          // Array containing the highLights

  GLfloat         xMax, xMin;
  GLfloat         yMax, yMin;
  GLfloat         zMax, zMin;

  GLfloat         xRot;                      // Rotation angles
  GLfloat         yRot;
  GLfloat         zRot;

  GLfloat         xTrans;                    // Translations
  GLfloat         yTrans;
  GLfloat         zTrans;

  GLfloat         scaleFactor;               // Factor for scaling

  void InitGL(void);
  void SetUpRC(void);
  void DrawIt(void);
  void DrawContent(void);
  void DrawAxes(void);

  void OnPaint(wxPaintEvent&);
  void OnSize(wxSizeEvent&);
  void OnChar(wxKeyEvent&);
  void OnMouseEvent(wxMouseEvent&);
  void OnMouseLeftEvent(wxMouseEvent&);
  void OnMouseRightEvent(wxMouseEvent&);
  void OnMouseWheelEvent(wxMouseEvent&);
  void OnEraseEvent(wxEraseEvent&);

public:
  clCanvas(wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long, const wxString&, enDrawSetting, GLenum);
  ~clCanvas(void);

  void SetMaxima(const GLfloat, const GLfloat, const GLfloat, const GLfloat, const GLfloat, const GLfloat);
  void SetRotations(const GLfloat, const GLfloat, const GLfloat);
  void SetTranslations(const GLfloat, const GLfloat, const GLfloat);
  void SetScaleFactor(const GLfloat);
  void SetVertices(std::vector<GLfloat>*, std::vector<int>*);

  void ReDraw(void);

DECLARE_EVENT_TABLE()
};

#endif

