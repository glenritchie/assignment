#ifndef _vertexarray_h_
#define _vertexarray_h_
#include <vector>
#include <algorithm>
#include <deque>
#include <set>
#include "utils.h"
#include "ctexture.h"

//const unsigned int MAX_SET_SIZE = 10000;
//#pragma pack(push)
//#pragma pack(1)
/**
 A vertex struct
*/
struct Vertex
{
//	Vertex() : u(texcoord1[0]), v(texcoord1[1]) { ; }
	float position[3];		// 3D position
	float colour[4];		// RGBA colour
	float texcoord1[2];		// Tex co-ords
	float texcoord2[2];		// Tex co-ords
//	float colour[4];		// RGBA colour
	float normal[3];		// Normal
//	CTexture *texture;
//	unsigned int detail_texid;
//	float &u;
//	float &v;
};
typedef struct vi
{
	Vertex v;
	unsigned int index;
}vi;
//#pragma pack( pop )
bool operator==(const Vertex &a, const Vertex &b);

/**
 * A base class to handle rendering of a set of vertices. Internally the derived classes can store the verts in an order they want
 * but usually it's an indexed vertex array to reduce space and bandwidth.<br>
 * The derived class is only responsible for implementing Render and rebuild. Rebuild is called when the derived class should check the 'dirty' flag
 * and if it's true rebuild it's arrays, because they have changed
 */
class VertexSet
{
public:
	/// The type of vertex set to make	
	enum set_type { 
		/// Triangles (3 verts)
		TRIANGLES, 
		/// Fan ( one vert as center, others spanning off it
		FANS, 
		/// Every 4 verts make a quad
		QUADS, 
		/// Reuse every 3rd vertex
		STRIP 
		};

	/* 
	 * Construct a new vertex set based on the given type.
	 * @param type The type of set to make. 
	 * @param Static Will this vertex set change often?
	*/
	VertexSet(set_type type, bool Static=false)
	{
		b_static = Static; current_idx=duplicates=fans=unique=0;
		s_type = type; dirty = false;
		local_texture_state_changes = 0;
		useSecondTextureCoords = false;
	}
	/// Deconstructor
	virtual ~VertexSet() { /*DText("Vset::~Vset() : Duplicates found: %d", duplicates);*/; }
	
	// Should be something like this
	//	virtual void AssignTexture(unsigned int start, unsigned int end, Ctexture *tex) = 0;
//	virtual void AssignTexture(unsigned int start, unsigned int end, unsigned int tid) = 0;
//	virtual void SetAlpha(unsigned int start, unsigned int end, float value) = 0;

	/// Render the array and return the number of verts rendered
	virtual unsigned int Render()  = 0;
	/// Rebuild the vertex arrays
	virtual void rebuild() = 0;						

	/** Add a new face made up of 3 vertices
	 * @param newVertex1 The first vertex
	 * @param newVertex2 The second vertex
	 * @param newVertex3 The third vertex
	 * @param textureID The texture id to assign to this face
	 * @return The index in the array that the vertex was inserted. If it was less than size() then the vertex was NOT inserted
	 * and only a new index was created.
	 */
	unsigned int AddFace(const Vertex &newVertex1, const Vertex &newVertex2, const Vertex &newVertex3, unsigned int textureID) ;

	/** Add a new face made up of any number of vertices
	 * @param newVertex The vertices that make up this face
	 * @param numVertices The number of vertices in this face
	 * @param textureID The texture id to assign to this face
	 * @return The index in the array that the vertex was inserted. If it was less than size() then the vertex was NOT inserted
	 * and only a new index was created.
	 */
	unsigned int AddFace(const Vertex *newVertex, unsigned int numVertices, unsigned int textureID) ;
	
	/** Get array size(verts) 
	 * @return The number of vertices currently in memory
	 */
	unsigned int size() const { return (unsigned int)vertex_list.size(); }	
	//Vertex& grabVertex(unsigned int id)  { return vertex_list[id]; } ;	// Get the vertex at index
	
	/** Tell this vertex set to use the normals associated with the vertices. You may be wondering when you wouldn't want to use the normals?.
	 * Well... Text on the screen is a good example. :)
	 * @param newvalue Set to true to enable the use of normals
	 */
	void setUseNormal(bool newvalue) { useNormals = newvalue; }
	/** 
   * Tell this vertex set to use the second set of texture-coords. This current implementation has limited support for 2 texture streams(multi-texturing).
	 * This function will have no effect if multi-texturing has been turned off in the program settings.
	 * @param newvalue Set to true to enable the use of multi-texturing
	 */
	void setUseSecondTextureCoords(bool newvalue) { useSecondTextureCoords = newvalue; }
	/**
	 * Attempt to turn the standard vertex array and indicies to make multiple fans ( NOT FULLY IMPLEMENTED YET!! )
	 */
	void TryFan() ;
	
	/** Get a copy of the indices in this set
	 * @return A vector of the indicies */
	std::vector<unsigned int> getIndices() const	{ return vertex_order; }
		/** Get a copy of the vertices in this set
	 * @return A vector of the vertices */
	std::vector<Vertex> getVertices()	const { return vertex_list; }
	/** Get a copy of the texture ids in this set
	 * @return A vector of the ids */
	std::vector<unsigned int> getFaceTextureIDs()	const { return faceTextureIDs; }
/** Get the number of texture state changes that are predicted to occur on the next call to render()
	* @param The number of texture state changes
	*/
	unsigned int getTextureStateChanges() const { return local_texture_state_changes; }

	/**
	 * Optimize the order of the triangles so that the minumum number of texture state changes occur
	 * Only works for TRIANGLES
	*/
	void optimizeByTexture();
	/**
	 * Force an update of the vertex arrays and ensure that the derived classes render up2data data
	 */
	void setDirty() 
	{ 
		dirty = true; 
//		rebuild();
	}
	/** Get the number of duplicate vertices that were discarded<br>
	 *  Approximate number of bytes saved will be (duplicates*sizeof(Vertex)) - (duplicates*sizeof(unsigned int))
	 *  @return The number of duplicate verticces that were discarded
	 */
	unsigned int getDuplicates() const { return duplicates; }


void VertexSet::setData(unsigned int indices[], unsigned int icount,
                        Vertex vertices[], unsigned int vcount, 
												unsigned int texids[], unsigned int tcount);

private:
	unsigned int AddVertex(const Vertex &newVertex) ;						// Add a vertex(no duplicates!)
protected:

	/// Set for quick check of duplicates
//	std::set<Vertex> set_v_data;
	std::set<vi> set_v_data;
	/// Array for vertex indices
	std::vector<unsigned int> vertex_order;			
	/// Array for vertices
	std::vector<Vertex> vertex_list;
	/// Array for face ids
	std::vector<unsigned int> faceTextureIDs;
	/// Is this array expected to never change?
	bool b_static;
	/// Current index in array
	unsigned int current_idx;						
	/// Array type(list, fan etc)
	set_type s_type;							
	/// Have arrays changed since last render call?
	bool dirty;									
	/// Number of duplicates discarded
	unsigned int duplicates;
	/// Number of unique vertices
	unsigned int unique;	
	/** Use the normals of each vertex */
	bool useNormals;
	/** Use the second texture coords(multi-texturing) */	
	bool useSecondTextureCoords;
	
	/// Fans
	std::vector<unsigned int> fan_count;			
	// Number of fans
	unsigned int fans;
	/** Create a fan */
	void DoFan(int val, std::deque<unsigned int> &v_fan, std::vector<unsigned int> &v_nofan);
	/// number of texture state changes expected to occur
	unsigned int local_texture_state_changes;
};


#endif // _vertexarray_h_

