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
	object box1
	{
		texture tsl_wire2.tga
		origin 10 5 20
		angles 0 45 0
		type box
		{
			faceTexScale0 1 1 
			faceTexScale1 1 1 
			faceTexScale2 1 1 
			faceTexScale3 1 1 
			faceTexScale4 1 1 
			faceTexScale5 1 1 
			faceTexShift0 0.5 0 0
			faceTexShift1 0.5 0 0
			faceTexShift2 0.5 0 0
			faceTexShift3 0.5 0 0
			faceTexShift4 0.5 0 0
			faceTexShift5 0.5 0 0

			density 4
			width 10
			height 10
			depth 10
		}
	}
	object box2
	{
		texture tsl_wire2.tga
		origin 17 15 18
		angles 0 45 0
		type box
		{
			faceTexScale0 1 1 
			faceTexScale1 1 1 
			faceTexScale2 1 1 
			faceTexScale3 1 1 
			faceTexScale4 1 1 
			faceTexScale5 1 1 
			faceTexShift0 0.5 0 0
			faceTexShift1 0.5 0 0
			faceTexShift2 0.5 0 0
			faceTexShift3 0.5 0 0
			faceTexShift4 0.5 0 0
			faceTexShift5 0.5 0 0

			density 4
			width 10
			height 10
			depth 10
		}
	}
	object box4
	{
		texture tsl_wire2.tga
		origin 20 5 2
		angles 0 0 0
		type box
		{
			faceTexScale0 1 1 
			faceTexScale1 1 1 
			faceTexScale2 1 1 
			faceTexScale3 1 1 
			faceTexScale4 1 1 
			faceTexScale5 1 1 
			faceTexShift0 0.5 0 0
			faceTexShift1 0.5 0 0
			faceTexShift2 0.5 0 0
			faceTexShift3 0.5 0 0
			faceTexShift4 0.5 0 0
			faceTexShift5 0.5 0 0

			density 4
			width 10
			height 10
			depth 10
		}
	}
	object box4
	{
		texture tsl_wire2.tga
		origin 22 15 4
		angles 0 15 0
		type box
		{
			faceTexScale0 1 1 
			faceTexScale1 1 1 
			faceTexScale2 1 1 
			faceTexScale3 1 1 
			faceTexScale4 1 1 
			faceTexScale5 1 1 
			faceTexShift0 0.5 0 0
			faceTexShift1 0.5 0 0
			faceTexShift2 0.5 0 0
			faceTexShift3 0.5 0 0
			faceTexShift4 0.5 0 0
			faceTexShift5 0.5 0 0

			density 4
			width 10
			height 10
			depth 10
		}
	}


	


	object box3
	{
		texture tsl_wire2.tga
		origin 22 5 14
		angles 0 20 0
		type box
		{
			faceTexScale0 1 1 
			faceTexScale1 1 1 
			faceTexScale2 1 1 
			faceTexScale3 1 1 
			faceTexScale4 1 1 
			faceTexScale5 1 1 
			faceTexShift0 0.5 0 0
			faceTexShift1 0.5 0 0
			faceTexShift2 0.5 0 0
			faceTexShift3 0.5 0 0
			faceTexShift4 0.5 0 0
			faceTexShift5 0.5 0 0

			density 4
			width 10
			height 10
			depth 10
		}
	}

	object gaurdpost 
	{
		origin -15 0 40
		type file
		{
			filename models/post1.pov
		}
	}

	object ground
	{
		origin 0 -0.5 0
		texture carpet1.tga
		type box
		{
			density 10 
			faceTexScale4 10 10
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
		origin -20 9 40
		type light
		{
			redf 1.0	
			greenf 1.0
			bluef 1.0
		}
	}
	object bulblight3
	{
		origin 20 2 30
		type light
		{
			redf 1.0	
			greenf 1.0
			bluef 1.0
		}
	}


	object barrierA
	{
		texture tsl_wire2.tga
		origin  27 10 -7.5
		angles 0 90 0
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
























}

