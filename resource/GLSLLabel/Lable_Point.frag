#version 330 core
out vec4 FragColor;

in vec3 Color;
in float ShowState;
flat in int isDepth;

void main()
{	
	if(int(ShowState) != 1 || length(gl_PointCoord - vec2(0.5)) > 0.5)
	{
		discard;
	}
	else
	{
		 FragColor = vec4(Color.x/255.0,Color.y/255.0,Color.z/255.0,1.0);
		 if(isDepth==0)
		 {
			//高亮的时候深度值为0
			gl_FragDepth = 0;
		 }
		 else
		 {
			//手动修改深度值，使其与添加载荷边界点的深度值保持一致
			 gl_FragDepth = gl_FragCoord.z-0.01;
		 }
		
	}
	
} 