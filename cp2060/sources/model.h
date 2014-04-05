#ifdef _MSC_VER
// Disable warning 4786:
// "identifier was truncated to '255' characters in the debug information"
#pragma warning( disable : 4786)
#endif
#ifndef __MODEL_H__
#define __MODEL_H__
#include "object.h"

void readFrames(std::string &frameFile, Object *root);
void loadModelFile(std::string &fileName, Object *(&root));
void readline(std::ifstream &stream, std::string &line, unsigned int &lineNumber);
Object *createObject(std::string &type, Object *oldObject);

#endif // __MODEL_H__
