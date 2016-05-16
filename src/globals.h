/**********************************************************
*** Header file for global definitions
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef GLOBALS_H
#define GLOBALS_H

//#define __DEBUG__

#define PRECISION 1E-7
#define CLIPRANGE 1E-4
#define PI        3.141592654f

#define _X_ 1
#define _Y_ 2
#define _Z_ 3

#define MINANGLE         30.0f
#define MAXPERCENTAREA   1.0f
#define MAXPERCENTVOLUME 5.0f

#define MESLEN 500

enum enHighLight
{
  hlElement,
  hlFace,
  hlNode,
  hlNone
};

enum enObjectType
{
  otNone,
  otElement,
  otElement2D,
  otElement3D,
  otTriangle,
  otRectangle,
  otTetrahedron,
  otHexahedron,
  otNode,
  otNode2D,
  otNode3D,
  otGrid,
  otUnstructured,
  otUnstructured2D,
  otUnstructured3D,
  otSurfaceGrid,
  otAc3dObject,
  otAc3dSurface
};

#endif
