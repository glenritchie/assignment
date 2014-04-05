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
			object shadeTop
			{
				//transparent 0.2
				//twosided 1
				lighted 0
//				texture spot1.tga
				origin 0 2 0   
				pivot 0 0 0 
				angles 0 0 0
				//scale 1 1 2
				type sphere
				{
					radius 1
				}
				object bulblight
				{
					origin 0 0 0
					type light
					{
						redf 0.4	
						greenf 1.0
						bluef 1.0
					}
				}
			}
		}
	}
}
