#version 330 core

layout (location = 0) in float aHighlightState;		//����״̬
layout (location = 1) in float aShowState;	//����״̬
layout (location = 2) in float aDepthState;	//��Ȳ���״̬
layout (location = 3) in float aSize;	//��С
layout (location = 4) in vec3 aColor;	//��ɫRGB
layout (location = 5) in vec3 aVertex;	//�������꣨ͨ������ϵ�£�
layout (location = 6) in vec3 aPosition;//λ�����꣨ģ������ϵ�£�
layout (location = 7) in vec4 aRotate; //��ת�����ת�Ƕ� 

out vec3 Color;
out float ShowState;
out float Depth;
out float isDepth; //�Ƿ������Ȳ���

//ͨ���ӽ��µ�PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;

//ģ���ӽ��µ�PVM
uniform mat4 uModel_m;
uniform mat4 uView_m;
uniform mat4 uProjection_m;


mat4 translate(float x,float y, float z);//ƽ��
mat4 scale(float x, float y, float z);//����
mat4 rotate(float x, float y, float z, float angle);//��ת
vec3 ModelToNormPositon(vec3 modelPos,mat4 projection,mat4 view,mat4 model);//ģ������ת��Ϊ��׼����
vec3 NormToModelPositon(vec3 normPos,mat4 projection,mat4 view,mat4 model);//��׼����ϵת��Ϊģ������ϵ

void main()
{
	ShowState = aShowState;
	//������ɫ
	if(int(aHighlightState) == 1)
	{
		Color = vec3(255,255,255);//��ɫ����
		isDepth = 0;
	}
	else
	{
		Color = aColor;
		isDepth = int(aDepthState);		
	}
	mat4 model;
	//���غ���ӵ��ģ������ϵת����ͨ������ϵ������
	vec3 pos_norm = ModelToNormPositon(aPosition,uProjection_m,uView_m,uModel_m);
	Depth = 0.5+pos_norm.z*0.5-0.01;  //������ֵ �ü�ͷ�����ֵ����ӵ�����ֵ����һ��
	vec3 pos_c = NormToModelPositon(pos_norm,uProjection_c,uView_c,uModel_c);

	//��ȡ��ת����
	if(length(vec3(aRotate.x,aRotate.y,aRotate.z))== 0) //�����Ϊ0 ��ô����ת
	{
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*scale(0.8,0.8,0.8);
	}
	else
	{
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(aRotate.x,aRotate.y,aRotate.z,aRotate.w)*scale(0.8,0.8,0.8);
	}
	gl_Position = uProjection_c * uView_c * model* vec4(aVertex, 1.0f);
	
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

mat4 rotate(float x, float y, float z, float angle)	//angleΪ�Ƕ�
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
