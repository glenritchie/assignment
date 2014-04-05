// 0- Near Face - Facing z+ 1
// 1- Far face - Facing z -1
// 2- Left	face -x
// 3- Right Face +x
// 4- Top Face +y	
// 5- Bottom Face -y	

object World
{
	object barrier
	{
		texture tsl_wire2.tga
		origin -15 10 -25
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
		origin -15 10 35
		type box
		{
			faceTexScale2 1 1
			faceTexShift2 0.5 0 0
			faceTexScale3 1 1
			faceTexShift3 0.5 0 0
			faceTexShift0 0 0 0
			faceTexScale0 0.1 0.1 1
			faceTexScale4 0.1 0.1 1
			density 4
			width 2
			height 20
			depth 30
		}
	}
	object upperSlopedbarrier
	{
		texture tsl_wire2.tga
		origin -26 30 0
		angles 0 0 45 
		pivot 0 0 0
		type box
		{
			faceTexScale2 7 1
			faceTexShift2 0.5 0 0
			faceTexScale3 7 1
			faceTexShift3 0.5 0 0
			faceTexShift0 0 0 0
			faceTexScale0 0.1 0.1 1
			faceTexScale4 0.1 0.1 1
			density 2
			width 2
			height 30
			depth 100
		}
	}
	object ground
	{
		origin 0 -0.5 0
//		origin 0 0 0
		texture carpet1.tga
		type box
		{
			density 5
//			faceTexScale4 15 15
			width 110
			height 1
			depth 110
		}
	}


//	object ground
//	{
//		texture grass2.tga
//		type ground
//		{
//			height 100
//			width 100
//			density 5
//		}
//	}

	
	
	object world1stepsSouth
	{
		origin 43 1.25 20 
		type file
		{
			filename models/world1steps.pov
		}
	}
	object world1stepsNorth
	{
		origin 43 1.25 -20 
		type file
		{
			filename models/world1steps2.pov
		}
	}
	
	
	object platformSouth
	{
		origin 12 13 35
		texture carpet1.tga
		type box
		{
				faceTexScale4 1 5
				faceTexScale1 5 1
				width 52
				depth 30
				height 1.5
		}
	}
	object platformNorth
	{
		origin 12 13 -35
		texture carpet1.tga
		type box
		{
				faceTexScale4 1 5
				faceTexScale1 5 1
				width 52
				depth 30
				height 1.5
		}
	}
	object platformHolderSouth
	{
		origin 37.3 6.5 35
		texture carpet1.tga
		type box
		{
				faceTexScale4 1 5
				faceTexScale1 5 1
				width 1.5
				depth 30
				height 13
		}
	}


	object platformHolderNorth
	{
		origin 37.3 6.5 -35
		texture carpet1.tga
		type box
		{
				faceTexScale4 1 5
				faceTexScale1 5 1
				width 1.5
				depth 30
				height 13
		}
	}

	object robotA 
	{
		//origin -25 0 0
		
		//origin 5.38 0 0.44
		//origin 33.80 30 32.82
		origin 8.46 8 -7.88
		//origin -43.38 0 40.44
		//origin -24 0.3 0
        //type sphere
        //{
            //radius 1
        //}
		type file
		{
			filename models/robot2.pov
			//idleframes models/left_punch.frm
			//idleframes models/standing2.frm
			idleframes models/walk.frm
		}
	}
	object chairA
	{
		origin -5 0 -5
		type file
		{
			filename models/chair.pov
		}
	}
	object left
	{
		origin -50 20 0 
		//texture brickwall.tga
		texture tsl_wire2.tga
		type box
		{
			//faceTexScale2 5 1
			faceTexScale3 3 1 
			faceTexShift3 0.5 0 0
			//faceTexShift3 0 0 0
			//faceTexScale2 6 6
			density 10 
			width 2
			height 40
			depth 100
		}
	}
	object right
	{
		origin 50 20 0 
		//texture brickwall.tga
		texture tsl_wire2.tga
		type box
		{
			density 10
			faceTexScale2 3 1
			faceTexShift2 0.5 0 0
			 
			//faceTexScale2 6 6
			width 2
			height 40
			depth 100
		}
	}
	object south
	{
		origin 0 20 50
		//texture brickwall.tga
		texture tsl_wire2.tga
		angles 0 90 0
		//pivot
		type box
		{
			density 3 
			faceTexScale2 5 1
			faceTexScale3 3 2
			faceTexShift3 0.5 0 0
			//faceTexScale2 30 30
			width 1
			height 40
			depth 100
		}
	}
	object north
	{
		origin 0 20 -50
		texture tsl_wire2.tga
		angles 0 -90 0
		//pivot
		type box
		{
			//faceTexScale2 5 1
			faceTexScale3 3 2
			faceTexShift3 0.5 0 0
			//faceTexScale2 6 6
			density 5
			width 1
			height 40
			depth 100
		}
	}
	object vent
	{
		origin -40 39.5 0
		texture vent.tga
		type box
		{
			faceTexScale5 1 1 
			width 10
			height 1
			depth 10
		}
	}
	object bottomAltar
	{
		origin 45 2.5 0
		texture wbackground.tga
		type box
		{
	//		faceTexScale5 1 1 
			width 5 
			height 5 
			depth 5
		}
	}
		object topAltar
		{
			origin 45 5 0
			texture wbackground.tga
//		texture glen.tga
			type box
			{
	//		faceTexScale5 1 1 
				width 8 
				height 2 
				depth 8
			}
		}
	object glenPic
	{
		origin 49.3 20 0
		texture glen.tga
		type box
		{
			faceTexScale5 1 1 
			width 1
			height 10
			depth 10
		}
	}
/*	object glenPicText
	{
		origin 49.3 14 0
		texture yourstruly.tga
		//transparent 1
		//twosided 1
		//lighted 0
		type box
		{
			
			faceTexScale5 1 1 
			width 1
			height 2
			depth 10
		}
	}
*/
object thegamPic
	{
		origin 49.3 20 -35
		texture ready4battle.tga
		type box
		{
			faceTexScale5 1 1 
			width 1
			height 10
			depth 10
		}
	}

	object roof
	{
		origin 0 40 0
		texture brickwall.tga
		type box
		{
			faceTexScale5 10 10 
//			faceTexScale3 8 1
			//faceTexScale2 6 6
			width 110
			height 1
			depth 110
		}
	}
	object waypoints
	{
		type file
		{
			filename models/world1waypoints.pov
		}
	}
	object lampPostA
	{
		origin -46 0 46 
		type file
		{
			filename models/lamppost.pov
			idleframes models/lampidle.frm
		}
	}
	object lampPostC
	{
		origin -10 14 -45 
		type file
		{
			filename models/lamppost.pov
			idleframes models/lampidle.frm
		}
	}
	object lampPostC
	{
		origin -10 14 45 
		type file
		{
			filename models/lamppost.pov
			idleframes models/lampidle.frm
		}
	}
	object ball
	{
		origin 20 37.5 -5
		type sphere
		{
			radius 1
		}
	}
	
	object lampPostB
	{
		origin 46 0 -10
		type file
		{
			filename models/lamppost.pov
			idleframes models/lampidle.frm
		}
	}
/*
	object thing
	{
		origin 5 10 0
		angles 180 0 0
		texture light
		twosided 1
		type box
		{
//				faceTexScale4 1 5
//				faceTexScale1 5 1
				width 5
				depth 0
				height 5
		}
	}
*/
}
