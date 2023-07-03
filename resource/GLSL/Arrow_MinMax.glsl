#version 330 core
#ifdef vertex_shader
	
#include "extfunction.glsl"
layout (location = 0) in vec3 aPos;		//顶点坐标（通用坐标系下）
layout (location = 1) in vec3 aPointPos;//位置坐标（模型坐标系下）
layout (location = 2) in vec3 aDirection; //方向
layout (location = 3) in float aDepthState;	//深度测试状态

out float Value;
out float Depth;
//通用视角下的PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;

//模型视角下的PVM
uniform mat4 uModel_m;
uniform mat4 uView_m;
uniform mat4 uProjection_m;

uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;//是否等差(1为等差,0为等比)
uniform float textureCoordRatio;//纹理坐标的系数


void main()
{
	//传递颜色
	Value = length(aDirection);
	// aDirection = normalize(aDirection);
	
	mat4 model;
	//将载荷添加点从模型坐标系转化到通用坐标系下坐标
	vec3 pos_norm = ModelToNormPositon(aPointPos,uProjection_m,uView_m,uModel_m);
	Depth = aDepthState == 0 ? 0.0 : 0.5+pos_norm.z*0.5-0.01;	
	vec3 pos_c = NormToModelPositon(pos_norm,uProjection_c,uView_c,uModel_c);

	float aSize = 1.0 + (isEquivariance == 0 ? log2(Value/minValue) / log2(maxValue/minValue) : (Value-minValue) / (maxValue-minValue)) * textureCoordRatio;

	//根据值设定箭头的方向
	float angle = degrees(acos(dot(vec3(1,0,0),aDirection)/length(aDirection)));//获取旋转角度
	if(abs(angle)<0.1) //判断夹角为0
	{
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*scale(0.3*aSize,0.3*aSize,0.3*aSize);
	}	
	else if((abs(angle)-180<0.1) && (abs(angle)-180>-0.1))//判断夹角为180
	{
		vec3 axisRotate = vec3(0,0,1);				//获取旋转轴
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle)*scale(0.2*aSize,0.2*aSize,0.2*aSize);
	}
	else
	{
		vec3 axisRotate = normalize(cross(vec3(1,0,0),aDirection));				//获取旋转轴
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle)*scale(0.2*aSize,0.2*aSize,0.2*aSize);
	}

	gl_Position = uProjection_c * uView_c * model* vec4(aPos, 1.0f);
	
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

uniform sampler1D Texture;

in float Value;
in float Depth;

uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;//是否等差(1为等差,0为等比)
uniform float textureCoordRatio;//纹理坐标的系数


void main()
{	
	vec3 Color = vec3(texture(Texture, (isEquivariance == 0 ? log2(Value/minValue) / log2(maxValue/minValue) : (Value-minValue) / (maxValue-minValue)) * textureCoordRatio));
	FragColor = vec4(Color, 1.0);	 
	//手动修改深度值，使其与添加载荷边界点的深度值保持一致
	gl_FragDepth = Depth;
}
#endif