#version 330 core
#ifdef vertex_shader
#include "extfunction.glsl"
layout (location = 0) in vec3 aColor;				//颜色RGB
layout (location = 1) in vec2 aVertex;				//纹理顶点坐标（像素坐标系下 每个纹理都在0,0点）
layout (location = 2) in vec2 aTexCoord;			//文字纹理坐标
layout (location = 3) in vec3 aPosition;//位置坐标（模型坐标系下）
layout (location = 4) in float aArrowSize; //箭头大小
layout (location = 5) in vec3 aDirection; //方向

out vec2 TexCoord;		//纹理坐标
out vec3 Color;			//颜色

//像素坐变换矩阵
uniform mat4 uProjection_font;
uniform mat4 uView_font;
uniform mat4 uModel_font;

//模型坐标系变换矩阵
uniform mat4 uProjection_m;
uniform mat4 uView_m;
uniform mat4 uModel_m;

//通用视角下的PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;

//传入自适应分辨率比例
uniform float uRatio;

void main()
{
	Color = aColor;
	TexCoord = vec2(aTexCoord.x/1024.0,aTexCoord.y/1024.0);  //1024为纹理的像素大小

	mat4 model;
	vec3 position = GetGlobleAxisPosition(80,80);//将坐标轴放在左下角 80X80像素处

	//首先判断X轴方向
	float angle = degrees(acos(dot(vec3(1,0,0),aDirection)/length(aDirection)));//获取旋转角度
	if(abs(angle) < 0.1) //判断夹角为0
	{	
		model = uModel_c * translate(position.x,position.y,position.z)*scale(0.8,0.8,0.8);
	}
	//Y轴
	else if((abs(angle)-180<0.1) && (abs(angle)-180>-0.1))//判断夹角为180
	{
		vec3 axisRotate = vec3(0,0,1);
		model = uModel_c * translate(position.x,position.y,position.z)* rotate(axisRotate.x,axisRotate.y,axisRotate.z,90)*scale(0.8,0.8,0.8);
	}
	//Z轴
	else if(aType == 2)
	{
		vec3 axisRotate = vec3(0,1,0);
		model = uModel_c * translate(position.x,position.y,position.z)* rotate(axisRotate.x,axisRotate.y,axisRotate.z,-90)*scale(0.8,0.8,0.8);
	}

	//获取文字经过变换后在通用视角下的坐标
	vec3 FontPos_c_beforeTrans = vec3(1.0,0,0);	//文字变换前在同通用坐标系下的坐标
	vec3 FontPos_c_afterTrans =  (model* vec4(FontPos_c_beforeTrans, 1.0f)).xyz; //文字变换后在通用坐标系下的坐标
	//获取文字经过变换后在标准坐标系下坐标	
	vec3 normPos = (uProjection_c *uView_c * uModel_c*vec4(FontPos_c_afterTrans,1.0)).xyz;
	//文字添加点在像素坐标系下坐标（原点在左上角）
	vec2 pixelPos;
	pixelPos.x = (normPos.x * uScr_width / 2 + uScr_width / 2);
	pixelPos.y = (-normPos.y * uScr_height / 2 + uScr_height / 2);

	mat4 model_2;
	model_2 = translate(pixelPos.x,uScr_height-pixelPos.y,0)*scale(0.3*uRatio,0.3*uRatio,0.3*uRatio);  //调整文字大小
	gl_Position = uProjection_font * uView_font * model_2 * vec4(aVertex,0,1);

    

}
#endif

#ifdef fragment_shader

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Color;
uniform sampler2D Texture;
void main()
{

	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Texture, TexCoord).r);
	FragColor = vec4(Color,1.0)* sampled;
     
}
#endif




