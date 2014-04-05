#ifndef __file_h__
#define __file_h__

#include "object.h"

class FileObject : public Object
{
public:
		FileObject(string newName, string fileName);
		FileObject(Object *o);

		void readParams(ifstream &file, unsigned int &lineNumber, unsigned int &level);
protected:
		void shapeDraw() { ; }
private:
		string fileName;
		string idleFrames;
};

#endif //__file_h__

