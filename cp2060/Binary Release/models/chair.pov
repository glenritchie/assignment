object chair
{
	origin 0 0 0
	//angles 0 0 0
	object seat
	{
		//angles 0 0 0
		//pivot 0 0 0
		//limits 0 0 0
		//scale 1.0 1 1
		origin 0 4 0
		type box
		{
			depth 3
			height 0.2
			width 3
		}
		object frontLeftChairLeg
		{
			origin 1.3 -2 1.3
			pivot 0 0 0
			type box
			{
				depth 0.2 
				height 4
				width 0.2
			}
		}
		object frontRightChairLeg
		{
			origin -1.3 -2 1.3
			pivot 0 0 0
			type box
			{
				depth 0.2 
				height 4
				width 0.2
			}
		}
		object backLeftChairLeg
		{
			origin 1.3 -2 -1.3
			pivot 0 0 0
			type box
			{
				depth 0.2 
				height 4
				width 0.2
			}
		}
		object backRightChairLeg
		{
			origin -1.3 -2 -1.3
			pivot 0 0 0
			type box
			{
				depth 0.2 
				height 4
				width 0.2
			}
		}
		object chairBack 
		{
			origin 0 2 -1.4
			pivot 0 0 0
			type box
			{
				depth 0.2 
				height 4
				width 3 
			}
		}
	}
}
