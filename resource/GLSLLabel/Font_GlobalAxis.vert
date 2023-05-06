#version 330 core

layout (location = 0) in float aHighlightState;		//����״̬
layout (location = 1) in float aShowState;	//����״̬
layout (location = 2) in float aDepthState;	//��Ȳ���״̬
layout (location = 3) in float aSize;	//��С
layout (location = 4) in vec3 aColor;	//��ɫRGB
layout (location = 5) in vec3 aVertex;	//��������
layout (location = 6) in vec2 aTexCoord;//������������
layout (location = 7) in vec3 aPosition;//λ�����꣨ģ������ϵ�£�
layout (location = 8) in float aArrowSize; //��ͷ��С
layout (location = 9) in vec3 aDirection; //����


out vec2 TexCoord;		//��������
out vec3 Color;			//��ɫ
out float ShowState;	//����״̬
out float Depth;		//���ֵ
flat out int IsDepth;		//�Ƿ������Ȳ���

//��������Ӧ�ֱ��ʱ���
uniform float uRatio;

//�����ӽ��µ�PVM
uniform mat4 uView_font;
uniform mat4 uProjection_font;
//ͨ���ӽ��µ�PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;
//��Ļ�ߴ�
uniform float uScr_width;
uniform float uScr_height;
//��ǰ�Ӿ���ߴ�
uniform float uPers_width;
uniform float uPers_height;

//��ǰ�ӵ�״̬�µ�X��Y��Z��
uniform vec3 uAuxX_new;
uniform vec3 uAuxY_new;
uniform vec3 uAuxZ_new;



mat4 translate(float x,float y, float z);//ƽ��
mat4 scale(float x, float y, float z);//����
mat4 rotate(float x, float y, float z, float angle);//��ת
vec3 uModelToNormPositon(vec3 uModelPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f);//ģ������ת��Ϊ��׼����
vec3 NormTouModelPositon(vec3 normPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f);//��׼����ϵת��Ϊģ������ϵ
vec3 GetGlobleAxisPosition(int x, int y);//��ȡȫ������������Ļ���½���ʾ��λ�� x y Ϊ��Ļ���½�Ϊԭ�����������

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
		IsDepth = int(aDepthState);
	}

	TexCoord = vec2(aTexCoord.x/1024.0,aTexCoord.y/1024.0);  //1024Ϊ��������ش�С
	mat4 model;
	vec3 pos_c = GetGlobleAxisPosition(int(aPosition.x),int(aPosition.y));
	
	//����ֵ�趨��ͷ�ķ���
	float angle = degrees(acos(dot(vec3(1,0,0),aDirection)/length(aDirection)));//��ȡ��ת�Ƕ�
	if(abs(angle) < 0.1) //�жϼн�Ϊ0
	{
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z);
	}
	else if((abs(angle)-180<0.1) && (abs(angle)-180>-0.1))//�жϼн�Ϊ180
	{
		vec3 axisRotate = vec3(0,0,1);				//��ȡ��ת��
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle);
	}
	else
	{
		vec3 axisRotate = normalize(cross(vec3(1,0,0),aDirection));				//��ȡ��ת��
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle);
	}
	
	//��ȡ���־����任����ͨ���ӽ��µ�����
	vec3 FontPos_c_beforeTrans = vec3(1.0*0.8*aArrowSize+0.2,0,0);	//���ֱ任ǰ��ͬͨ������ϵ�µ�����
	vec3 FontPos_c_afterTrans =  (model* vec4(FontPos_c_beforeTrans, 1.0f)).xyz; //���ֱ任����ͨ������ϵ�µ�����
	//��ȡ���־����任���ڱ�׼����ϵ������	
	vec3 normPos = (uProjection_c *uView_c * uModel_c*vec4(FontPos_c_afterTrans,1.0)).xyz;
	//������ӵ�����������ϵ�����꣨ԭ�������Ͻǣ�
	vec2 pixelPos;
	pixelPos.x = (normPos.x * uScr_width / 2 + uScr_width / 2);
	pixelPos.y = (-normPos.y * uScr_height / 2 + uScr_height / 2);
	
	mat4 model2;
	model2 = translate(pixelPos.x,uScr_height-pixelPos.y,0)*scale(0.3*uRatio*aSize,0.3*uRatio*aSize,0.3*uRatio*aSize);  //�������ִ�С
	gl_Position = uProjection_font * uView_font * model2 * vec4(aVertex,1);
	

    

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

 vec3 uModelToNormPositon(vec3 uModelPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f)
 {
	vec3 pos_norm = (uProjection_f * (uView_f * (uModel_f*vec4(uModelPos_f, 1.0)))).xyz;
	return pos_norm;
 }
 
 vec3 NormTouModelPositon(vec3 normPos_f,mat4 uProjection_f,mat4 uView_f,mat4 uModel_f)
 {
	vec3 pos_uModel = (inverse(uModel_f)*(inverse(uView_f)*(inverse(uProjection_f)*vec4(normPos_f, 1.0)))).xyz;
	return pos_uModel;
 }
  vec3 GetGlobleAxisPosition(int x, int y)
 {
	//��ת��Ϊ��֤��������ʾ����Ļ���½���Ҫ�ƶ������ꡣ
	return vec3(-uAuxX_new * (0.5*uPers_width - x / uScr_width * uPers_width) - uAuxY_new * (0.5*uPers_height - y / uScr_height * uPers_height));		
 }