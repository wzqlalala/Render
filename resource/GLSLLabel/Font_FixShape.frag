#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Color;
in float ShowState;
in float Depth;
in float IsDepth;


uniform sampler2D Texture;



void main()
{

	if(int(ShowState) != 1)
	{
		discard;
	}
	else
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Texture, TexCoord).r);
		
		FragColor = vec4(Color.x/255.0,Color.y/255.0,Color.z/255.0,1.0)*sampled;

		if(IsDepth ==1)
		{
			//手动修改深度值，使其与添加连接点的深度值保持一致
			gl_FragDepth = Depth;
		}
		else
		{
			gl_FragDepth = 0;
		}
	}
      
}

