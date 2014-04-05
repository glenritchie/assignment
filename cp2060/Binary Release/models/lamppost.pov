object base
{
	origin 0 0 0 
	type box
	{
		width 1.5 
		height 0.1
		depth 1.5
	}
	object tube	
	{
		origin 0 0 0 
		pivot 0 0 0 
		angles 90 0 0
		type cylinder
		{
			height 5 
			topRadius 0.2
			baseRadius 0.2
		}
		object shade
		{
			origin 0 5 0
			pivot 0 0 0 
			angles 90 0 0
			type cylinder
			{
				height 1.5 
				topRadius 1
				baseRadius 0.2
			}
		}
		object shadeTop
		{
			lighted 0
			origin 0 7 0 
			angles 0 0 0 
			type sphere
			{
				radius 1
			}
		}
		object bulblight
		{
			origin 0 0 7 
			type light
			{
				redf 1.0	
				greenf 1.0
				bluef 1.0
			}
		}
	}
}
