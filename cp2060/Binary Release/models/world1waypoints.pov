object points
{
	object chairwaypoint
	{
		origin -5 0 -3
		lighted 0
		type waypoint
		{
			connect waypoint5
			action models/sit.frm
			postAction models/sitidle.frm
			actionAngle 0
		}
	}
	object waypoint2
	{
		origin -40 0 0
		lighted 0
		type waypoint
		{
			connect waypoint3
		}
	}
	object waypoint3
	{
		origin -40 0 20 
		lighted 0
		type waypoint
		{
			connect waypoint2
			connect waypoint4
		}
	}
	object waypoint4
	{
		origin 20 0 20 
		lighted 0
		type waypoint
		{
			connect waypoint3
			connect waypoint5
		}
	}
	object waypoint5
	{
		origin 20 0 0
		lighted 0
		type waypoint
		{
			connect waypoint4
			connect chairwaypoint
		}
	}

}
