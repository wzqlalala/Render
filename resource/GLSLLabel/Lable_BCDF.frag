#version 330 core
out vec4 FragColor;

in vec3 Color;
in float ShowState;
in float Depth;
in float isDepth;


void main()
{	

	if(int(ShowState) != 1)
	{
		discard;
	}
	else
	{
		 FragColor = vec4(Color.x/255.0,Color.y/255.0,Color.z/255.0,1.0);
		
		 if(int(isDepth)==0)
		 {
			//������ʱ�����ֵΪ0
			gl_FragDepth = 0;
		 }
		 else
		 {
			 //�ֶ��޸����ֵ��ʹ��������غɱ߽������ֵ����һ��
			 gl_FragDepth = Depth;
		 }

		 
	 }
	



} 