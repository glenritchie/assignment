#ifndef __master_class_h__
#define __master_class_h__
#include <string>
#include "common.h"
/** A master class that allows me to implement something similar to java's toString function */
class MasterClass
{
public:
	/** Return a string that describes this class 
	 * @return A string that describes this class
	 */
	virtual std::string toString() { return std::string("Not implemented"); }
};

#endif // __master_class_h__
