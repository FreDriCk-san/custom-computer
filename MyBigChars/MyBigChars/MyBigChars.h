#include <iostream>
#include "../../MyTerm/MyTerm/MyTerm.h"

char* bc_convertStringToCharArr(std::string line);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(char key, int x, int y, Colors foreground, Colors background);
int bc_initMyBigChars();
int bc_loadSymbols(const char * filename);