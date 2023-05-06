#version 330 core
layout (location = 0) in float aHighlightState;		//高亮状态
layout (location = 1) in float aShowState;	//显隐状态
layout (location = 2) in float aDepthState;	//深度测试状态
layout (location = 3) in float aSize;	//大小
layout (location = 4) in vec3 aColor;	//颜色RGB
layout (location = 5) in vec3 aVertex;	//顶点坐标（通用坐标系下）
layout (location = 6) in vec3 aPosition;//位置坐标（模型坐标系下）
layout (location = 7) in vec3 aPlaneNormVec;  //平面法向量
layout (location = 8) in vec3 aArrowVec;//箭头指向

out vec3 Color;
out float ShowState;
out float Depth;
out float isDepth; //是否进行深度测试

//通用视角下的PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;

//模型视角下的PVM
uniform mat4 uModel_m;
uniform mat4 uView_m;
uniform mat4 uProjection_m;

mat4 translate(float x,float y, float z);//平移
mat4 scale(float x, float y, float z);//缩放
mat4 rotate(float x, float y, float z, float angle);//旋转
vec3 ModelToNormPositon(vec3 modelPos,mat4 projection,mat4 view,mat4 model);//模型坐标转化为标准坐标
vec3 NormToModelPositon(vec3 normPos,mat4 projection,mat4 view,mat4 model);//标准坐标系转化为模型坐标系

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
	mat4 model = uModel_c;
	vec3 iniPlaneNormVec = vec3(0,0,1);
	vec3 iniArrowVec = vec3(1,0,0);
	//旋转对应法向量
	float angle = degrees(acos(dot(iniPlaneNormVec,aPlaneNormVec)/length(iniPlaneNormVec)/length(aPlaneNormVec)));//获取旋转角度
	if(abs(angle)>0.1 && (abs(abs(angle)-180)>0.1)) //除法向量同向或者反向之外的其他情况
	{
		vec3 axisRotate = normalize(cross(vec3(0,0,1),aPlaneNormVec));	 
		model = rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle) * model;
		iniArrowVec = (model * vec4(iniArrowVec,1.0)).xyz;
	}
	//旋转对于箭头方向
	float angle2 = degrees(acos(dot(iniArrowVec,aArrowVec)/length(iniArrowVec)/length(aArrowVec)));//获取旋转角度
	if((abs(abs(angle2)-180)<0.1))//判断夹角为180
	{
		vec3 axisRotate = aPlaneNormVec;				//获取旋转轴
		model = rotate(axisRotate.x,axisRotate.y,axisRotate.z,180) * model;
	}
	else if(abs(angle2)>0.1)//夹角不为0
	{	
		vec3 axisRotate = normalize(cross(iniArrowVec,aArrowVec));	 
		model = rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle2) * model;
	}

	//最后平移到指定位置
	vec3 pos_norm = ModelToNormPositon(aPosition,uProjection_m,uView_m,uModel_m);
	Depth = 0.5+pos_norm.z*0.5-0.01;  //输出深度值 深度值和添加点的深度值保持一致
	vec3 pos_c = NormToModelPositon(pos_norm,uProjection_c,uView_c,uModel_c);
	model = translate(pos_c.x,pos_c.y,pos_c.z) *scale(0.6,0.6,0.6)* model;

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

 vec3 ModelToNormPositon(vec3 modelPos,mat4 projection,mat4 view,mat4 model)
 {
	vec3 pos_norm = (projection * (view * (model*vec4(modelPos, 1.0)))).xyz;
	return pos_norm;
 }
 
 vec3 NormToModelPositon(vec3 normPos,mat4 projection,mat4 view,mat4 model)
 {
	vec3 pos_model = (inverse(model)*(inverse(view)*(inverse(projection)*vec4(normPos, 1.0)))).xyz;
	return pos_model;
 }
