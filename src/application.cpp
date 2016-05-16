/****************************************************************
*** Definition of class clApplication
***
*** Body File
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "ac3dobject.h"
#include "application.h"
#include "box.h"
#include "geometry.h"
#include "geometryreader.h"
#include "list.h"
#include "node3d.h"
#include "object2d.h"
#include "object3d.h"
#include "surface.h"
#include "surfacegrid.h"
#include "unstructured3d.h"

using namespace std;

// TODO: Read PLY files


clApplication::clApplication(void)
{
  try
  {
    object3d       = NULL;
    unstructured3d = NULL;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::clApplication");
    throw EX;
  }
}

clApplication::~clApplication(void)
{
  for(unsigned int g=0; g<geometryList.size(); g++) delete geometryList[g];
  for(unsigned int sg=0; sg<surfaceGridList.size(); sg++) delete surfaceGridList[sg];
  if(object3d)       delete object3d;
  if(unstructured3d) delete unstructured3d;
}

bool clApplication::Run(int argc, char* argv[])
{
	int ret = 0;
	bool createBoundingBox = false;

	try
	{
	  time_t t0 = time(NULL);

    // Get commandline arguments
    for(int i=1; i<argc; i++)
    {
      if (strcmp(argv[i], "-f") == 0)
      {
        /* Use different datafile */
        char filename[50];
        strcpy(filename, argv[++i]);

        if(strstr(filename, ".geo") || strstr(filename, ".GEO"))
        {
          /*
          ** Create geometry and add to list
          */
          printf("\nReading geometry file: %s...\n", filename);
          clGeometry       *geometry = new clGeometry(0, filename);
          clGeometryReader  geometryReader(filename, *geometry);

          geometryList.push_back(geometry);
        }
        else if(strstr(filename, ".ac") || strstr(filename, ".AC"))
        {
          printf("\nReading AC3D file: %s...\n", filename);
          clAc3dObject *ac3dObject = new clAc3dObject(0);
          ac3dObject->ReadAC3DFile(filename);

          object3d = new clObject3D(ac3dObject);

          delete ac3dObject;
          ac3dObject = NULL;
        }
        else if(strstr(filename, ".gts") || strstr(filename, ".GTS"))
        {
          printf("\nReading surfaceGrid file: %s...\n", filename);
          clSurfaceGrid    *surfaceGrid = new clSurfaceGrid(0, filename);
          surfaceGrid->ReadGTSGrid(filename);

          surfaceGridList.push_back(surfaceGrid);
        }
        else if(strstr(filename, ".o3d") || strstr(filename, ".O3D"))
        {
          printf("\nReading object file: %s...\n", filename);
          object3d = new clObject3D();
          object3d->ReadO3DFile(filename);

          string outputfilename = filename;
          outputfilename += ".vtk";
          object3d->WriteLegacyVTKFile(outputfilename);
        }
        else if(strstr(filename, ".stl") || strstr(filename, ".STL"))
        {
          printf("\nReading object file: %s...\n", filename);
          object3d = new clObject3D();
          object3d->ReadSTLFile(filename);

          string outputfilename = filename;
          outputfilename += ".vtk";
          object3d->WriteLegacyVTKFile(outputfilename);
        }
      }
      else if (strcmp(argv[i], "-b") == 0)
      {
        /*
        ** Automagically create bounding box
        */
        createBoundingBox = true;
      }
      else if (strcmp(argv[i], "-2d") == 0)
      {
        /*
        ** Test routine for 2d unstructured grid
        */
        ret = 1;
      }
      else if (strcmp(argv[i], "-3d") == 0)
      {
        /*
        ** Test routine for 3d unstructured grid
        */
        ret = 2;
      }
      else
      {
        printf("\nUnknown commandline option: '%s'\n", argv[i]);
        printf("Use : gridLab [-f FILENAME]\n");
        ret = -1;
      }
    }

    if(ret == 1)
    {
      TestUnstructured2D();
    }
    else if(ret == 2)
    {
      TestUnstructured3D();
    }
    else if(ret != -1)
    {
      bool success = true;

      if(createBoundingBox) success = success && CreateBoundingBox();
      if(!geometryList.empty()) success = success && Geometry_SurfaceGrid(25, 25);
      if(!surfaceGridList.empty()) success = success && SurfaceGrid_Unstructured3D();
      if(object3d) success = success && Object3D_Unstructured3D();
    }

    if(object3d)
    {
      string filename = object3d->GetName();
      printf("Creating object3D VTK file...\n");
      object3d->WriteLegacyVTKFile(filename + ".object3d.vtk");
    }

    if(!surfaceGridList.empty())
    {
      for(unsigned int sg=0; sg<surfaceGridList.size(); sg++)
      {
        string filename = surfaceGridList[sg]->GetName();
        printf("Creating unstructured2D VTK file...\n");
        surfaceGridList[sg]->WriteLegacyVTKGrid(filename + ".grid2d.vtk");
      }
    }

    if(unstructured3d)
    {
      string filename = unstructured3d->GetName();
      printf("Creating unstructured3D VTK file...\n");
      unstructured3d->DumpGrid("temp.csv");
      unstructured3d->WriteLegacyVTKGrid(filename + ".grid3d.vtk");
    }

    time_t t1 = time(NULL);
    printf("Time: %ld minutes\n", (t1-t0)/60);

    char i;
    cin >> i;
	}

  catch(clExceptionTree EX)
  {
    string mes = "";

    EX.AddMethodToTree("clApplication::Run");
    mes = EX.PrintError(mes);
    cerr << mes;

    char i;
    cin >> i;
  }

  return ret;
}

bool clApplication::TestUnstructured2D(void)
{
  bool success = true;

  clUnstructured2D *unstructured2d = NULL;

  try
  {
    cout << endl << "TEST ROUTINE FOR 2D DELAUNAY TRIANGULATION" << endl;

    string fileName = "./data/2D_Test_2.o2d";
    clObject2D *object2d = new clObject2D(1, fileName);
    object2d->ReadO2DFile(fileName);

    unstructured2d = new clUnstructured2D(-50.0f, 70.0f, -70.0f, 70.0f);

    int numberVertices = object2d->GetNumberVertices();
    for(int v=0; v<numberVertices; v++)
    {
      double x = object2d->X(v);
      double y = object2d->Y(v);
      clNode2D *newNode = new clNode2D(unstructured2d->GetNewNodeID(), x, y);
      printf("[%3d%%] Inserting NODE ID = %d (%f, %f)\n", 100*v/numberVertices, newNode->GetNodeID(), x, y);

      unstructured2d->InsertNode(newNode);
    }

    if(unstructured2d)
    {
      char grid2dFilename[MESLEN];

      printf("Refining grid...\n");
      RefineGrid(object2d, unstructured2d);

      printf("Deleting initial grid...\n");
      unstructured2d->DeleteInitialGrid();

      printf("Deleting elements from cavities...\n");
      RemoveCavityElements(object2d, unstructured2d);

      printf("Verifying unstructured2d grid...\n");
      unstructured2d->CheckGrid();

      printf("Creating 2D VTK file...\n");
      sprintf(grid2dFilename, "%s.%s.%s", "grid", "2d", "vtk");
      unstructured2d->WriteLegacyVTKGrid(grid2dFilename);

      if(unstructured2d) delete unstructured2d;
    }
  }

  catch(clExceptionTree EX)
  {
    if(unstructured2d) delete unstructured2d;

    EX.AddMethodToTree("clApplication::TestUnstructured2D");
    throw(EX);
  }

  return success;
}

bool clApplication::TestUnstructured3D(void)
{
  bool success = true;

  clUnstructured3D *unstructured3D = NULL;

  try
  {
    cout << endl << "TEST ROUTINE FOR 3D DELAUNAY TRIANGULATION" << endl;

    string fileName = "./data/3D_Test_2.o3d";
    clObject3D *object3d = new clObject3D(1, fileName);
    object3d->ReadO3DFile(fileName);

    unstructured3D = new clUnstructured3D(-150.0f, 150.0f, -150.0f, 150.0f, -150.0f, 150.0f);

    int numberVertices = object3d->GetNumberVertices();
    for(int v=0; v<numberVertices; v++)
    {
      double x = object3d->X(v);
      double y = object3d->Y(v);
      double z = object3d->Z(v);
      clNode3D *newNode = new clNode3D(unstructured3D->GetNewNodeID(), x, y, z);
      printf("[%3d%%] Inserting NODE ID = %d (%f, %f, %f)\n", 100*v/numberVertices, newNode->GetNodeID(), x, y, z);

      unstructured3D->InsertNode(newNode);
    }

    if(unstructured3D)
    {
      printf("Refining grid...\n");
      RefineGrid(object3d, unstructured3D);

      printf("Deleting initial grid...\n");
      unstructured3D->DeleteInitialGrid();

      printf("Deleting elements from cavities...\n");
      RemoveCavityElements(object3d, unstructured3D);

      printf("Verifying unstructured3d grid...\n");
      unstructured3D->CheckGrid();

      char grid3dFilename[MESLEN];
      printf("Creating 3D CSV file...\n");
      sprintf(grid3dFilename, "%s.%s.%s", "grid", "3d", "csv");
      unstructured3D->DumpGrid(grid3dFilename);

      printf("Creating 3D VTK file...\n");
      sprintf(grid3dFilename, "%s.%s.%s", "object", "3d", "vtk");
      object3d->WriteLegacyVTKFile(grid3dFilename);

      printf("Creating 3D VTK file...\n");
      sprintf(grid3dFilename, "%s.%s.%s", "grid", "3d", "vtk");
      unstructured3D->WriteLegacyVTKGrid(grid3dFilename);

      if(unstructured3D) delete unstructured3D;
    }
  }

  catch(clExceptionTree EX)
  {
    unstructured3D->DumpGrid("unstructured3d.csv");
    if(unstructured3D) delete unstructured3D;

    EX.AddMethodToTree("clApplication::TestUnstructured3D");
    throw(EX);
  }

  return success;
}

bool clApplication::CreateBoundingBox(void)
{
  bool success = true;

  try
  {
    /*
    ** Create a bounding box
    */
    // TODO: Works only for one geometry
    if(geometryList.size()>1)
    {
      throw clExceptionTree("main", "-", "boundingBox only functional for 1 geometry.");
    }
    else
    {
      clGeometry *geometry = *geometryList.begin();
      clGeometry *box      = new clGeometry(0, "boundingBox");
      clBoxCreator(*geometry, *box, btBox, laNone, hdNone);

      geometryList.push_back(box);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::TestUnstructured2D");
    throw(EX);
  }

  return success;
}

bool clApplication::Geometry_SurfaceGrid(int numNodesU, int numNodesV)
{
  bool success = true;

  try
  {
    /*
    ** Generate surfaceGrid for each geometry
    */
    for(unsigned int g=0; g<geometryList.size(); g++)
    {
      clGeometry *geometry = geometryList[g];
      cout << endl << "Creating surface grid for geometry " << geometry->GetName() << "..." << endl;

      clSurfaceGrid *surfaceGrid = new clSurfaceGrid(0, geometry->GetName());
      for(int s=0; s<geometry->GetNumberSurfaces(); s++)
      {
        printf("[%3d%%] Applying grid to surface %d...\n", 100*s/geometry->GetNumberSurfaces(), s);
        clSurface *surface = geometry->GetSurfacePointer(s);
        surfaceGrid->CreateUnstructured(surface, numNodesU, numNodesV, dtUniform);
      }
      printf("[100%%] Done...\n");
      surfaceGridList.push_back(surfaceGrid);
    }

    for(unsigned int sg=0; sg<surfaceGridList.size(); sg++)
    {
      clSurfaceGrid *surfaceGrid = surfaceGridList[sg];
      cout << "Checking surfaceGrid " << surfaceGrid->GetName() << "..." << endl;
      surfaceGrid->CheckGrid();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::Geometry_SurfaceGrid");
    throw(EX);
  }

  return success;
}

bool clApplication::SurfaceGrid_Unstructured3D(void)
{
  /*
  ** Generates an unstructured3D volumetric grid from surfaceGrids
  */
  bool success = true;

  try
  {
    // Create an unstructured3d grid
    clSurfaceGrid *surfaceGrid      = *surfaceGridList.begin();
    int            totalNumberNodes =  surfaceGrid->GetNumberNodes();

    double minX = surfaceGrid->GetMinX();
    double maxX = surfaceGrid->GetMaxX();
    double minY = surfaceGrid->GetMinY();
    double maxY = surfaceGrid->GetMaxY();
    double minZ = surfaceGrid->GetMinZ();
    double maxZ = surfaceGrid->GetMaxZ();

    for(unsigned int sg=1; sg<surfaceGridList.size(); sg++)
    {
      surfaceGrid       = surfaceGridList[sg];
      totalNumberNodes += surfaceGrid->GetNumberNodes();

      minX = ((surfaceGrid->GetMinX() < minX) ? surfaceGrid->GetMinX() : minX);
      maxX = ((surfaceGrid->GetMaxX() > maxX) ? surfaceGrid->GetMaxX() : maxX);
      minY = ((surfaceGrid->GetMinY() < minY) ? surfaceGrid->GetMinY() : minY);
      maxY = ((surfaceGrid->GetMaxY() > maxY) ? surfaceGrid->GetMaxY() : maxY);
      minZ = ((surfaceGrid->GetMinZ() < minZ) ? surfaceGrid->GetMinZ() : minZ);
      maxZ = ((surfaceGrid->GetMaxZ() > maxZ) ? surfaceGrid->GetMaxZ() : maxZ);
    }

    double deltaX = maxX - minX;
    double deltaY = maxY - minY;
    double deltaZ = maxZ - minZ;

    minX = minX - deltaX/2;
    maxX = maxX + deltaX/2;
    minY = minY - deltaY/2;
    maxY = maxY + deltaY/2;
    minZ = minZ - deltaZ/2;
    maxZ = maxZ + deltaZ/2;

    // Create initial grid
    unstructured3d = new clUnstructured3D(minX, maxX, minY, maxY, minZ, maxZ);

    printf("Inserting surfaceGrid nodes...\n");

    vector< vector<double> > vertexList;
    for (unsigned int sg=0; sg<surfaceGridList.size(); sg++)
    {
      surfaceGrid = surfaceGridList[sg];
      for(int n=0; n<surfaceGrid->GetNumberNodes(); n++)
      {
        clNode3D *node = surfaceGrid->GetNodePointer(n);
        if(!node->IsHaloNode())
        {
          double x = node->x();
          double y = node->y();
          double z = node->z();

          vector<double> vertex;
          vertex.push_back(x);
          vertex.push_back(y);
          vertex.push_back(z);
          vertexList.push_back(vertex);
        }
      }
    }

    if(unstructured3d)
    {
      printf("Generating grid...\n");
      success = success && this->InsertVertexList(vertexList, unstructured3d, true);

      printf("Deleting initial grid...\n");
      success = success && unstructured3d->DeleteInitialGrid();

      printf("Verifying unstructured3d grid...\n");
      success = success && unstructured3d->CheckGrid();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::SurfaceGrid_Unstructured3D");
    throw(EX);
  }

  return success;
}

bool clApplication::Object3D_Unstructured3D(void)
{
  /*
  ** Generates an unstructured3D volumetric grid from a 3D Object
  */

  bool success = true;

  try
  {
    double minX = object3d->GetMinX();
    double maxX = object3d->GetMaxX();
    double minY = object3d->GetMinY();
    double maxY = object3d->GetMaxY();
    double minZ = object3d->GetMinZ();
    double maxZ = object3d->GetMaxZ();

    double deltaX = maxX - minX;
    double deltaY = maxY - minY;
    double deltaZ = maxZ - minZ;

    minX = minX - deltaX/2;
    maxX = maxX + deltaX/2;
    minY = minY - deltaY/2;
    maxY = maxY + deltaY/2;
    minZ = minZ - deltaZ/2;
    maxZ = maxZ + deltaZ/2;

    unstructured3d = new clUnstructured3D(minX, maxX, minY, maxY, minZ, maxZ);
    if(unstructured3d)
    {
      unstructured3d->SetName(object3d->GetName());

      printf("Generating grid...\n");
      vector< vector<double> > vertexList = object3d->GetVertices();
      success = success && this->InsertVertexList(vertexList, unstructured3d, true);

      printf("Refining grid...\n");
      this->RefineGrid(object3d, unstructured3d);

      printf("Deleting initial grid...\n");
      success = success && unstructured3d->DeleteInitialGrid();

      printf("Deleting elements from cavities...\n");
//      success = success && RemoveCavityElements(object3d, unstructured3d);

      printf("Verifying unstructured3d grid...\n");
      success = success && unstructured3d->CheckGrid();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::Object3D_Unstructured3D");
    throw(EX);
  }

  return success;
}

bool clApplication::EnsureGeometryConformity(clUnstructured2D *unstructured2d)
{
  bool success = true;

  // TODO: EnsureGeometryConformity
  // Insert edges / faces
  try
  {

  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::EnsureGeometryConformity");
    throw(EX);
  }

  return success;
}

bool clApplication::EnsureGeometryConformity(clUnstructured3D *unstructured3d)
{
  bool success = true;

  // TODO: EnsureGeometryConformity
  // Insert edges / faces
  try
  {

  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::EnsureGeometryConformity");
    throw(EX);
  }

  return success;
}

bool clApplication::RemoveCavityElements(clObject2D *object2d, clUnstructured2D *unstructured2d)
{
  bool success = true;

  try
  {
    for(int e=0; e<unstructured2d->GetNumberElements(); e++)
    {
      clTriangle *element = unstructured2d->GetElementPointer(e);
      clNode2D    node    = element->GetCentroid();
      if(!object2d->PointInObject(node.x(), node.y()))
      {
        unstructured2d->DeleteElement(element->GetElementID());
        e--; // Reset e
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::RemoveCavityElements");
    throw(EX);
  }

  return success;
}

bool clApplication::RemoveCavityElements(clObject3D *object3d, clUnstructured3D *unstructured3d)
{
  bool success = true;

  try
  {
    for(int e=0; e<unstructured3d->GetNumberElements(); e++)
    {
      clTetrahedron *element = unstructured3d->GetElementPointer(e);
      clNode3D       node    = element->GetCentroid();

//      cout << endl << "Testing ELEMENT ID " << element->GetElementID() << endl;
      if(!object3d->PointInObject(node.x(), node.y(), node.z()))
      {
        cout << "Deleting ELEMENT ID " << element->GetElementID() << endl;
        unstructured3d->DeleteElement(element->GetElementID());
        e--; // Reset e
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::RemoveCavityElements");
    throw(EX);
  }

  return success;
}

bool clApplication::RefineGrid(clObject2D *object2d, clUnstructured2D *unstructured2d)
{
  bool success = true;

  /*
  ** Implements Ruppert's Algorithm
  */

  try
  {
    // Ensure boundary cells are marked
    unstructured2d->SetBoundaryCells();

    // Check if any from the current nodes encroaches an edge
    int n=0;
    while(n<unstructured2d->GetNumberNodes())
    {
      clNode2D *node = unstructured2d->GetNodePointer(n);
      if(!node->IsHaloNode())
      {
        double x = node->x();
        double y = node->y();

        // Check if node is encroaching the objec; return new x, y
        if(object2d->IsEncroachingObject(x, y, true))
        {
          clNode2D *newNode = new clNode2D(unstructured2d->GetNewNodeID(), x, y);
          printf("Inserting NODE ID = %d (%f, %f)\n", newNode->GetNodeID(), x, y);
          if(unstructured2d->InsertNode(newNode)>0)
          {
            // Restart from beginning
            n=-1; // TODO: This is rather drastic; is it necessary?
          }
        }
      }

      // Next node
      n++;
    }

    // Check if any element has angles < minimum angle
    int e=0;
    while(e<unstructured2d->GetNumberElements())
    {
      clTriangle *element = unstructured2d->GetElementPointer(e);

      if(!element->IsHaloCell() /*&& !element->IsBoundaryCell()*/)
      {
        if((element->GetMinimumAngle() < MINANGLE) || (element->GetArea() / unstructured2d->GetArea() * 100 > MAXPERCENTAREA))
        {
          // Insert the circumCentre as a new node
          clNode2D circumCentre = element->GetCircumCentre();
          double x = circumCentre.x();
          double y = circumCentre.y();

          if(object2d->PointInObject(x, y))
          {
            /*
            ** If the circumCentre encroaches an edge, then x, y are equal to the mid point of the edge,
            ** otherwise x, y are still referring to the circumCentre ot the triangle.
            */
            object2d->IsEncroachingObject(x, y, true);
            clNode2D *newNode = new clNode2D(unstructured2d->GetNewNodeID(), x, y);
            printf("Inserting NODE ID = %d (%f, %f)\n", newNode->GetNodeID(), x, y);
            unstructured2d->InsertNode(newNode);

            // Restart from beginning
            e = -1;
          }
        }
      }

      // Next element
      e++;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::RefineGrid");
    throw(EX);
  }

  return success;
}

bool clApplication::RefineGrid(clObject3D *object3d, clUnstructured3D *unstructured3d)
{
  bool success = true;

  /*
  ** Implements Ruppert's Algorithm
  */

  try
  {
    // Ensure boundary cells are marked
    unstructured3d->SetBoundaryCells();

    // Check if any from the current nodes encroaches an edge
    bool ready = false;
    while(!ready)
    {
      vector< vector<double> > vertexList;
      for(int n=0; n<unstructured3d->GetNumberNodes(); n++)
      {
        clNode3D *node = unstructured3d->GetNodePointer(n);
        if(!node->IsHaloNode())
        {
          double x = node->x();
          double y = node->y();
          double z = node->z();

          // Check if node is encroaching the objec; return new x, y, z
          if(object3d->IsEncroachingObject(x, y, z, true))
          {
            vector<double> vertex;
            vertex.push_back(x);
            vertex.push_back(y);
            vertex.push_back(z);
            vertexList.push_back(vertex);
          }
        }
      }

      if(vertexList.empty())
      {
        ready = true;
      }
      else
      {
        InsertVertexList(vertexList, unstructured3d, false);
      }
    }

    // Check if any element has angles < minimum angle
    ready = false;
    while(!ready)
    {
      vector< vector<double> > vertexList;
      for(int e=0; e<unstructured3d->GetNumberElements(); e++)
      {
        clTetrahedron *element = unstructured3d->GetElementPointer(e);

        if(!element->IsHaloCell() /*&& !element->IsBoundaryCell()*/)
        {
          cout << "ELEMENT PERCENTAGE: " << (100*element->GetVolume() / unstructured3d->GetVolume()) << endl;
          if(/*(element->GetMinimumAngle() < MINANGLE) ||*/ (element->GetVolume() / unstructured3d->GetVolume() * 100 > MAXPERCENTVOLUME))
          {
            // Insert the circumCentre as a new node
            clNode3D circumCentre = element->GetCircumCentre();
            double x = circumCentre.x();
            double y = circumCentre.y();
            double z = circumCentre.z();

            if(object3d->PointInObject(x, y, z))
            {
              /*
              ** If the circumCentre encroaches an edge, then x, y, z are equal to the mid point of the edge,
              ** otherwise x, y, z are still referring to the circumCentre ot the tetrahedron.
              */
              vector<double> vertex;
              vertex.push_back(x);
              vertex.push_back(y);
              vertex.push_back(z);
              vertexList.push_back(vertex);
            }
          }
        }
      }

      if(vertexList.empty())
      {
        ready = true;
      }
      else
      {
        InsertVertexList(vertexList, unstructured3d, false);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::RefineGrid");
    throw(EX);
  }

  return success;
}

bool clApplication::InsertNode(clObject2D *object2d, clUnstructured2D *unstructured2d)
{
  // TODO: Is this routine being used?
  bool success = true;

  try
  {
    // Create copy of vertices
    vector< vector<double> > vertexList  = object3d->GetVertices();
    unsigned int             oldListSize = vertexList.size();
    while(success && (vertexList.size()>0))
    {
      unsigned int newListSize = vertexList.size();
      int          progress    = 100*newListSize/oldListSize;

      vector<double> vertex = vertexList[0];
      vertexList.erase(vertexList.begin());

      double x = vertex[0];
      double y = vertex[1];

      clNode2D *newNode = new clNode2D(unstructured2d->GetNewNodeID(), x, y);
      int       returnValue = unstructured2d->InsertNode(newNode);
      if(returnValue < 0)
      {
        printf("[%3d%%] Error     NODE ID = %d (%f, %f)\n", progress, newNode->GetNodeID(), x, y);
        success = false;
      }
      else if(returnValue == 0)
      {
        printf("[%3d%%] Duplicate NODE ID = %d (%f, %f)\n", progress, newNode->GetNodeID(), x, y);
        success = true;
      }
      else if(returnValue == 1)
      {
        printf("[%3d%%] Delayed   NODE ID = %d (%f, %f)\n", progress, newNode->GetNodeID(), x, y);
        vertexList.push_back(vertex);
        success = true;
      }
      else if(returnValue == 2)
      {
        printf("[%3d%%] Success   NODE ID = %d (%f, %f)\n", progress, newNode->GetNodeID(), x, y);
        success = true;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::InsertNode");
    throw(EX);
  }

  return success;
}

bool clApplication::InsertVertexList(vector< vector<double> > &vertexList, clUnstructured3D *unstructured3d, bool skipVertices = false)
{
  bool success = true;

  try
  {
    // Create copy of vertices
    unsigned int numberDelayed = 0;
    unsigned int oldListSize   = vertexList.size();
    while(success && (vertexList.size()>0))
    {
      unsigned int newListSize = vertexList.size();
      int          progress    = 100*(oldListSize-newListSize)/oldListSize;

      vector<double> vertex = vertexList.front();
      vertexList.erase(vertexList.begin());

      double x = vertex[0];
      double y = vertex[1];
      double z = vertex[2];

      clNode3D *newNode = new clNode3D(unstructured3d->GetNewNodeID(), x, y, z);
      int       returnValue = unstructured3d->InsertNode(newNode);
      if(returnValue < 0)
      {
        printf("[%3d%%] Error     NODE ID = %d (%f, %f, %f)\n", progress, newNode->GetNodeID(), x, y, z);
        success = false;
      }
      else if(returnValue == 0)
      {
        printf("[%3d%%] Duplicate NODE ID = %d (%f, %f, %f)\n", progress, newNode->GetNodeID(), x, y, z);
        success = true;
        numberDelayed = 0;
      }
      else if(returnValue == 1)
      {
        printf("[%3d%%] Delayed   NODE ID = %d (%f, %f, %f)\n", progress, newNode->GetNodeID(), x, y, z);
        vertexList.push_back(vertex);

        numberDelayed++;
        if(numberDelayed >= newListSize)
        {
          if(skipVertices)
          {
            // Skips insertion of vertices that cause creation of degenerate elements
            while(vertexList.size()>0)
            {
              vertex   = vertexList.back();
              vertexList.erase(vertexList.end());

              double x = vertex[0];
              double y = vertex[1];
              double z = vertex[2];

              progress = 100;
              printf("[%3d%%] Skipped   NODE ID = %d (%f, %f, %f)\n", progress, newNode->GetNodeID(), x, y, z);
            }

            success = true;
          }
          else
          {
            // Throws exception in case vertices cause creation of degenerate elements
            success=false;
            char message[MESLEN];
            sprintf(message, "Running into endless loop; aborting (numberDelayed = %d)...", numberDelayed);
            throw clExceptionTree("clApplication", "InsertVertexList", message);
          }
        }
        else
        {
          success = true;
        }
      }
      else if(returnValue == 2)
      {
        printf("[%3d%%] Success   NODE ID = %d (%f, %f, %f)\n", progress, newNode->GetNodeID(), x, y, z);
        success = true;
        numberDelayed = 0;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clApplication::InsertVertexList");
    throw(EX);
  }

  return success;
}
