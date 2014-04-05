#ifndef WIN32
#define GL_GLEXT_PROTOTYPES
#endif
#include "common.h"
#include "gl_varray.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
//#include <GL/glext.h>
#include <GL/glu.h>
#ifdef _DRIVER
#include <iostream>
using namespace std;
#endif

					int id2 = -1;
// Extensions
//GL_MAX_ELEMENTS_VERTICES_EXT
#ifdef PFNGLACTIVETEXTUREARBPROC 
extern void APIENTRY glClientActiveTextureARB (GLenum p);
extern void APIENTRY glActiveTextureARB (GLenum p);
#endif
//GL_MAX_ELEMENTS_INDICES_EXT

//typedef void (APIENTRY * PFNLOCKARRAYSEXTPROC) (int first, GLsizei count);
//typedef void (APIENTRY * PFNUNLOCKARRAYSEXTPROC) (void);
//extern PFNLOCKARRAYSEXTPROC LockArraysEXT;
//extern PFNUNLOCKARRAYSEXTPROC  UnlockArraysEXT;
//typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);
//extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTextureARB;

extern bool compiled_vertex_arrays_availiable;
extern bool use_compiled_vertex_arrays;
extern bool use_multitexture;
unsigned int texture_state_changes = 0;

gl_varray::gl_varray(set_type type, bool Static)  : VertexSet(type, Static), array(0), listid(0), i_array(0)
{
//	array = 0;
}
gl_varray::~gl_varray()
{ 
	if (glIsList(listid))
		glDeleteLists(listid, 1);
	// Make sure local data is destroyed
	if (array) delete [] array; 
	if (i_array) delete [] i_array;
	//	VertexSet::~VertexSet();
}
extern bool hasExtDrawRange;
void gl_varray::rebuild()
{
	//DebugText(EMSG, "%s", __PRETTY_FUNCTION__);
	unsigned int i=0;
	// If data has changed then we need to rebuild the arrays
	if (dirty)
	{
		local_texture_state_changes = 0;
		if (s_type == TRIANGLES)
			optimizeByTexture();

		//DebugText(EMSG, "Begin rebuild..");
		// Copy data into local arrays to build the display list
		if (array!=0)		delete [] array;		array = new Vertex[vertex_list.size()];
		if (i_array!=0)	delete [] i_array;	i_array = new unsigned int[vertex_order.size()];
		
		for (i=0;i<vertex_list.size();i++)  	array[i] = vertex_list[i];
		for (i=0;i<vertex_order.size();i++)		i_array[i] = vertex_order[i];

		// Delete the old list if there was one and create a new display list
		if (glIsList(listid))
			glDeleteLists(listid, 1);
		listid = glGenLists(1);

		// We are compiling this list
		glNewList(listid, GL_COMPILE);

		//DText("Recompiled Vset: duplicates: %d : saved %d bytes of memory", duplicates, sizeof(Vertex)*duplicates);
		dirty = false;
		
		// The amount of bytes between each vertex
		unsigned int stride = sizeof(Vertex);
		if (use_multitexture)
		{
#ifdef PFNGLACTIVETEXTUREARBPROC 
			glClientActiveTextureARB(GL_TEXTURE0_ARB); 
			glTexCoordPointer(2, GL_FLOAT, stride, ((unsigned char *)array) + offsetof(Vertex, texcoord1));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
			
			glClientActiveTextureARB(GL_TEXTURE1_ARB); 
			glTexCoordPointer(2, GL_FLOAT, stride, ((unsigned char *)array) + offsetof(Vertex, texcoord2));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
		}
		else
		{
			// Each texture co-ord is made of 2 floats at offset texcoord1
			glTexCoordPointer(2, GL_FLOAT, stride, ((char *)array) + offsetof(Vertex, texcoord1));
		}

		
		// Set up all the client states
		glEnableClientState(GL_COLOR_ARRAY);
		
		// If we are using normals then enable the state to use the array
		if (useNormals)
			glEnableClientState(GL_NORMAL_ARRAY);
		else	// Otherwise disable it coz we don't need it
			glDisableClientState(GL_NORMAL_ARRAY);
		// Enable states
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// Set up pointers to the data
		// Note the judicous use of the 'offsetof' keyword
		// This allows us to get the address of that variable at compile-time so we can
		// modify or add new user data to the Vertex structure without trying to figure out the 
		// new offsets manually. Also.. offsetof takes care of issues to do with packing etc.

		// Each colour is made of 4 floats ( RGBA, [0.0f - 1.0f] ) at offset colour
		glColorPointer(4, GL_FLOAT, stride, ((char *)array) + offsetof(Vertex, colour));
		
		// If we are using normals then we need a pointer to it
		if (useNormals)
		{
			// Each normal is required to be 3 floats and is at offset normal
			glNormalPointer(GL_FLOAT, stride, ((char *)array) + offsetof(Vertex, normal));
		}
		// Each vertex is made up of 3 vertices (eg. no w-coord yet ) at offset position
		glVertexPointer(3, GL_FLOAT, stride, ((char *)array) + offsetof(Vertex, position));

		unsigned int array_size = vertex_order.size();

/*		if (compiled_vertex_arrays_availiable && use_compiled_vertex_arrays)
			LockArraysEXT (0, array_size);
		void UnlockArraysEXT (void)
*/
		
		// Variables
		int oldtid = -1;		// The active texture id
		int tid = -1;			// The texture id for the vertex
		unsigned int o_id;		// The current index
		unsigned int idx;		// Tmp used to figure out how many verts to draw
		
		// Make sure we are drawing with textures
		glEnable(GL_TEXTURE_2D);
		
		// Figure out what type of primitives we should be drawing
		unsigned int type;
		unsigned int indicesPerFace = 0;
		if	(s_type == QUADS)	{ type = GL_QUADS; 					indicesPerFace = 4; }
		else if (s_type == TRIANGLES)	{ type = GL_TRIANGLES; 			indicesPerFace = 3; }
		else if (s_type == STRIP)	{ type = GL_TRIANGLE_STRIP; indicesPerFace = 4; }
		
		// Keep a pointer to the current indices to draw
		unsigned int *i_ary = i_array;
		
		// Go through the index array drawing stuff
		unsigned int faceID = 0;
		for (i = 0; i < array_size;)
		{
			faceID = i / indicesPerFace;
			o_id = i_array[i];
			tid = faceTextureIDs[faceID];
			//tid = array[o_id].texid;
			
			// If we are up to an index that has a different texture than the current we are using to draw
			// then switch textures ( this is always called the first iteration )
			if (oldtid != tid)
			{
				//glActiveTextureARB(GL_TEXTURE1_ARB);
				//glBindTexture(GL_TEXTURE_2D, array[o_id].detail_texid);
				if (use_multitexture && id2 != -1)
				{
#ifdef PFNGLACTIVETEXTUREARBPROC 
					//DebugText(EMSG, "rendering with multi: id2: %d", id2);
					glActiveTextureARB(GL_TEXTURE0_ARB);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, tid);
 
					glActiveTextureARB(GL_TEXTURE1_ARB);
					glEnable(GL_TEXTURE_2D);
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
					glBindTexture(GL_TEXTURE_2D, id2);

					glMatrixMode(GL_TEXTURE);
					glLoadIdentity();
					glScalef((float)16, (float)16, 1);
					glMatrixMode(GL_MODELVIEW);
#endif
				}
				else
					glBindTexture(GL_TEXTURE_2D, tid);

				local_texture_state_changes++;
				oldtid = tid; 
			}
			// Go through the faceTextureIDs array until we find another texture
			for (idx = faceID; ( idx < faceTextureIDs.size() ) && (faceTextureIDs[idx] == tid); ++idx);
			/** 4 Sept 2004 - Geez i am stupid!
			 * Look at the faceID in the condition variable for this for loop:
			 * for (idx = faceID; ( faceID < faceTextureIDs.size() ) && (faceTextureIDs[idx] == tid); ++idx)
			 		;
			 * Can you tell me what the hell it does? No? neither can I. It never gets changed.. It
			 * was causing some pretty stupid buffer overrun errors, anyway i changed it to 'idx' 
			 * and now it works fine.
			 */
			// idx will be the idx in the faceTextureIDs array that we encounterd another face
			// Work out how many indicies that is
			unsigned int count = (idx - faceID) * indicesPerFace;
			glDrawElements(type, count, GL_UNSIGNED_INT, i_ary);	// Draw the triangles
			i += count;
			i_ary += count;
		}
		//glEnd();
		glEndList();
		
		// Delete the data coz we don't need it anymore
		delete [] array;  	array = 0;
		delete [] i_array; 	i_array = 0;
	}

}
unsigned int gl_varray::Render()
{
	//return 0;
	
//#ifdef DEBUG
//dirty = true;
	if (dirty)
	{
		//DebugText(ERROR_2BFIXED, "%s : Render was called while dirty==true", __PRETTY_FUNCTION__);
		rebuild();
	}
//	return 0;	
//#endif
		
	// Draw the array. If it was dirty it would have been rebuilt by now
	// If the list is valid
	if (glIsList(listid))
	{
		// Draw the list
		glCallList(listid);
		texture_state_changes+=local_texture_state_changes;
		// Return how many tris were drawn
		return vertex_order.size()/3;
	}

	return 0;

}
