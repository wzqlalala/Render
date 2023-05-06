#version 330 core

layout (location = 0) in float aHighlightState;		//����״̬
layout (location = 1) in float aShowState;	//����״̬
layout (location = 2) in float aDepthState;	//��Ȳ���״̬
layout (location = 3) in float aSize;	//��С���˲������ã�
layout (location = 4) in vec3 aColor;	//��ɫRGB
layout (location = 5) in vec3 aVertex;	//�������꣨ģ������ϵ�£�


out vec3 Color;
out float ShowState;
flat out int isDepth; //�Ƿ������Ȳ���
//ģ���ӽ��µ�PVM
uniform mat4 uModel_m;
uniform mat4 uView_m;
uniform mat4 uProjection_m;


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
		isDepth = int(aDepthState);
		Color = aColor;
	}
	gl_Position = uProjection_m * uView_m * uModel_m* vec4(aVertex.x,aVertex.y,aVertex.z, 1.0f);

	
}

