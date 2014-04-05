#include "common.h"

#include "vertexarray.h"
#include <memory.h>
#include <algorithm>
#include <iostream>
#include "utils.h"
//#include "profile.h"

#include "3dmath.h"

using namespace std;
/*
template <class c_type>
unsigned int unique_key(const c_type &v)
{
	// Return a unique key value
	unsigned char *a = (unsigned char *) &v;
	unsigned int checksum = 0;
	for (unsigned int i=0;i<sizeof(v);i++)
		checksum+=a[i];
	return checksum;
}
*/
bool operator==(const Vertex &a, const Vertex &b)
{
	return (memcmp(&a,&b,sizeof(a))==0);
}

/*
bool operator<(const Vertex &a, const Vertex &b)
{
	return (a.position[0] < b.position[0]);
}
*/
bool operator==(const vi &a, const vi &b)
{
	return (memcmp(&a,&b,sizeof(a))==0);
}

bool operator<(const vi &a, const vi &b)
{
	veryLong *al = (veryLong  *)&(a.v.position);
	veryLong *bl = (veryLong  *)&(b.v.position);
	return al < bl;
/*	float a1 = pow(a.v.position[0], 2);
	float a2 = pow(a.v.position[1], 4);
	float a3 = pow(a.v.position[2], 8);
	float a4 = a1 + a2 + a3;
	
	float b1 = pow(b.v.position[0], 2);
	float b2 = pow(b.v.position[1], 4);
	float b3 = pow(b.v.position[2], 8);
	float b4 = b1 + b2 + b3;
	*/
	//return a4 < b4;

/*	CVector3 zero;
	return (
		Distance(CVector3(a.v.position[0], a.v.position[1], a.v.position[2]), zero) < 
		Distance(CVector3(b.v.position[0], b.v.position[1], b.v.position[2]), zero));
	// return (a.v.position[0] < b.v.position[0]);
	*/
}

unsigned int VertexSet::AddFace(const Vertex &newVertex1, const Vertex &newVertex2, const Vertex &newVertex3,
						 unsigned int textureID)
{
	AddVertex(newVertex1);
	AddVertex(newVertex2);
	AddVertex(newVertex3);
	faceTextureIDs.push_back(textureID);
	return 0;
}
unsigned int VertexSet::AddFace(const Vertex *newVertex, unsigned int numVertices, unsigned int textureID)
{
	for (unsigned int i = 0; i < numVertices; i++)
		AddVertex(newVertex[i]);
	faceTextureIDs.push_back(textureID);
	return 0;
}
unsigned int VertexSet::AddVertex(const Vertex &newVertex)
{
	
	// Profile Func("C3DSModel::RenderFaces")
	dirty = true;
	// Add vertex.. but leave out duplicates
	unsigned int i = 0;
	/*
	11/07/2004 8:21PM - Spikeles
		This is stupid but i can't be bothered fixing it to use a the proper STL set class
	*/
	// Spend a couple of cycles and a little bit of memory to check using a hash function
	// wether this vertex is in here or not?
	vi v;
	v.v = newVertex;
#ifdef DEBUG
	// Just make sure the vertex was copied right
	if (!(v.v == newVertex))
	{
		DebugText(ERROR_CRITICAL, "OOOPS!!! Shallow copy occurred!!!!! v.v != newVertex !!!");
		exit(-1);
	}
#endif
	v.index = current_idx;
	pair< set<vi>::iterator, bool> inserted = set_v_data.insert(v);
	if (!inserted.second)	 // If this vertex IS in here.. we need to FIND it.
	{
		set<vi>::iterator iter = inserted.first;
		v = *iter;
		i = v.index;
#ifdef DEBUG
		// If the two vertex arrays are not equal we have a problem
		if ((newVertex.position[0] != vertex_list[i].position[0]) ||
			  (newVertex.position[1] != vertex_list[i].position[1]) ||
			  (newVertex.position[2] != vertex_list[i].position[2]))
	  {
			DebugText(EMSG, "ALERT! newVertex and vertex_list[%d] are different!", i);
	  }
		else
		{
			DebugText(EMSG, "Found dup at %d\nVertex to insert: %.2f, %.2f, %.2f\nVertex found: %.2f, %.2f, %.2f", 
								i, 
								newVertex.position[0], newVertex.position[1], newVertex.position[2],			
								vertex_list[i].position[0],vertex_list[i].position[1],vertex_list[i].position[2]);
			duplicates++;
			vertex_order.push_back(i);
			return i;
		}
#else
			duplicates++;
			vertex_order.push_back(i);
			return i;

#endif
/*		for (i=0;i<vertex_list.size();i++)
		{
			if (vertex_list[i] == newVertex)
			{
				duplicates++;
				vertex_order.push_back(i);
				return i;
			}
		}
	*/
	}
	/*int key = unique_key(newVertex);
	pair< set<unsigned int>::iterator, bool> inserted = set_v_data.insert(key);
	if (!inserted.second)	 // If this vertex IS in here.. we need to FIND it.
	{
		for (i=0;i<vertex_list.size();i++)
		{
			if (vertex_list[i] == newVertex)
			{
				duplicates++;
				//int p = *(inserted.first);	
				vertex_order.push_back(i);
				return i;
			}
		}
	}
*/
	vertex_list.push_back(newVertex);
	vertex_order.push_back(current_idx);
	unique++;
	current_idx++;
	return current_idx;
}

void VertexSet::setData(unsigned int indices[], unsigned int icount,
                        Vertex vertices[], unsigned int vcount, 
												unsigned int texids[], unsigned int tcount)
{
	// Load vertex order full of indicies
	faceTextureIDs.reserve(tcount);
	vertex_order.reserve(icount);
	vertex_list.reserve(vcount);
	
	unsigned int i = 0;	
	for (i = 0; i < tcount; i++) faceTextureIDs.push_back(texids[i]); 
	for (i = 0; i < icount; i++) vertex_order.push_back(indices[i]);
	for (i = 0; i < vcount; i++)
	{
		vertex_list.push_back(vertices[i]);
		vi v;
		v.v = vertices[i];
		v.index = i;
		set_v_data.insert(v);
	}
	
	current_idx = icount;
	unique = vcount;
	dirty = true;

}
void VertexSet::optimizeByTexture()
{
//	DebugText(EMSG, "%s", __PRETTY_FUNCTION__);
	if (s_type != TRIANGLES)
	{
		DebugText(EMSG, "Warning!! Attempt to optimize by texture something other than TRIANGLES");
		return;
	}
	
	// Basically we want to group all the triangles based on their texture id.
	// So. First count how many different textures there are in this array
	std::set<unsigned int> uniqueTextureSet;
	unsigned int i = 0, j =0;
	for (i = 0; i < faceTextureIDs.size(); i++)
	{
		unsigned int texid = faceTextureIDs[i];
		uniqueTextureSet.insert(texid);
	}
	// Do not bother trying to optimize if there is only one texture for this object
	if (uniqueTextureSet.size() == 1)
		return;
	DebugText(EMSG, "Unique textures in this array: %d", uniqueTextureSet.size());

	// Tricky bit
	// Copy the unique texture id's into an array. so that the id's can be indexed
	int *u = new int[uniqueTextureSet.size()];
	std::set<unsigned int>::iterator iter;
	i = 0;
	for (iter = uniqueTextureSet.begin(); iter != uniqueTextureSet.end(); iter++, i++)
		u[i] = *(iter);
	std::vector<unsigned int> *newOrder = new std::vector<unsigned int>[uniqueTextureSet.size()];
	/* 
		Spikeles - 11/07/2004 7:40PM
		Basically it works like so:
		newOrder[0] = Fill with all the faces with tid of 1's	
		newOrder[1] = Fill with all the faces with tid of 3's	
		newOrder[2] = Fill with all the faces with tid of 8's	
		The 1,3,8 are just numbers that could represent any texture id
		The code above maps the id's to a zero based position in an array.  
		Then the code below fills those arrays with data.
		The first vector ( newOrder[0] ) will be fed all the indices of a unique texture id.
		So you might get..
		newOrder[0] = {0,1,2},{2,3,1}
		This means there are two triangles ( 0-1-2 and 2-3-1 ) which both are supposed to have an id of u[0]
		After we do that then we simply go through each newOrder[i] array and copy it's contents into
		the main vertex_order array getting a nice optimized and sorted array.
		
		There are possibilities here to optimize based on preferences, but i don't know whether the order of faces
		and textures rendered matters.
		eg.
		Should all the big textures (512x512) be rendered before all the (32x32) textures?
		Should i render the MOST faces with a single texture first? or last? or should i order them in ascending or
			descending order?
		***Does it really matter???***

		Spikeles - 8 August 2004
		Hmmm.. just been thinking.. Perhaps the order does matter when it comes to caches???
	*/
	// Begin optimizing
	int faceID = 0;
	for (i=0;i<vertex_order.size();i+=3, faceID++)
	{
		int v[3] = {vertex_order[i],vertex_order[i+1],vertex_order[i+2]};
		// get the texture id for this triangle
		int tid = faceTextureIDs[faceID];
	
		// Find where it will be placed
		for (j = 0; j < uniqueTextureSet.size(); j++)	{ if (u[j] == tid) break; }
		newOrder[j].push_back(v[0]);
		newOrder[j].push_back(v[1]);
		newOrder[j].push_back(v[2]);
	}
/*
	// Print arrays
	for (i = 0; i < uniqueTextureSet.size(); i++)
	{
		DebugText(EMSG, "tid: %u", u[i]);
		for (j = 0; j < newOrder[i].size(); j+=3)
		{
			DebugText(EMSG, "%u, %u, %u", newOrder[i][j], newOrder[i][j+1], newOrder[i][j+2]);
			//vertex_order.push_back(newOrder[i][j]);
		}
	}
	
	
	// Print old order
	DebugText(EMSG, "---old order---");
	faceID = 0;
	for (i=0;i<vertex_order.size();i+=3, faceID++)
	{
		int v[3] = {vertex_order[i],vertex_order[i+1],vertex_order[i+2]};
		int tid = faceTextureIDs[faceID];
		DebugText(EMSG, "tid: %d, %d %d %d", tid,v[0],v[1],v[2] );
	}
*/
	// Clear vertex_order array and fill it with the new order
	faceTextureIDs.clear();
	vertex_order.clear();
	for (i = 0; i < uniqueTextureSet.size(); i++)
	{
		unsigned int indices = (unsigned int)newOrder[i].size();
		for (int y = 0; y < int(indices/3); y++)
			faceTextureIDs.push_back(u[i]);
		for (j = 0; j < newOrder[i].size(); j++)
			vertex_order.push_back(newOrder[i][j]);
	}
/*
	// Print new order	
	DebugText(EMSG, "---new order---");
	faceID = 0;
	for (i=0;i<vertex_order.size();i+=3, faceID++)
	{
		int v[3] = {vertex_order[i],vertex_order[i+1],vertex_order[i+2]};
		int tid = faceTextureIDs[faceID];
		DebugText(EMSG, "tid: %d, %d %d %d", tid,v[0],v[1],v[2] );
	}
*/	
	// Rebuild everything
	delete [] u;   
	delete [] newOrder;
	dirty = true;	
}

void VertexSet::DoFan(int val, std::deque<unsigned int> &v_fan, std::vector<unsigned int> &v_nofan)
{
	unsigned int idx=0;
	unsigned int i;
	// Indices contain unique indices


	unsigned int c;

	int *top=new int[unique];
	int *bottom=new int[unique];
	fill_n(top, unique, -1);
	fill_n(bottom, unique, -1);
	idx = 0;
	for (i=0;i<vertex_order.size();i+=3)
	{
		int v[3] = {vertex_order[i],vertex_order[i+1],vertex_order[i+2]};
		if (!(v[0]== val || v[1]==val || v[2]==val))
		{
			v_nofan.push_back(v[0]);
			v_nofan.push_back(v[1]);
			v_nofan.push_back(v[2]);
			continue;
		}
		// Shift right until val is the first value
		int tmp[3] = {v[0],v[1],v[2]};
		while (tmp[0]!=val)
		{
			int tmp2[3] = {tmp[0],tmp[1],tmp[2]};
			tmp[0]=tmp2[2];
			tmp[1]=tmp2[0];
			tmp[2]=tmp2[1];
		}
		bottom[idx]=tmp[1];
		top[idx++]=tmp[2];
	}
	int last, first;
	for (i=0;i<unique;i++)
	{
		c = count(top, top+unique, bottom[i]);
		if (c==0)
			first = bottom[i];
		c = count(bottom, bottom+unique, top[i]);
		if (c==0)
			last = top[i];
	}

	int find = last;	
	for (i=0;i<unique;i++)
	{
		if (top[i]==find)
		{
			v_fan.push_front(find);
			find = bottom[i];
			i=-1;
		}
	}
	v_fan.push_front(first);
	v_fan.push_front(val);

	delete [] top;
	delete [] bottom;
}	
void VertexSet::TryFan()
{
	unsigned int *indices=new unsigned int[unique];
	unsigned int *location=0;
	unsigned int idx=0;
	unsigned int i = 0;
	for (i=0;i<vertex_order.size();i++)
	{
		unsigned int v_idx = vertex_order[i];
		bool found=false;
		for (unsigned int k=0;k<idx;k++)
		{
			if (indices[k]==v_idx)
			{
				found = true;
				break;
			}
		}
		if (found)
			continue;
		indices[idx++]=v_idx;
	}


	std::deque<unsigned int> v_fan;
	std::vector<unsigned int> v_nofan;
	DoFan(0, v_fan, v_nofan);
	fans++;
	fan_count.push_back(v_fan.size());


	for (i=0;i<v_fan.size();i++)
		cout << v_fan[i] << ",";
	cout << endl;


	int triple=0;
	for (i=0;i<v_nofan.size();i++)
	{
		cout << v_nofan[i] << ",";
		triple++;
		if ((triple % 3) == 0)
			cout << "|";
	}
	cout << endl;
	delete [] indices;
}
