/************************************************
*** Header file for string convertion functions
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef STR_H
#define STR_H

#if defined(__cplusplus)
extern "C"{
#endif

int    chrloc(const char, const char*);
char*  stradd(const char*, const char*);
char*  strtrim(const char*);
int    str2int(const char*);
int    str2int2(const char*);
double str2double(const char*);

#if defined(__cplusplus)
}
#endif

#endif
