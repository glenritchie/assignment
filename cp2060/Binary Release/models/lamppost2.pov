object base
{
	origin 0 0 0 
	type box
	{
		width 3 
		height 0.7
		depth 3
	}
	object bottomJoint
	{
		origin 0 0.5 0
		type sphere
		{
			radius 0.3
		}
		object lowertube	
		{
			pivot 0 5 0 
			angles 90 0 0
			type cylinder
			{
				height 5 
				topRadius 0.2
				baseRadius 0.2
			}
			object firstJoint
			{
				origin 0 5 0
				type sphere
				{
					radius 0.3
				}
				object middletube	
				{
					pivot 0 5 0 
					angles 90 0 0
					type cylinder
					{
						height 5 
						topRadius 0.2
						baseRadius 0.2
					}
					object middleJoint
					{
						origin 0 5 0
						type sphere
						{
							radius 0.3
						}
						object uppertube	
						{
							pivot 0 0 0 
							angles 90 0 0
							type cylinder
							{
								height 4 
								topRadius 0.2
								baseRadius 0.2
							}
							object secondJoint
							{
								origin 0 4 0
								type sphere
								{
									radius 0.3
								}
								object hood
								{
									twosided 1
									type cylinder
									{
										height 2
                                        slices 10
										topRadius 1
										baseRadius 0.2
									}
									object hoodShade
									{
			//							twosided 1
										//transparent 0.2
										lighted 0
                                        texture spot1.tga
										type cylinder
										{
											stacks 5 
											slices 10
											height 7
											topRadius 3.2 
											baseRadius 0.1
										}
									}
									object bulbbase
									{
										twosided 1
										type cylinder
										{
											height 1
											topRadius 0.2
											baseRadius 0.2
										}
									}
									object bulbtop
									{
										origin 0 0 1.0
										type sphere
										{
											radius 0.3
										}
										object bulblight
										{
											origin 0 0 0.4 
											type spotlight
											{
                                               // direction 0 -1 0
												redf 1.0
												greenf 0.0
												bluef 0.0
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
