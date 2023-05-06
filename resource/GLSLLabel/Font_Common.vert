#version 330 core

layout (location = 0) in float aHighlightState;		//高亮状态
layout (location = 1) in float aShowState;	//显隐状态
layout (location = 2) in float aDepthState;	//深度测试状态
layout (location = 3) in float aSize;	//大小
layout (location = 4) in vec3 aColor;	//颜色RGB
layout (location = 5) in vec3 aVertex;	//顶点坐标
layout (location = 6) in vec2 aTexCoord;//文字纹理坐标
layout (location = 7) in vec3 aPosition;//位置坐标（模型坐标系下）

out vec2 TexCoord;		//纹理坐标
out vec3 Color;			//颜色
out float ShowState;	//显隐状态
out float Depth;		//深度值
out float IsDepth;		//是否进行深度测试


//传入自适应分辨率比例
uniform float uRatio;
//像素坐变换矩阵
uniform mat4 uProjection_font;
uniform mat4 uView_font;
uniform mat4 uModel_font;

//模型坐标系变换矩阵
uniform mat4 uProjection_m;
uniform mat4 uView_m;
uniform mat4 uModel_m;

//屏幕尺寸
uniform float uScr_width;
uniform float uScr_height;

mat4 translate(float x,float y, float z);//平移
mat4 scale(float x, float y, float z);//缩放
mat4 rotate(float x, float y, float z, float angle);//旋转
vec3 ModelToNormPositon(vec3 uModelPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f);//模型坐标转化为标准坐标
vec3 NormToModelPositon(vec3 normPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f);//标准坐标系转化为模型坐标系

void main()
{
	ShowState = aShowState;
	//传递颜色
	if(int(aHighlightState)==1)
	{
		Color = vec3(255,255,255);//白色高亮
		IsDepth = 0;
	}
	else
	{
		Color = aColor;
		IsDepth = aDepthState;
	}
	
	TexCoord = vec2(aTexCoord.x/1024.0,aTexCoord.y/1024.0);  //1024为纹理的像素大小

	//将载荷添加点从模型坐标系转化到通用坐标系下坐标
	vec3 pos_norm = ModelToNormPositon(aPosition,uProjection_m,uView_m,uModel_m);
	//文字向右上移动一定距离 防止与图标重叠
	pos_norm = vec3(pos_norm.x+0.3*aSize*uRatio*0.03,pos_norm.y+0.3*aSize*uRatio*0.03,pos_norm.z);
	Depth = 0.5+pos_norm.z*0.5-0.1;  //输出深度值 让文字的深度值和载荷添加点的深度值保持一致
	//Depth = 0.5+pos_norm.z*0.5;  //输出深度值 让文字的深度值和载荷添加点的深度值保持一致

	vec3 pos_font = NormToModelPositon(pos_norm,uProjection_font,uView_font,uModel_font);
	mat4 model;
	model = translate(pos_font.x,pos_font.y,pos_font.z)*scale(0.3*aSize*uRatio,0.3*aSize*uRatio,0.3*aSize*uRatio);  //调整文字大小

	gl_Position = uProjection_font * uView_font * model * vec4(aVertex,1);

}
mat4 translate(float x,float y, float z)
{
	 return mat4(
	 1,0,0,0,
	 0,1,0,0,
	 0,0,1,0,
	 x,y,z,1
	 );
}

mat4 scale(float x, float y, float z)
{
	 return mat4(
	 x,0,0,0,
	 0,y,0,0,
	 0,0,z,0,
	 0,0,0,1
	 );
}

mat4 rotate(float x, float y, float z, float angle)	//angle为角度
{
	 float cos0 = cos(radians(-angle));
	 float sin0 = sin(radians(-angle));
	 float t = 1.0f - cos0;
	 float txx = t * x * x;
	 float txy = t * x * y;
	 float txz = t * x * z;
	 float tyy = t * y * y;
	 float tyz = t * y * z;
	 float tzz = t * z * z;
	 float sinx = sin0 * x;  float siny = sin0 * y;  float sinz = sin0 * z;    
	 return mat4(  txx+cos0,txy-sinz,txz+siny,0,  
				   txy+sinz,tyy+cos0,tyz-sinx,0,  
				   txz-siny,tyz+sinx,tzz+cos0,0,  
				   0,       0,       0,       1  );
 }

 vec3 ModelToNormPositon(vec3 uModelPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f)
 {
	vec3 pos_norm = (uProjection_f * (uView_f * (uModel_f*vec4(uModelPos_f, 1.0)))).xyz;
	return pos_norm;
 }
 
 vec3 NormToModelPositon(vec3 normPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f)
 {
	vec3 pos_uModel = (inverse(uModel_f)*(inverse(uView_f)*(inverse(uProjection_f)*vec4(normPos_f, 1.0)))).xyz;
	return pos_uModel;
 }