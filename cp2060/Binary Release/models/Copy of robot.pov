// Object file format
//
// Defaults are 0 unless otherwise specified
// object <name>
// {
//    parameter <value>
//		- parameters can be of the following types
//			- origin <x> <y> <z> 
//				- These values are relative to the parent object
//			- angles <Rotation on the X axis>, <Rotation on the Y axis>, <Rotation on the Z axis>
//			- pivot <x> <y> <z>
//				- This point is relative to the origin of this object
//			- scale  <scale on X axis> <scale on Y axis> <scale on Z axis>
//				- default ( 1 1 1 )
//			- type <typename>
//			{
//				- <typename> is one of the predefined types
//						- none		- This object is a placeholder and will not be drawn
//						- box		- This object is a box
//						- cylinder	- This object is a cylinder 
//									- It is aligned along the Z axis and should be rotated using
//									  fixedAngles param.
//						- sphere	- This object is a sphere
//				- After the braces there follows parameters sepcific to the object type
//				<parameter> <value>
//					- parameter depends on the object
//						- sphere object params
//							- radius <value> 
//								- This is the radius of the object
//						- box object params
//							- depth <value>
//								- This is the depth(along Z axis) of the object
//							- height <value>
//								- This is the height(along the Y axis) of the object
//							- width <value>
//								- This is the width(along the X axis) of the object
//						- cylinder object
//							- topRadius <value>
//								- This is the radius of the top of the cylinder	
//							- baseRadius <value>
//								- This is the radius of the base of the cylinder	
//							- height <value>
//								- This is the height of the cylinder
//
//
//			}
//	  (additional parameters may be added after type declaration)
//	  (additional objects may be added. These object automatically get the enclosing object
//		as it's parent)
// }
//
//
//
//
//
object pelvis
{
	origin 0 6.4 0
	angles 0 0 0
	fixedAngles 0 0 0
	pivot 0 0 0
	limits 0 0 0
	scale 1 1 1
 	type box
		{
			depth 0.8
			width 0.8
			height 1
		}




	
object body
{
	origin 0 2 0
	angles 0 0 0
	fixedAngles 0 0 0
	pivot 0 -2 0
	limits 0 0 0
	scale 1 1 1
  // Body is a surface
 	type bezier
	{
		texture conanchest.tga
		density  5
		uKnotCount 16 
		vKnotCount 12 
		uKnots 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1
//		uKnots 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
//		vKnots 0 0 0 0 0 1 1 1  3 3 3
		vKnots 0 0 0 0 0 0 1 1 1 1 1 1
		// NOTE: numControlPoints = 4 * (vKnotsCount - controlPaths)
		// numPoints = (uKnotCount - uPoints) * (vKnotCount - vPoints)
		//      36  = (12 - 6) * (12 - 6) 
		//      48  = (16 - 8) * (12 - 6) 
		
		// Top path
		controlPath
		{
			controlPoint 0 2.5 -0.5
			controlPoint 0.3 2.5 -0.5
			controlPoint 0.3 2.5 0 
			controlPoint 0.3 2.5 0.5 
			controlPoint -0.3 2.5 0.5
			controlPoint -0.3 2.5 0  
			controlPoint -0.3 2.5 -0.5  
			controlPoint 0 2.5 -0.5  
		}
		controlPath
		{
			controlPoint 0 2.0 -1 
			controlPoint 4.7 2.0 -1 
			controlPoint 4.7 2.0 0
			controlPoint 1.0 2.0 2.5
			controlPoint -1.0 2.0 2.5  
			controlPoint -4.7 2.0 0
			controlPoint -4.7 2.0 -1 
			controlPoint 0 2.0 -1 
		}
		controlPath
		{
			controlPoint 0 1 -1 
			controlPoint 2.7 1 -1 
			controlPoint 2.7 1 0
			controlPoint 1.3 1 2.5
			controlPoint -1.3 1 2.5
			controlPoint -2.7 1 0  
			controlPoint -2.7 1 -1  
			controlPoint 0 1 -1  
		}
		controlPath
		{
			controlPoint 0 0 -0.3 
			controlPoint 1.0 0 -0.3 
			controlPoint 1.0 0 0
			controlPoint 1.7 0 1.0
			controlPoint -1.7 0 1.0
			controlPoint -1.0 0 0  
			controlPoint -1.0 0 -0.3  
			controlPoint 0 0 -0.3  
		}
		// Bottom path
		controlPath
		{
			controlPoint 0 -1 -1 
			controlPoint 2.7 -1 -1 
			controlPoint 2.7 -1 0
			controlPoint 1.7 -1 1.0
			controlPoint -1.7 -1 1.0
			controlPoint -2.7 -1 0  
			controlPoint -2.7 -1 -1  
			controlPoint 0 -1 -1  
		}
		// Bottom path
		controlPath
		{
			controlPoint 0 -2 -1 
			controlPoint 1.0 -2 -1 
			controlPoint 1.0 -2 0
			controlPoint 1.0 -2 1.0
			controlPoint -1.0 -2 1.0
			controlPoint -1.0 -2 0  
			controlPoint -1.0 -2 -1  
			controlPoint 0 -2 -1  
		}
	}
	//type box
	//{
	//	depth 1.6
	//	width 3.7
	//	height 4
  //}
	object neck
	{
		origin 0 2.7 0 
		angles 0 0 0
		pivot 0 0 0
		limits 0 0 0
		type box
		{
			depth 0.8
			width 0.8
			height 1
		}
		object head
		{
			origin 0 1.5 0
			angles 0 0 0
			pivot 0 0 0
			param 1.1 10 10
			limits 0 0 0
			type sphere
			{
				radius 1.1
			}
			object nose
			{
				origin 0.0 -0.1 1.4
				angles 1 0 2
				pivot 0 3 0
				param 0.7 0.2 0.2
				limits 0 0 0
				type box
				{
					depth 0.7
					width 0.2
					height 0.2
				}
			}
			object leftEye
			{
				origin -0.3 0.2 1.2
				angles 0 0 0
				pivot 0 0 0
				param 0.2 6 6
				limits 0 0 0
				type sphere 
				{
					radius 0.001
				}
			}
			object rightEye
			{
				origin 0.3 0.2 1.2
				angles 0 0 0
				pivot 0 0 0
				param 0.2 6 6
				limits 0 0 0
				type sphere 
				{
					radius 0.001
					//radius 0.2
				}
			}
		}
	}
	object upperLeftArm
	{
		origin 2 1.3 0 
		pivot 0 0.8 0 
		angles 0 0 0 
		param 0.6 0.6 2 
		limits 0 0 0
		//type cylinder
		//{
//			topRadius 0.3
			//baseRadius 0.3
			//height 2
		//}
	type bezier
	{
		density 10 
		uKnotCount 16 
		vKnotCount 12 
		uKnots 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1
		vKnots 0 0 0 0 0 0 1 1 1 1 1 1
		// NOTE: numControlPoints = 4 * (vKnotsCount - controlPaths)
		// numPoints = (uKnotCount - uPoints) * (vKnotCount - vPoints)
		//      36  = (12 - 6) * (12 - 6) 
		//      48  = (16 - 8) * (12 - 6) 
		
		// Top path
		controlPath
		{
			controlPoint 0.0 0.8 -0.3	
			controlPoint 0.3 0.8 -0.1	
			controlPoint 0.3 0.8 0.1	
			controlPoint 0.3 0.8 0.3	
			controlPoint -0.4 0.8 0.3	
			controlPoint -0.4 0.8 0.1	
			controlPoint -0.4 0.8 -0.1	
			controlPoint -0.0 0.8 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 0.6 -0.3	
			controlPoint 0.35 0.6 -0.1	
			controlPoint 0.35 0.6 0.1	
			controlPoint 0.35 0.6 0.3	
			controlPoint -0.40 0.6 0.3	
			controlPoint -0.40 0.6 0.1	
			controlPoint -0.40 0.6 -0.1	
			controlPoint -0.0 0.6 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 0.3 -0.3	
			controlPoint 0.4 0.3 -0.1	
			controlPoint 0.4 0.3 0.1	
			controlPoint 0.4 0.3 0.8	
			controlPoint -0.4 0.3 0.8	
			controlPoint -0.4 0.3 0.1	
			controlPoint -0.4 0.3 -0.1	
			controlPoint -0.0 0.3 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 -0.3 -0.3	
			controlPoint 0.45 -0.3 -0.1	
			controlPoint 0.45 -0.3 0.1	
			controlPoint 0.45 -0.3 0.5	
			controlPoint -0.45 -0.3 0.5	
			controlPoint -0.45 -0.3 0.1	
			controlPoint -0.45 -0.3 -0.1	
			controlPoint -0.0 -0.3 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 -0.6 -0.3	
			controlPoint 0.45 -0.6 -0.1	
			controlPoint 0.45 -0.6 0.1	
			controlPoint 0.45 -0.6 0.3	
			controlPoint -0.45 -0.6 0.3	
			controlPoint -0.45 -0.6 0.1	
			controlPoint -0.45 -0.6 -0.1	
			controlPoint -0.0 -0.6 -0.3	
		}
	controlPath
		{
			controlPoint 0.0 -1 -0.3	
			controlPoint 0.45 -1 -0.1	
			controlPoint 0.45 -1 0.1	
			controlPoint 0.45 -1 0.3	
			controlPoint -0.45 -1 0.3	
			controlPoint -0.45 -1 0.1	
			controlPoint -0.45 -1 -0.1	
			controlPoint -0.0 -1 -0.3	
		}
	}

//		type box
//		{
//			depth 0.6
//			width 0.6
//			height 2
//		}   
		object lowerLeftArm
		{
			origin 0 -2.25 0
			angles 0 0 0 
			pivot 0 1.25 0 
			param 0.6 0.6 2.5 
			limits 0 0 0
//			type box
//			{
//				depth 0.6
//				width 0.6
//				height 2.5
//			}   
	type bezier
	{
		density 10 
		uKnotCount 16 
		vKnotCount 12 
		uKnots 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1
		vKnots 0 0 0 0 0 0 1 1 1 1 1 1
		// NOTE: numControlPoints = 4 * (vKnotsCount - controlPaths)
		// numPoints = (uKnotCount - uPoints) * (vKnotCount - vPoints)
		//      36  = (12 - 6) * (12 - 6) 
		//      48  = (16 - 8) * (12 - 6) 
		
		// Top path
		controlPath
		{
			controlPoint 0.0 1.3 -0.3	
			controlPoint 0.45 1.3 -0.1	
			controlPoint 0.45 1.3 0.1	
			controlPoint 0.45 1.3 0.3	
			controlPoint -0.45 1.3 0.3	
			controlPoint -0.45 1.3 0.1	
			controlPoint -0.45 1.3 -0.1	
			controlPoint -0.0 1.3 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 0.6 -0.3	
			controlPoint 0.40 0.6 -0.1	
			controlPoint 0.40 0.6 0.1	
			controlPoint 0.40 0.6 0.3	
			controlPoint -0.40 0.6 0.3	
			controlPoint -0.40 0.6 0.1	
			controlPoint -0.40 0.6 -0.1	
			controlPoint -0.0 0.6 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 0.3 -0.3	
			controlPoint 0.4 0.3 -0.1	
			controlPoint 0.4 0.3 0.1	
			controlPoint 0.4 0.3 0.3	
			controlPoint -0.4 0.3 0.3	
			controlPoint -0.4 0.3 0.1	
			controlPoint -0.4 0.3 -0.1	
			controlPoint -0.0 0.3 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 -0.3 -0.3	
			controlPoint 0.35 -0.3 -0.1	
			controlPoint 0.35 -0.3 0.1	
			controlPoint 0.35 -0.3 0.3	
			controlPoint -0.35 -0.3 0.3	
			controlPoint -0.35 -0.3 0.1	
			controlPoint -0.35 -0.3 -0.1	
			controlPoint -0.0 -0.3 -0.3	
		}
		controlPath
		{
			controlPoint 0.0 -0.6 -0.3	
			controlPoint 0.3 -0.6 -0.1	
			controlPoint 0.3 -0.6 0.1	
			controlPoint 0.3 -0.6 0.3	
			controlPoint -0.3 -0.6 0.3	
			controlPoint -0.3 -0.6 0.1	
			controlPoint -0.3 -0.6 -0.1	
			controlPoint -0.0 -0.6 -0.3	
		}
	controlPath
		{
			controlPoint 0.0 -1 -0.3	
			controlPoint 0.25 -1 -0.1	
			controlPoint 0.25 -1 0.1	
			controlPoint 0.25 -1 0.3	
			controlPoint -0.25 -1 0.3	
			controlPoint -0.25 -1 0.1	
			controlPoint -0.25 -1 -0.1	
			controlPoint -0.0 -1 -0.3	
		}
	}







		object leftHand
			{
				origin 0 -1.75 0
				angles 0 0 0 
				pivot 0 0.5 0 
				param 0.5 0.5 1.0
				type box
				{
					depth 0.5
					width 0.5
					height 1
				}   
			}
		}
	}
	object upperRightArm
	{
		origin -2 1.0 0 
		pivot 0 0.75 0 
		angles 0 0 0 
		param 0.6 0.6 2 
		limits 0 0 0
		type box
		{
			depth 0.6
			width 0.6
			height 2
		}   
		object lowerRightArm
		{
			origin 0 -2.25 0
			angles 0 0 0 
			pivot 0 1.25 0 
			param 0.6 0.6 2.5 
			limits 0 0 0
			type box
			{
				depth 0.6
				width 0.6
				height 2.5
			}   
			object rightHand
			{
				origin 0 -1.75 0
				angles 0 0 0 
				pivot 0 0.5 0 
				param 0.5 0.5 1.0
				type box
				{
					depth 0.5
					width 0.5
					height 1
				}   
			}
		}
	}
}

	object upperLeftLeg
	{
		origin 1 -2.0 0
		angles 0 0 0
		pivot 0 1.5 0
		param 1 1 3
		limits 0 0 0
		type box
		{
			depth 1
			width 1
			height 3
		}   
		object lowerLeftLeg
		{
 			origin 0 -3.0 0
			pivot 0 1.5 0
			angles 0 0 0
			param 1 1 3 
			limits 0 0 0
			type box
			{
				depth 1
				width 1
				height 3
			}   
			object leftFoot
			{
				origin 0 -1.5 0.5 
				angles 0 0 0
				pivot 0 0.2 -0.5
				param 2 1 0.5 
				limits 0 0 0
				type box
				{
					depth 2
					width 1
					height 0.5
				}   
			}
		}
	}
	object upperRightLeg
	{
		origin -1 -2.0 0
		angles 0 0 0
		pivot 0 1.5 0
		param 1 1 3
		limits 0 0 0
		type box
		{
			depth 1
			width 1
			height 3
		}   
		object lowerRightLeg
		{
			angles 0 0 0
 			origin 0 -3.0 0
			pivot 0 1.5 0
			param 1 1 3
			limits 0 0 0
			type box
			{
				depth 1
				width 1
				height 3
			}   
			object rightFoot
			{
				origin 0 -1.5 0.5 
				angles 0 0 0
				pivot 0 0.2 -0.5
				param 2 1 0.5 
				limits 0 0 0
				type box
				{
					depth 2
					width 1
					height 0.5
				}   
			}
		}
	}

// This is the end of the pelvis
}
