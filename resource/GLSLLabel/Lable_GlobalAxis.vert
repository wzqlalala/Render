#version 330 core
layout (location = 0) in float aHighlightState;		//����״̬
layout (location = 1) in float aShowState;	//����״̬
layout (location = 2) in float aDepthState;	//��Ȳ���״̬
layout (location = 3) in float aSize;	//��С
layout (location = 4) in vec3 aColor;	//��ɫRGB
layout (location = 5) in vec3 aVertex;	//�������꣨ͨ������ϵ�£�
layout (location = 6) in vec3 aPosition;//λ�����꣨��������ϵ�£�posΪ��Ļ���½�Ϊԭ�� ����Ϊy�������� ����Ϊx�������� ����������
layout (location = 7) in vec3 aDirection; //��ͷָ��


out vec3 Color;
out float ShowState;
out float Depth;
out float isDepth; //�Ƿ������Ȳ���

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
vec3 GetGlobleAxisPosition(int x, int y);//��ȡȫ������������Ļ���½���ʾ��λ�� x y Ϊ��Ļ���½�Ϊԭ�����������

void main()
{
	ShowState = aShowState;
	//������ɫ
	if(int(aHighlightState)==1)
	{
		Color = vec3(255,255,255);//��ɫ����
		isDepth = 0;
	}
	else
	{
		Color = aColor;
		isDepth = aDepthState;
	}
	mat4 model;
	//����λ��
	vec3 pos_c = GetGlobleAxisPosition(int(aPosition.x),int(aPosition.y));//��������������½� 75X75���ش�

	//����ֵ�趨��ͷ�ķ���
	float angle = degrees(acos(dot(vec3(1,0,0),aDirection)/length(aDirection)));//��ȡ��ת�Ƕ�
	if(abs(angle)<0.1) //�жϼн�Ϊ0
	{
		model = uModel_c * translate(pos_c.x,pos_c.y,pos_c.z);
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

 vec3 GetGlobleAxisPosition(int x, int y)
 {
		//��ת��Ϊ��֤��������ʾ����Ļ���½���Ҫ�ƶ������ꡣ
		return vec3(-uAuxX_new * (0.5*uPers_width - x / uScr_width * uPers_width) - uAuxY_new * (0.5*uPers_height - y / uScr_height * uPers_height));		
 }
