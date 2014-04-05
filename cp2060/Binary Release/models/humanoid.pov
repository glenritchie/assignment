// A box, depth(z), width(x), height(y)
object body
{
	origin 0 8 0
	angles 0 0 0
	pivot 0 0 0
	limits 0 0 0
	scale 1.8 1.8 1.0
	type sphere 
	{
		baseRadius 1
		topRadius 1
		height 4 
		radius 1
	}
	object pelvis
	{
		origin 0 -2 0 
		angles 0 0 0
		pivot 0 0 0
		limits 0 0 0
		scale 1.4 1 0.7

		type sphere
		{
			radius 0.8
		}

	}
	object neck
	{
		origin 0 2 0 
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
				angles 0 0 0
				pivot 0 0 0
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
				origin -0.3 0.2 1.0
				angles 0 0 0
				pivot 0 0 0
				param 0.2 6 6
				limits 0 0 0
				type sphere 
				{
					radius 0.2
				}
			}
			object rightEye
			{
				origin 0.3 0.2 1.0
				angles 0 0 0
				pivot 0 0 0
				param 0.2 6 6
				limits 0 0 0
				type sphere 
				{
					radius 0.2
				}
			}
		}
	}

	object leftShoulderBall
	{
		origin 1.5 1.25 0
		//origin -1.0 1.2 0 
		scale 1.9 1 1 
		type sphere
		{
			radius 0.6  
		}
	

	object upperLeftArm
	{
		//origin 2.0 1.75 0
		origin 0.7 0.6 0 
		angles 0 0 0 
		pivot 0 -1.6 0 
		param 0.6 0.6 2 
		limits 0 0 0
		scale 0.5 1.5 0.6
		type sphere
		{
			radius 1
		}
//		type box
//		{
//			depth 0.6
//			width 0.6
//			height 2
//		}
	
		object lowerLeftArm
		{
			origin 0 -1.25 0
			angles 0 0 0 
			pivot 0 -1 0 
			param 0.6 0.6 2.5 
//			limits 0 0 0
			scale 0.5 1.5 0.6
			type sphere
			{
				radius 0.9 
			}


//			type box
//			{
//				depth 0.6
//				width 0.6
//				height 2.5
//			}   
			object leftHand
			{
				origin 0 -0.75 0
				pivot 0 -1.0 0 
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
	object upperRightArm
	{
//		origin -2.0 1.75 0
		origin -2 0.5 0 
		angles 0 0 0 
		pivot 0 .7 0 
		//param 0.6 0.6 2 
		limits 0 0 0
		type box
		{
			depth 0.6
			width 0.6
			height 2
		}   
		object lowerRightArm
		{
			origin 0 -1.25 0
			angles 0 0 0 
			pivot 0 -1 0 
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
				origin 0 -0.75 0
				pivot 0 -1.0 0 
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
	object upperLeftLeg
	{
		origin 1 -1.75 0
		angles 0 0 0
		pivot 0 -1.5 0
		param 1 1 3
		limits 0 0 0
		scale 0.7 1.9 0.6
		type sphere
		{
			radius 1.0 
		}
		object knee
		{
			origin 0 -1.4 0.3
			angles 0 0 0
			pivot 0 0 0
			scale 1.2 1 0.6
			type sphere
			{
				radius 0.4 
			}
		}


//		type box
//		{
//			depth 1
//			width 1
//			height 3
//	}   
		object lowerLeftLeg
		{
			origin 0 -1.5 0
			angles 0 0 0
			pivot 0 -1.5 0
			param 1 1 3 
			scale 0.7 1.9 0.6
			type sphere
			{
				radius 1.0 
			}
			limits 0 0 0
//			type box
//			{
//				depth 1
//				width 1
//				height 3
//			}   
			object leftFoot
			{
				origin 0 0 0.5 
				angles 0 0 0
				pivot 0 -1.5 0
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
		origin -1 -3.0 0
		angles 0 0 0
		pivot 0 0 0
		param 1 1 3
		limits 0 0 0
		scale 0.7 1.9 0.6
		type sphere
		{
			radius 1.0 
		}
		object rightknee
		{
			origin 0 -1.4 0.3
			angles 0 0 0
			pivot 0 0 0
			scale 1.2 1 0.6
			type sphere
			{
				radius 0.4 
			}
		}


//		type box
//		{
//			depth 1
//			width 1
//			height 3
//		}   
		object lowerRightLeg
		{
			origin 0 -1.5 0
			angles 0 0 0
			pivot 0 -1.5 0
			param 1 1 3
			limits 0 0 0
			scale 0.7 1.9 0.6
			type sphere
			{
				radius 1.0 
			}
	
//type box
//			{
//				depth 1
//				width 1
//				height 3
//			}   
			object rightFoot
			{
				origin 0 -1.5 0.5 
				angles 0 0 0
				pivot 0 0 0
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
}




