#version 330 core

layout (location = 0) in float aHighlightState;		//����״̬
layout (location = 1) in float aShowState;	//����״̬
layout (location = 2) in float aDepthState;	//��Ȳ���״̬
layout (location = 3) in float aSize;	//��С
layout (location = 4) in vec3 aColor;	//��ɫRGB
layout (location = 5) in vec3 aVertex;	//��������
layout (location = 6) in vec2 aTexCoord;//������������
layout (location = 7) in vec3 aPosition;//��ĻOpenGL����ϵ�����꣨�� x��-1,1�� y��-1,1��֮�䣩 ��Ļ����Ϊԭ��

out vec2 TexCoord;		//��������
out vec3 Color;			//��ɫ
out float ShowState;	//����״̬
out float IsDepth;		//�Ƿ������Ȳ���

//��������Ӧ�ֱ��ʱ���
uniform float uRatio;
//�������任����
uniform mat4 uProjection_font;
uniform mat4 uView_font;
uniform mat4 uModel_font;

//��Ļ�ߴ�
uniform float uScr_width;
uniform float uScr_height;

mat4 translate(float x,float y, float z);//ƽ��
mat4 scale(float x, float y, float z);//����

vec3 ModelToNormPositon(vec3 uModelPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f);//ģ������ת��Ϊ��׼����
vec3 NormToModelPositon(vec3 normPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f);//��׼����ϵת��Ϊģ������ϵ

void main()
{
	ShowState = aShowState;
	//������ɫ
	if(int(aHighlightState)==1)
	{
		Color = vec3(255,255,255);//��ɫ����
		IsDepth = 0;
	}
	else
	{
		Color = aColor;
		IsDepth = aDepthState;
	}
	ShowState = aShowState;
	TexCoord = vec2(aTexCoord.x/1024.0,aTexCoord.y/1024.0);  //1024Ϊ��������ش�С
	mat4 Model;
	Model = translate(uScr_width/2+aPosition.x*uScr_width/2,uScr_height/2+aPosition.y*uScr_height/2,0)*scale(0.3*uRatio*aSize,0.3*uRatio*aSize,0.3*uRatio*aSize);  //�������ִ�С  ����������Ļ����Ϊԭ��
	gl_Position = uProjection_font * uView_font * Model * vec4(aVertex,1);
    

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