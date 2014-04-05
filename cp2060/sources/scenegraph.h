#ifndef __scenegraph_h__
#define __scenegraph_h__
#include "object.h"
#include "camera.h"
/**
	The scene graph is responsible for rendering the scene.
	It contains a heirachy of objects. But therein lies a problem.

	The order in which they would normally be drawn (parent->child)
	may not be correct. Objects should be drawn in back to front order
	so it's up to the scene graph to order them appropriately.
*/
class SceneGraph
{
	SceneGraph();
	~SceneGraph();

	void setCamera(CCamera *camera);
	void render(unsigned int frame);

private:
	Object *root;
};


#endif // _scenegraph_h__
_

