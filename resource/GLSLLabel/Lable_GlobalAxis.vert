#version 330 core
layout (location = 0) in float aHighlightState;		//高亮状态
layout (location = 1) in float aShowState;	//显隐状态
layout (location = 2) in float aDepthState;	//深度测试状态
layout (location = 3) in float aSize;	//大小
layout (location = 4) in vec3 aColor;	//颜色RGB
layout (location = 5) in vec3 aVertex;	//顶点坐标（通用坐标系下）
layout (location = 6) in vec3 aPosition;//位置坐标（像素坐标系下）pos为屏幕左下角为原点 向上为y轴正方形 向右为x轴正方形 的像素坐标
layout (location = 7) in vec3 aDirection; //箭头指向


out vec3 Color;
out float ShowState;
out float Depth;
out float isDepth; //是否进行深度测试

//通用视角下的PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;

//屏幕尺寸
uniform float uScr_width;
uniform float uScr_height;
//当前视景体尺寸
uniform float uPers_width;
uniform float uPers_height;
//当前视点状态下的X轴Y轴Z轴
uniform vec3 uAuxX_new;
uniform vec3 uAuxY_new;
uniform vec3 uAuxZ_new;

mat4 translate(float x,float y, float z);//平移
mat4 scale(float x, float y, float z);//缩放
mat4 rotate(float x, float y, float z, float angle);//旋转
vec3 GetGlobleAxisPosition(int x, int y);//获取全局坐标轴在屏幕左下角显示的位置 x y 为屏幕左下角为原点的像素坐标

void main()
{
	ShowState = aShowState;
	//传递颜色
	if(int(aHighlightState)==1)
	{
		Color = vec3(255,255,255);//白色高亮
		isDepth = 0;
	}
	else
	{
		Color = aColor;
		isDepth = aDepthState;
	}
	mat4 model;
	//计算位置
	vec3 pos_c = GetGlobleAxisPosition(int(aPosition.x),int(aPosition.y));//将坐标轴放在左下角 75X75像素处

	//根据值设定箭头的方向
	float angle = degrees(acos(dot(vec3(1,0,0),aDirection)/length(aDirection)));//获取旋转角度
	if(abs(angle)<0.1) //判断夹角为0
	{
		model = uModel_c * translate(pos_c.x,pos_c.y,pos_c.z);
	}	
	else if((abs(angle)-180<0.1) && (abs(angle)-180>-0.1))//判断夹角为180
	{
		vec3 axisRotate = vec3(0,0,1);				//获取旋转轴
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle);
	}
	else
	{
		vec3 axisRotate = normalize(cross(vec3(1,0,0),aDirection));				//获取旋转轴
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle);
	}
	model *=scale(0.8*aSize,0.8*aSize,0.8*aSize);

	gl_Position = uProjection_c * uView_c * model * vec4(aVertex, 1.0f);
	
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

 vec3 GetGlobleAxisPosition(int x, int y)
 {
		//旋转后为保证坐标轴显示在屏幕左下角需要移动的坐标。
		return vec3(-uAuxX_new * (0.5*uPers_width - x / uScr_width * uPers_width) - uAuxY_new * (0.5*uPers_height - y / uScr_height * uPers_height));		
 }
