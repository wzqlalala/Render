#version 330 core

layout (location = 0) in float aHighlightState;		//高亮状态
layout (location = 1) in float aShowState;	//显隐状态
layout (location = 2) in float aDepthState;	//深度测试状态
layout (location = 3) in float aSize;	//大小（此参数无用）
layout (location = 4) in vec3 aColor;	//颜色RGB
layout (location = 5) in vec3 aVertex;	//顶点坐标（模型坐标系下）


out vec3 Color;
out float ShowState;
flat out int isDepth; //是否进行深度测试
//模型视角下的PVM
uniform mat4 uModel_m;
uniform mat4 uView_m;
uniform mat4 uProjection_m;


void main()
{
	ShowState = aShowState;
	//传递颜色
	if(int(aHighlightState) == 1)
	{
		Color = vec3(255,255,255);//白色高亮
		isDepth = 0;
	}
	else
	{
		isDepth = int(aDepthState);
		Color = aColor;
	}
	gl_Position = uProjection_m * uView_m * uModel_m* vec4(aVertex.x,aVertex.y,aVertex.z, 1.0f);

	
}

