#ifndef __LightManager_h__
#define __LightManager_h__
//#include "vector.h"
//#include "base_entity.h"
//#include <vector>
class Light;
/**
 * A class to manage lights.<br>
 * At the beginning of each frame after you set up the camera matrix
 * call setupLightsForFrame() and the lights will be positioned. Lights will be enabled or disabled
 * based on distance to the camera and maximum set lights. eg. if only allowed 8 lights but you give 9 the closest
 * 8 will get turned on.<br>
 * There is only one drawback with this approach. You MUST specify the light co-ords in world-coords! I'm currently
 * working on a fix for this put i'm not finished yet.
 *
 */
class LightManager 
{
public:
	/** Construct a new LightManager */
	LightManager();
	/**
	 *  Set the number of lights that are applied to the world at once
     *	@param newMaxLights The new number
	 */
	void setMaxLights(long unsigned int newMaxLights) { maxLights = newMaxLights; }

	/**
	 * Calls the Graphics API sub-system to set up the lights in world space
	 */
	void setupLightsForFrame();
	void removeLight(Light *light);
	/**
	 * Add a new light to manage
	 */
	void addLight(Light *newLight);
	/**
	 * Remove a light
	 */
//	void removeLight(Light *
	/** Get the number of lights in the scene
	 * @return The number of lights in the scene */
	unsigned int getNumberOfLights();
	/** Get a light at the specified index 
	 * @param index The zero based index
	 * @return The light at the index or NULL
	*/
	Light *getLight(unsigned int index);
private:	
	long unsigned int maxLights;
	//std::vector<Light *> lightVector;
};
extern LightManager lightManager;
#endif // __LightManager_h__

