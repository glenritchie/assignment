#ifndef __light_h__
#define __light_h__
#include "base_entity.h"
#include "anim_texture.h"

//class CSprite;
/**
 * A light is an entity that lights up the area around it.<br>
 * You can create any colour light by giving different amounts of red/gree/blue.<br>
 * eg.<br>
 * <code>
 * Light *l = new Light(255,0,0,50.0f)	// Create a new red light<br>
 * Light *l = new Light(0,255,0,50.0f)	// Create a new green light<br>
 * Light *l = new Light(0,0,0,50.0f)	// Create a new BLACK(yes black!) light<br>
 * // Note that after you create a light you MUST add it to the LightManager<br>
 * lightManager.add(l);<br>
 * </code>
 */
class Light : public CBaseEntity
{
public:
	/**
	 * Contruct a light with the specified params<br>
	 * @param red The red component. 0(none)-255(full red)
	 * @param green The green component. 0(none)-255(full green)
	 * @param blue The blue component. 0(none)-255(full blue)
	 * @param range The range in world co-ords that the light will reach
	*/
	Light(int red =255, int green=255, int blue=255, float range = 50.0f);

	void subinit();
	/** Flip the lights enabled flag. If it was 0 then change to 1, if 1 change to 0*/
	void flipEnabled() { enabled = !enabled; }
		
	/** Set the light on or off
	 * @param newState The new state of the light. 0 - Off, 1 - On
	 */
	void enableLight(bool newState);
	/** Is the light enabled?
	 * @return true if it is, else false
	 */
	bool isLightEnabled();
	/**
	 * Set the range of the light. You should set this as low as you can get away with to prevent 
	 * the light being applied or calculated on vertices that don't need it.
	 * @param newRange The range of the light in world co-ords
	 */
	void setRange(float newRange);
	/** Get the range of the light
	 * @return The range of the light in world co-ords
	 */
	float getRange();

	void Render();
	void PreRender();
	void Cleanup();
	
	/** Get the red component of the light 
	 * @return A value in the range of 0(none) to 255(full) that specifies the concentration of this colour
	 */
	int getRed();
	/** Get the blue component of the light 
	 * @return A value in the range of 0(none) to 255(full) that specifies the concentration of this colour
	 */
	int getBlue();
	/** Get the green component of the light 
	 * @return A value in the range of 0(none) to 255(full) that specifies the concentration of this colour
	 */
	int getGreen();

	/** Get the red component of the light as a float
	 * @return A value in the range of 0(none) to 1.0f(full) that specifies the concentration of this colour
	 */
	float getFloatRed();
	/** Get the blue component of the light as a float
	 * @return A value in the range of 0(none) to 1.0f(full) that specifies the concentration of this colour
	 */
	float getFloatBlue();
	/** Get the green component of the light as a float
	 * @return A value in the range of 0(none) to 1.0f(full) that specifies the concentration of this colour
	 */
	float getFloatGreen();

	/** Set the red component of the light
	 * @param newRed A value in the range of 0(none) to 255(full) that specifies the concentration of this colour
	 */
	void setRed(int newRed);
	/** Set the green component of the light
	 * @param newGreen A value in the range of 0(none) to 255(full) that specifies the concentration of this colour
	 */
	void setGreen(int newGreen);
	/** Set the blue component of the light
	 * @param newBlue A value in the range of 0(none) to 255(full) that specifies the concentration of this colour
	 */
	void setBlue(int newBlue);

	void Think();
	void Collision(CBaseEntity *other);

	void getbbox(VECTOR3 &min, VECTOR3 &max);
	
	/** Some temporary and not finished test data to allow lights to have a parent that they are relative to
	*/
	float m[16];
	/** Some temporary and not finished test data to allow lights to have a parent that they are relative to
	*/
	bool relative;
	bool getStatic() { return staticLight; }
	void setStatic(bool value) { staticLight = value; }
protected:
	bool staticLight;
	/** The red	concentration of this colour from 0 to 255 */
	int r;
	/** The green concentration of this colour from 0 to 255 */
	int g;
	/** The blue	concentration of this colour from 0 to 255 */
	int b;
	//specular etc in here!
	/** A lens flare sprite */
//	CSprite *s;
	/** Is this light enabled? */
	bool enabled;
	/** The range of this light in world co-ordinates. */
	float range;

	

	//CTexture *lightTexture;
	CAnimatedTexture *lightTexture;
};


class PositionalLight : public Light { };
class SpotLight : public Light { };
#endif // __light_h__
