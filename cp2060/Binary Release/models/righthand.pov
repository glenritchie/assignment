object rightwrist
{
	origin 0 -1.5 0
	angles 0 0 0 
	pivot 0 0.5 0 
	type box
	{
		depth 0.5
		width 0.3
		height 0.5
	} 
	object rightlowerPinky
	{
		origin 0 -0.5 -0.20
		pivot 0 0.25 0
		type box
		{
			depth 0.1
			width 0.1
			height 0.2
		}
		object rightupperPinky
		{
			origin 0 -0.25 0
			pivot 0 0.125 0
			type box
			{
				depth 0.1
				width 0.1
				height 0.3
			}
		}
	}
	object rightlowerRing
	{
		origin 0 -0.4 -0.08
		pivot 0 0.2 0
		type box
		{
			depth 0.1
			width 0.1
			height 0.3
		}
		object rightupperRing
		{
			origin 0 -0.3 0
			pivot 0 0.15 0
			type box
			{
				depth 0.1
				width 0.1
				height 0.3
			}
		}
	}
	object rightlowerMiddle
	{
		origin 0 -0.4 0.04
		pivot 0 0.2 0
		type box
		{
			depth 0.1
			width 0.1
			height 0.37
		}
		object rightupperMiddle
		{
			origin 0 -0.37 0
			pivot 0 0.18 0
			type box
			{
				depth 0.1
				width 0.1
				height 0.37
			}
		}
	}
	object rightlowerIndex
	{
		origin 0 -0.4 0.17
		pivot 0 0.2 0
		type box
		{
			depth 0.1
			width 0.1
			height 0.35
		}
		object rightupperIndex
		{
			origin 0 -0.3 0
			pivot 0 0.17 0
			type box
			{
				depth 0.1
				width 0.1
				height 0.35
			}
		}
	}

	object rightlowerthumb
	{
		origin 0 -0 0.3
		pivot 0 -0.15 0
		angles 0 0 0
		type box
		{
			depth 0.1
			width 0.1
			height 0.3
		}
		object rightupperthumb
		{
			origin 0 -0.2 0
			pivot 0 0.2 0
			type box
			{
				depth 0.1
				width 0.1
				height 0.2
			}
		}
	}
}
