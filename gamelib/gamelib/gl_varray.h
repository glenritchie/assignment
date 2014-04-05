#ifndef _gl_varray_h_
#define _gl_varray_h_
#include "vertexarray.h"
#include "ctexture.h"
#include "resource_manager.h"

/**
 * A derived class of VertexSet designed for use in OpenGL
*/
class gl_varray : public  VertexSet
{
public:
	/// Create a new instance
	gl_varray(set_type type, bool Static=false);
	~gl_varray();

//	void AssignTexture(unsigned int start, unsigned int end, unsigned int tid);
//	void SetAlpha(unsigned int start, unsigned int end, float value);
	/** Render verts and return no of verts rendered
	 * @return The number of verts rendered
	*/
	unsigned int Render();	
	/// Rebuild changed data
	void rebuild();
private:

	Vertex *array;
	unsigned int *i_array;
//	unsigned int *tex_array;
	unsigned int listid;
//	unsigned int oldtid;
};

#endif // _gl_varray_h_
