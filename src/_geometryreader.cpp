/*****************************************************
*** Body file for definition of class clGeometryReader
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <iostream>
#include <fstream>
#include <string.h>

#include "exception.h"
#include "geometry.h"
#include "geometryreader.h"
#include "matrix.h"
#include "patch.h"
#include "str.h"
#include "surface.h"

/*
**----------------
** Private methods
**----------------
*/

/*
** Decode methods
*/

int clGeometryReader::GetBlockSpec(const char *line) const
{
  int spec;

  if(strstr(line, "POINT"))
  {
    spec = POINT_SPEC;
  }
  else if(strstr(line, "PATCH"))
  {
    spec = PATCH_SPEC;
  }
  else if(strstr(line, "SURFACE"))
  {
    spec = SURFACE_SPEC;
  }
  else if(strstr(line, "GEOMETRY"))
  {
    spec = GEOMETRY_SPEC;
  }
  else if(strstr(line, "END"))
  {
    spec = END_SPEC;
  }
  else
  {
    spec = UNKNOWN_SPEC;
  }

  return (spec);
}

/*
** Read methods
*/

void clGeometryReader::ReadFile(clGeometry* geometry) const
{
  fstream file;

  // Open file for input
  file.open(fileName, ios::in);

  if(!file)
  {
    throw clExceptionTree("clGeometryReader", "ReadFile", "Could not open file.");
  }
  else
  {
    try
    {
      // Read line
      char line[LINE_SIZE];
      file.getline(line, LINE_SIZE);

      while(!file.eof() && file.good())
      {
        // Check first character in trimmed line
        switch(*strtrim(line))
        {
          case '#':
          case '*':
            // Comment line
            file.getline(line, LINE_SIZE);
            break;
          case '$':
          case '&':
          {
            // Block specifier
            int blockCommand = GetBlockSpec(line);

            switch(blockCommand)
            {
              case UNKNOWN_SPEC:
                throw clExceptionTree("clGeometryReader", "ReadFile", "Unknown block specifier.");
                break;
              case POINT_SPEC:
                // Start of point block
                throw clExceptionTree("clGeometryReader", "ReadFile", "Unexpected start of block POINT.");
                break;
              case PATCH_SPEC:
                // Start of patch block
                throw clExceptionTree("clGeometryReader", "ReadFile", "Unexpected start of block PATCH.");
                break;
              case SURFACE_SPEC:
                // Start of surface block
                throw clExceptionTree("clGeometryReader", "ReadFile", "Unexpected start of block SURFACE.");
                break;
              case GEOMETRY_SPEC:
                // Start of geometry block
                ReadGeometry(file, geometry);
                file.getline(line, LINE_SIZE);
                break;
              case END_SPEC:
                // End of block specification
                throw clExceptionTree("clGeometryReader", "ReadFile", "Unexpected EndOfBlock specification.");
                break;
              default:
                throw clExceptionTree("clGeometryReader", "ReadFile", "Function GetBlockSpec returned unknown value.");
                break;
            }

            break;
          }
          case '/':
            // End of block specification
            throw clExceptionTree("clGeometryReader", "ReadFile", "Unexpected EndOfBlock specification.");
            break;
          default:
            file.getline(line, LINE_SIZE);
            break;
        }
      }

      // Close file
      file.close();
    }

    catch(clExceptionTree EX)
    {
      // Make sure to close the file
      file.close();
      EX.AddMethodToTree("clGeometryReader::ReadFile");
      throw EX;
    }
  }
}

void clGeometryReader::ReadGeometry(fstream &file, clGeometry* geometry) const
{
  int numberSurfaces = 0;
  int EndOfBlock     = 0;

  // Read line
  char line[LINE_SIZE];
  file.getline(line, LINE_SIZE);

  while(!file.eof() && file.good() && !EndOfBlock)
  {
    // Check first character in trimmed line
    switch(*strtrim(line))
    {
      case '#':
      case '*':
        // Comment line
        file.getline(line, LINE_SIZE);
        break;
      case '$':
      case '&':
      {
        // Block specifier
        int blockCommand = GetBlockSpec(line);

        switch(blockCommand)
        {
          case UNKNOWN_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadGeometry", "Unknown block specifier.");
            break;
          case POINT_SPEC:
            // Start of point block
            throw clExceptionTree("clGeometryReader", "ReadGeometry", "Unexpected start of block POINT.");
            break;
          case PATCH_SPEC:
            // Start of patch block
            throw clExceptionTree("clGeometryReader", "ReadGeometry", "Unexpected start of block PATCH.");
            break;
          case SURFACE_SPEC:
          {
            // Start of surface block
            clSurface *surface = new clSurface(numberSurfaces++);

            ReadSurface(file, surface);
            geometry->AddSurface(surface);

            file.getline(line, LINE_SIZE);
            break;
          }
          case GEOMETRY_SPEC:
            // Start of geometry block
            throw clExceptionTree("clGeometryReader", "ReadGeometry", "Unexpected start of block GEOMETRY.");
            break;
          case END_SPEC:
            // End of geometry block specification
            EndOfBlock = 1;
            break;
          default:
            throw clExceptionTree("clGeometryReader", "ReadGeometry", "Function GetBlockSpec returned unknown value.");
            break;
        }

        break;
      }
      case '/':
        // End of geometry block specification
        EndOfBlock = 1;
        break;
      default:
        file.getline(line, LINE_SIZE);
        break;
    }
  }
}

void clGeometryReader::ReadSurface(fstream &file, clSurface* surface) const
{
  int EndOfBlock    = 0;

  // Read line
  char line[LINE_SIZE];
  file.getline(line, LINE_SIZE);

  while(!file.eof() && file.good() && !EndOfBlock)
  {
    // Check first character in trimmed line
    switch(*strtrim(line))
    {
      case '#':
      case '*':
        // Comment line
        file.getline(line, LINE_SIZE);
        break;
      case '$':
      case '&':
      {
        // Block specifier
        int blockCommand = GetBlockSpec(line);

        switch(blockCommand)
        {
          case UNKNOWN_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadSurface", "Unknown block specifier.");
            break;
          case POINT_SPEC:
          {
            // Start of point block
            dMatrix pointsX, pointsY, pointsZ;

            ReadPoints(file, pointsX, pointsY, pointsZ);
            surface->CreatePatches(pointsX, pointsY, pointsZ);

            file.getline(line, LINE_SIZE);
            break;
          }
          case PATCH_SPEC:
          {
            // Start of patch block
            clPatch *patch = new clPatch;

            ReadPatch(file, patch);
            surface->AddPatch(patch);

            file.getline(line, LINE_SIZE);
            break;
          }
          case SURFACE_SPEC:
            // Start of surface block
            throw clExceptionTree("clGeometryReader", "ReadSurface", "Unexpected start of block SURFACE.");
            break;
          case GEOMETRY_SPEC:
            // Start of geometry block
            throw clExceptionTree("clGeometryReader", "ReadSurface", "Unexpected start of block GEOMETRY.");
            break;
          case END_SPEC:
            // End of geometry block specification
            EndOfBlock = 1;
            break;
          default:
            throw clExceptionTree("clGeometryReader", "ReadSurface", "Function GetBlockSpec returned unknown value.");
            break;
        }

        break;
      }
      case '/':
        // End of geometry block specification
        EndOfBlock = 1;
        break;
      default:
        file.getline(line, LINE_SIZE);
        break;
    }
  }
}

void clGeometryReader::ReadPoints(fstream &file, dMatrix &pointsX, dMatrix &pointsY, dMatrix &pointsZ) const
{
  int         EndOfBlock;
  int         AllRead;
  char        line[LINE_SIZE];
  char       *ptrLine = line;
  const char *tkn     = " ;:";
  int         countData;
  int         rmax = 0;
  int         cmax = 0;

  // Read line
  file.getline(line, LINE_SIZE);

  EndOfBlock = 0;
  AllRead    = 0;
  countData  = 0;
  while(!file.eof() && file.good() && !EndOfBlock)
  {
    // Check first character in trimmed line
    switch(*strtrim(line))
    {
      case '#':
      case '*':
        // Comment line, read next line
        file.getline(line, LINE_SIZE);
        break;
      case '&':
      case '$':
      {
        // Block specifier
        int blockCommand = GetBlockSpec(line);

        switch(blockCommand)
        {
          case UNKNOWN_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Unknown block specifier.");
            break;

          case POINT_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Unexpected start of block POINT.");
            break;

          case PATCH_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Unexpected start of block PATCH.");
            break;

          case SURFACE_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Unexpected start of block SURFACE.");
            break;

          case GEOMETRY_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Unexpected start of block GEOMETRY.");
            break;

          case END_SPEC:
            EndOfBlock = 1;
            break;

          default:
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Function GetBlockSpec returned unknown value.");
            break;
        }

        break;
      }
      case '/':
        // End of block specifier
        EndOfBlock = 1;
        break;
      default:
        if(strlen(strtrim(line)) > 0)
        {
          // Data line
          countData++;

          // Matrix specifiers
          if(countData==1)
          {
            // column defines constant u-values
            ptrLine = strtok(line, tkn);
            cmax    = str2int(ptrLine);

            // row defines constant v-values
            ptrLine = strtok(NULL, tkn);
            rmax    = str2int(ptrLine);

            // Set matrix dimensions
            pointsX.SetNumberRows(rmax);
            pointsX.SetNumberColumns(cmax);
            pointsY.SetNumberRows(rmax);
            pointsY.SetNumberColumns(cmax);
            pointsZ.SetNumberRows(rmax);
            pointsZ.SetNumberColumns(cmax);
          }

          // Specification of the x-coordinates
          else if(countData>=2 && countData<(2+rmax))
          {
            int row = (countData-1);

            // Set first u-element in current v
            ptrLine = strtok(line, tkn);
            pointsX.SetElement(row, 1, str2double(ptrLine));

            // Set following u-elements in current v
            for(int c=2; c<=cmax; c++)
            {
              ptrLine = strtok(0, tkn);
              pointsX.SetElement(row, c, str2double(ptrLine));
            }
          }

          // Specification of the y-coordinates
          else if(countData>=(2+rmax) && countData<(2+rmax+rmax))
          {
            int row = (countData-1-rmax);

            // Set first u-element in current v
            ptrLine = strtok(line, tkn);
            pointsY.SetElement(row, 1, str2double(ptrLine));

            // Set following u-elements in current v
            for(int c=2; c<=cmax; c++)
            {
              ptrLine = strtok(0, tkn);
              pointsY.SetElement(row, c, str2double(ptrLine));
            }
          }

          // Specification of the z-coordinates
          else if(countData>=(2+rmax+rmax) && countData<(2+rmax+rmax+rmax))
          {
            int row = (countData-1-rmax-rmax);

            // Set first u-element in current v
            ptrLine = strtok(line, tkn);
            pointsZ.SetElement(row, 1, str2double(ptrLine));

            // Set following u-elements in current v
            for(int c=2; c<=cmax; c++)
            {
              ptrLine = strtok(0, tkn);
              pointsZ.SetElement(row, c, str2double(ptrLine));
            }
          }

          // Too much data
          else
          {
            throw clExceptionTree("clGeometryReader", "ReadPoints", "Too much data.");
          }

          if(countData==(1+rmax+rmax+rmax))
          {
            AllRead = 1;
          }
        }

        file.getline(line, LINE_SIZE);
        break;
    }
  }

  if(!EndOfBlock || !AllRead)
  {
    throw clExceptionTree("clGeometryReader", "ReadPoints", "Unexpected end while reading block POINT.");
  }
}

void clGeometryReader::ReadPatch(fstream &file, clPatch* patch) const
{
  int          EndOfBlock;
  int          AllRead;
  char         line[LINE_SIZE];
  char         *ptrLine = line;
  const char   *tkn     = " ;:";
  int          countData;

  int     ID   = 0;
  char    type = 'B';
  dMatrix GX(4, 4);
  dMatrix GY(4, 4);
  dMatrix GZ(4, 4);

  // Read line
  file.getline(line, LINE_SIZE);

  EndOfBlock = 0;
  AllRead    = 0;
  countData  = 0;
  while(!file.eof() && file.good() && !EndOfBlock)
  {
    // Check first character in trimmed line
    switch(*strtrim(line))
    {
      case '#':
      case '*':
        // Comment line
        file.getline(line, LINE_SIZE);
        break;
      case '&':
      case '$':
      {
        // Block specifier
        int blockCommand = GetBlockSpec(line);

        switch(blockCommand)
        {
          case UNKNOWN_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPatch", "Unknown block specifier.");
            break;

          case PATCH_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPatch", "Unexpected start of block PATCH.");
            break;

          case SURFACE_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPatch", "Unexpected start of block SURFACE.");
            break;

          case GEOMETRY_SPEC:
            throw clExceptionTree("clGeometryReader", "ReadPatch", "Unexpected start of block GEOMETRY.");
            break;

          case END_SPEC:
            EndOfBlock = 1;
            break;

          default:
            throw clExceptionTree("clGeometryReader", "ReadPatch", "Function GetBlockSpec returned unknown value.");
            break;
        }

        break;
      }
      case '/':
        // End of block specifier
        EndOfBlock = 1;
        break;
      default:
        if(strlen(strtrim(line)) > 0)
        {
          // Data line
          countData++;
          switch(countData)
          {
            // ID Number
            case 1:
              ID      = str2int(line);
              AllRead = 0;
              break;

            // blendType
            case 2:
              if(chrloc('H', line) != -1)
              {
                type = 'H';
              }
              else if(chrloc('B', line) != -1)
              {
                type = 'B';
              }
              else
              {
                throw clExceptionTree("clGeometryReader", "ReadPatch", "Unknown blending type.");
              }
              break;

            // Matrix GX
            case 3:
            case 4:
            case 5:
            case 6:
              ptrLine = strtok(line, tkn);
              GX.SetElement(countData-2, 1, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GX.SetElement(countData-2, 2, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GX.SetElement(countData-2, 3, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GX.SetElement(countData-2, 4, str2double(ptrLine));
              break;

            // Matrix GY
            case 7:
            case 8:
            case 9:
            case 10:
              ptrLine = strtok(line, tkn);
              GY.SetElement(countData-6, 1, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GY.SetElement(countData-6, 2, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GY.SetElement(countData-6, 3, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GY.SetElement(countData-6, 4, str2double(ptrLine));
              break;

            // Matrix GZ
            case 11:
            case 12:
            case 13:
            case 14:
              ptrLine = strtok(line, tkn);
              GZ.SetElement(countData-10, 1, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GZ.SetElement(countData-10, 2, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GZ.SetElement(countData-10, 3, str2double(ptrLine));

              ptrLine = strtok(0, tkn);
              GZ.SetElement(countData-10, 4, str2double(ptrLine));

              if(countData == 14)
              {
                AllRead = 1;

                // Correct for right hand axis system when type is Bezier
                if(type=='B')
                {
                  GX.Transpose(); GX.MirrorColumns();
                  GY.Transpose(); GY.MirrorColumns();
                  GZ.Transpose(); GZ.MirrorColumns();
                }

                // Now create the patch
                patch.SetNumberID(ID);
                patch.SetGX(GX);
                patch.SetGY(GY);
                patch.SetGZ(GZ);
                patch.SetBlendType(type);
              }
              break;

            // Error
            default:
              throw clExceptionTree("clGeometryReader", "ReadPatch", "Wrong number of data lines.");
              break;
          }
        }

        file.getline(line, LINE_SIZE);
        break;
    }
  }

  if(!EndOfBlock || !AllRead)
  {
    throw clExceptionTree("clGeometryReader", "ReadPatch", "Unexpected end while reading block PATCH.");
  }
}


/*
**---------------
** Public methods
**---------------
*/

/*
** Constructor and Destructor
*/

clGeometryReader::clGeometryReader(const char *fName, clGeometry* geometry)
{
  try
  {
    fileNameLength = (int) (strlen(&(*fName)));
    fileName       = strdup(&(*fName));

    ReadFile(geometry);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometryReader::clGeometryReader");
    throw(EX);
  }
}

clGeometryReader::~clGeometryReader(void)
{
  if(fileName)
  {
    delete [] fileName;
  }
}

