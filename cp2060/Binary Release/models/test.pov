object World
{
	object barrier
	{
		texture tsl_wire2.tga
		origin -15 10 -25
		angles 0 45 0
		type box
		{
			faceTexScale2 3 1
			faceTexShift2 0.5 0 0
			faceTexScale3 3 1
			faceTexShift3 0.5 0 0
			faceTexShift0 0 0 0
			faceTexScale0 0.1 0.1 1
			faceTexScale4 0.1 0.1 1
			density 4
			width 2
			height 20
			depth 50
		}
	}
	object barrier2
	{
		texture tsl_wire2.tga
		origin 10 3.5 20
		angles 0 45 0
		type box
		{
			faceTexScale2 3 1
			faceTexShift2 0.5 0 0
			faceTexScale3 3 1
			faceTexShift3 0.5 0 0
			faceTexShift0 0 0 0
			faceTexScale0 0.1 0.1 1
			faceTexScale4 0.1 0.1 1
			density 4
			width 2
			height 7
			depth 4
		}
	}


	
	object ground
	{
		origin 0 -0.5 0
		texture carpet1.tga
		type box
		{
			density 10 
			faceTexScale4 1 1
			width 100
			height 1
			depth 100
		}
	}
	object robotA 
	{
		origin 8.46 8 -7.88
		type file
		{
			filename models/robot2.pov
			idleframes models/walk.frm
		}
	}
	object bulblight
	{
		origin 0 9 0
		type light
		{
			redf 1.0	
			greenf 1.0
			bluef 1.0
		}
	}
	object bulblight2
	{
		origin -20 9 30
		type light
		{
			redf 1.0	
			greenf 1.0
			bluef 1.0
		}
	}





}

