// A box, depth(z), width(x), height(y)
object body
{
	origin 0 8 0
	angles 0 0 0
	pivot 0 0 0
	param 0.2 0.2 4
	limits 0 0 0
	type box
	object neck
	{
		origin 0 2 0 
		angles 0 0 0
		pivot 0 0 0
		param 0.2 0.2 1
		limits 0 0 0
		type box
		object head
		{
			origin 0 1.5 0
			angles 0 0 0
			pivot 0 0 0
			param 1.1 10 10
			limits 0 0 0
			type sphere 
			object nose
			{
				origin 0.0 -0.1 1.4
				angles 0 0 0
				pivot 0 0 0
				param 0.7 0.1 0.1
				limits 0 0 0
				type  box
			}
			object leftEye
			{
				origin -0.3 0.2 1.0
				angles 0 0 0
				pivot 0 0 0
				param 0.2 6 6
				limits 0 0 0
				type sphere 
			}
			object rightEye
			{
				origin 0.3 0.2 1.0
				angles 0 0 0
				pivot 0 0 0
				param 0.2 6 6
				limits 0 0 0
				type sphere 
			}
		}
	}
	object upperLeftArm
	{
		origin 2.0 1.75 0
		angles 0 0 0 
		pivot 0 -1 0 
		param 0.2 0.2 2 
		limits 0 0 0
		type box
		object lowerLeftArm
		{
			origin 0 -1.25 0
			angles 0 0 0 
			pivot 0 -1 0 
			param 0.2 0.2 2.5 
			limits 0 0 0
			type box
			object leftHand
			{
				origin 0 -0.75 0
				pivot 0 -1.0 0 
				param 0.1 0.1 1.0
				type box
			}
		}
	}
	object upperRightArm
	{
		origin -2.0 1.75 0
		angles 0 0 0 
		pivot 0 -1 0 
		param 0.2 0.2 2 
		limits 0 0 0
		type box
		object lowerRightArm
		{
			origin 0 -1.25 0
			angles 0 0 0 
			pivot 0 -1 0 
			param 0.2 0.2 2.5 
			limits 0 0 0
			type box
			object rightHand
			{
				origin 0 -0.75 0
				pivot 0 -1.0 0 
				param 0.1 0.1 1.0
				type box
			}
		}
	}
	object upperLeftLeg
	{
			origin 1 -1.75 0
			angles 0 0 0
			pivot 0 -1.5 0
			param 0.2 0.2 3
			limits 0 0 0
			type box
			object lowerLeftLeg
			{
				origin 0 -1.5 0
				angles 0 0 0
				pivot 0 -1.5 0
				param 0.2 0.2 3 
				limits 0 0 0
				type box
				object leftFoot
				{
					origin 0 0 0.5 
					angles 0 0 0
					pivot 0 -1.5 0
					param 2 0.1 0.1 
					limits 0 0 0
					type box
				}
		}
	}
	object upperRightLeg
	{
		origin -1 -1.75 0
		angles 0 0 0
		pivot 0 -1.5 0
		param 0.2 0.2 3
		limits 0 0 0
		type box
		object lowerRightLeg
		{
			origin 0 -1.5 0
			angles 0 0 0
			pivot 0 -1.5 0
			param 0.2 0.2 3
			limits 0 0 0
			type box
			object rightFoot
			{
				origin 0 -1.5 0.5 
				angles 0 0 0
				pivot 0 0 0
				param 2 0.1 0.1 
				limits 0 0 0
				type box
			}
		}
	}
}




