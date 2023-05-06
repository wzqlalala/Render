#include "mLableShapeVertex.h"
namespace MLableRend
{
	mLableShapeVertex::mLableShapeVertex()
	{
		createVertex();
	}

	mLableShapeVertex::~mLableShapeVertex()
	{
	}
	mLableShapeVertex * mLableShapeVertex::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new mLableShapeVertex;
		}
		return _instance;
	}
	mLableShapeVertex * mLableShapeVertex::_instance = nullptr;
	void mLableShapeVertex::createVertex()
	{
		/*
		* X�Ͷ�������
		*/
		float ra = 0.06;
		_XVertices2 =
		{
			{1 * ra,1 * ra,0},
			{-1 * ra,-1 * ra,0},
			{1 * ra,-1 * ra,0},
			{-1 * ra,1 * ra,0}
		};
		/*
		* �������Ͷ�������
		*/
		_QuadVertices =
		{
			{1* ra,1* ra,0},
			{1 * ra,-1 * ra,0},
			{-1 * ra,-1 * ra,0},
			{-1 * ra,1 * ra,0}
		};

		/*
		* �ڵ���
		*/
		float h = 1.0; //��ͷ����---ԭ�����
		float h1 = 0.6;//����׶����----ԭ�����
		float a = 0.06;//����׶����߳���һ��
		_Arrow_Vertices =
		{
			{0.0f,0.0f,0.0f},	//0
			{h,0.0f,0.0f},	//1
			{h1,a,-a},		//2	
			{h1,a,a},			//3
			{h1,-a,a},		//4
			{h1,-a,-a}		//5
		};
		_Arrow_Vertices_2 =
		{
			{0.0f,0.0f,0.0f},	//0
			{h1,0.0f,0.0f},	//1
			{2 * h1 - h,a,-a},		//2	
			{2 * h1 - h,a,a},			//3
			{2 * h1 - h,-a,a},		//4
			{2 * h1 - h,-a,-a},		//5
		};
		/*
		* λ��  �ٶ�  ���ٶ�
		*/
		//�̶������ɶ�Ϊ0��
		float R = 0.05;
		float H1 = 0.25;
		float H2 = 0.33;
		_Cone_Vertices_0 = {
			{0,0,0},		 //0
			{H1,R,-R},	 //1
			{H1,R,R},		 //2
			{H1,-R,R},	 //3
			{H1,-R,-R},	 //4
		};
		_Cylinder_Vertices_0 = {
			{H1,R,-R},	 //1
			{H1,R,R},		 //2
			{H1,-R,R},	 //3
			{H1,-R,-R},	 //4
			{H2,R,-R},	 //5
			{H2,R,R},		 //6
			{H2,-R,R},	 //7
			{H2,-R,-R},	 //8

		};

		//���̶������ɶȲ�Ϊ0��
		float H1_1 = 0.2;
		float H2_1 = 0.28;
		float H3_1 = 0.53;
		_Cone_Line_Vertices = {
			{0,0,0},		  //0
			{H3_1,0,0},	  //1
			{H2_1,R,-R},	  //2
			{H2_1,R,R},	  //3
			{H2_1,-R,R},	  //4
			{H2_1,-R,-R},	  //5					 
		};
		_Cylinder_Vertices =
		{
			{H2_1,R,-R},	  //0
			{H2_1,R,R},	  //1
			{H2_1,-R,R},	  //2
			{H2_1,-R,-R},	  //3	

			{H1_1,R,-R},	  //4
			{H1_1,R,R},	  //5
			{H1_1,-R,R},	  //6
			{H1_1,-R,-R},	  //7	

		};

		/*
		* ��������ͼ�꣩
		*/
		float L = 0.15;//�߳�
		_TriVertices =
		{
			{0.0f,          (float)0.58*L,   0.0f},
			{(float)0.5*L,  -(float)0.29*L,  0.0f},
			{-(float)0.5*L, -(float)0.29*L,  0.0f },
		};

		/*
		* ������ͼ�꣩
		*/
		a = 0.12;//��
		float b = 0.08;	//��
		_RhombusVertices =
		{
			{a,    0.0f,  0.0f},
			{0.0f, b,    0.0f},
			{-a,    0.0f,  0.0f},
			{0.0f, -b,    0.0f},
		};
		/*
		* ��ʮ����ͼ�꣩
		*/
		a = 0.1;//��
		_XVertices =
		{
			{0.0f,    a,  0.0f},
			{0.0f,   -a,    0.0f},
			{-a,    0.0f,  0.0f},
			{a,    0.0f,  0.0f},

		};

		/*
		* ��������ͼ�꣩
		*/
		a = 0.12;
		b = 0.12;
		float c = 0.06;
		float d = 0.06;
		_HomeVertices =
		{
			{0.0f,  0.0f, 0.0f},
			{-a,	-b,  0.0f},

			{-a,	-b,  0.0f},
			{-c,	-b,  0.0f},

			{-c,	-b,  0.0f},
			{-c,	-(b + d),  0.0f},

			{-c,	-(b + d),  0.0f},
			{c,		-(b + d),  0.0f},

			{c,		-(b + d),  0.0f},
			{c,		-b,			0.0f},

			{c,		-b,		0.0f},
			{a,		-b,		0.0f},

			{a,		-b,		0.0f},
			{0.0f,	0.0f,	0.0f},

		};
		/*
		* ������ͼ��
		*/
		a = 0.12;
		b = 0.08;
		_LightningVertices =
		{
			{0.0f,  0.0f, 0.0f },
			{0.0f,  a,   0.0f},
			{-a,  0.0f, 0.0f},
			{0.0f,  0.0f, 0.0f},
			{0.0f,  -a,   0.0f},
			{a,  0.0f, 0.0f},
		};

		/*
		* ���ö�������
		*/
		QVector<QVector3D> _Circle_Vertex; //԰
		int rings = 50;//Բ�Ķ���
		float radius = 0.12;//Բ�İ뾶
		float ringDelta = 2 * M_PI / rings; //ÿ�ζ�Ӧ�ĽǶ�
		float ringAngle = 0;
		for (int i = 0; i < rings; ++i)
		{
			_Circle_Vertex.append(QVector3D(radius*cos(ringAngle), radius*sin(ringAngle), 0));
			ringAngle += ringDelta;
			_Circle_Vertex.append(QVector3D(radius*cos(ringAngle), radius*sin(ringAngle), 0));
		}

		float arrowSize = 0.1;

		/*
		* ����������RCRevolute����
		*/
		float rlinehalflong = 0.8;//��ת�᳤��һ��
		//��ת���߶�
		QVector<QVector3D> _Line_Vertex_rev;
		_Line_Vertex_rev.append(QVector3D(0, 0, rlinehalflong));
		_Line_Vertex_rev.append(QVector3D(0, 0, -rlinehalflong));

		//Բ�μ�ͷ
		QVector<QVector3D> _Arrow_Vertex_rev;
		float radius_arrwow = 0.45;//Բ�İ뾶
		ringAngle = 0;
		for (int i = 0; i < rings / 2 + 1; ++i)
		{
			_Arrow_Vertex_rev.append(QVector3D(radius_arrwow*cos(ringAngle), radius_arrwow*sin(ringAngle), 0));
			ringAngle += ringDelta;
			_Arrow_Vertex_rev.append(QVector3D(radius_arrwow*cos(ringAngle), radius_arrwow*sin(ringAngle), 0));
		}
		QVector3D lastVertex = _Arrow_Vertex_rev.back();

		_Arrow_Vertex_rev.append(QVector3D(-radius_arrwow - arrowSize, arrowSize, 0));
		_Arrow_Vertex_rev.append(lastVertex);
		_Arrow_Vertex_rev.append(QVector3D(-radius_arrwow + arrowSize, arrowSize, 0));
		_Arrow_Vertex_rev.append(lastVertex);

		_Revolute_Vertex = _Circle_Vertex + _Line_Vertex_rev + _Arrow_Vertex_rev;

		/*
		* ����������RCPrismatic����
		*/
		float cuboid_halfwidth = 0.35;//�����Ϳ��һ��
		float cuboid_halfheight = 0.2;//�����͸ߵ�һ��
		float line_halflong = 0.50;//�߶γ���һ��
		float line_height = 0.3;//�߶θ�
		float arrow_halflong = 0.75;//��ͷ����һ��

		//������
		QVector<QVector3D> _Cuboid_Vertex_pri;
		_Cuboid_Vertex_pri.append(QVector3D(cuboid_halfwidth, cuboid_halfheight, 0));//0
		_Cuboid_Vertex_pri.append(QVector3D(cuboid_halfwidth, -cuboid_halfheight, 0));//1
		_Cuboid_Vertex_pri.append(QVector3D(cuboid_halfwidth, -cuboid_halfheight, 0));//1
		_Cuboid_Vertex_pri.append(QVector3D(-cuboid_halfwidth, -cuboid_halfheight, 0));//2
		_Cuboid_Vertex_pri.append(QVector3D(-cuboid_halfwidth, -cuboid_halfheight, 0));//2
		_Cuboid_Vertex_pri.append(QVector3D(-cuboid_halfwidth, cuboid_halfheight, 0));//3
		_Cuboid_Vertex_pri.append(QVector3D(-cuboid_halfwidth, cuboid_halfheight, 0));//3
		_Cuboid_Vertex_pri.append(QVector3D(cuboid_halfwidth, cuboid_halfheight, 0));//0

		//�߶�
		QVector<QVector3D> _Line_Vertex_pri;
		_Line_Vertex_pri.append(QVector3D(line_halflong, line_height, 0));
		_Line_Vertex_pri.append(QVector3D(-line_halflong, line_height, 0));
		_Line_Vertex_pri.append(QVector3D(line_halflong, -line_height, 0));
		_Line_Vertex_pri.append(QVector3D(-line_halflong, -line_height, 0));


		//��ͷ
		QVector<QVector3D> _Arrow_Vertex_pri;
		_Arrow_Vertex_pri.append(QVector3D(-arrow_halflong, 0, 0));
		_Arrow_Vertex_pri.append(QVector3D(arrow_halflong, 0, 0));
		_Arrow_Vertex_pri.append(QVector3D(arrow_halflong - arrowSize, arrowSize, 0));
		_Arrow_Vertex_pri.append(QVector3D(arrow_halflong, 0, 0));
		_Arrow_Vertex_pri.append(QVector3D(arrow_halflong - arrowSize, -arrowSize, 0));
		_Arrow_Vertex_pri.append(QVector3D(arrow_halflong, 0, 0));

		_Prismatic_Vertex = _Cuboid_Vertex_pri + _Line_Vertex_pri + _Arrow_Vertex_pri;

		/*
		* ����������RCPlanar����
		*/
		rlinehalflong = 0.8;//��ת�᳤��һ��
		//��ת���߶�
		QVector<QVector3D> _RotAxis_Vertex_pla;
		_RotAxis_Vertex_pla.append(QVector3D(0, 0, rlinehalflong));
		_RotAxis_Vertex_pla.append(QVector3D(0, 0, -rlinehalflong));

		float cuboid_halfa = 0.18;//�����α߳���һ��
		//������
		QVector<QVector3D> _Cuboid_Vertex_pla;
		_Cuboid_Vertex_pla.append(QVector3D(cuboid_halfa, cuboid_halfa, 0));//0
		_Cuboid_Vertex_pla.append(QVector3D(cuboid_halfa, -cuboid_halfa, 0));//1
		_Cuboid_Vertex_pla.append(QVector3D(cuboid_halfa, -cuboid_halfa, 0));//1
		_Cuboid_Vertex_pla.append(QVector3D(-cuboid_halfa, -cuboid_halfa, 0));//2
		_Cuboid_Vertex_pla.append(QVector3D(-cuboid_halfa, -cuboid_halfa, 0));//2
		_Cuboid_Vertex_pla.append(QVector3D(-cuboid_halfa, cuboid_halfa, 0));//3
		_Cuboid_Vertex_pla.append(QVector3D(-cuboid_halfa, cuboid_halfa, 0));//3
		_Cuboid_Vertex_pla.append(QVector3D(cuboid_halfa, cuboid_halfa, 0));//0

		//ƽ�Ƽ�ͷ
		float transArrow_halflong = 0.75;//��ͷ����һ��
		QVector<QVector3D> _TransArrow_Vertex_pla;
		//X
		_TransArrow_Vertex_pla.append(QVector3D(-transArrow_halflong, 0, 0));
		_TransArrow_Vertex_pla.append(QVector3D(transArrow_halflong, 0, 0));
		_TransArrow_Vertex_pla.append(QVector3D(transArrow_halflong - arrowSize, arrowSize, 0));
		_TransArrow_Vertex_pla.append(QVector3D(transArrow_halflong, 0, 0));
		_TransArrow_Vertex_pla.append(QVector3D(transArrow_halflong - arrowSize, -arrowSize, 0));
		_TransArrow_Vertex_pla.append(QVector3D(transArrow_halflong, 0, 0));
		//Y
		_TransArrow_Vertex_pla.append(QVector3D(0, -transArrow_halflong, 0));
		_TransArrow_Vertex_pla.append(QVector3D(0, transArrow_halflong, 0));
		_TransArrow_Vertex_pla.append(QVector3D(-arrowSize, transArrow_halflong - arrowSize, 0));
		_TransArrow_Vertex_pla.append(QVector3D(0, transArrow_halflong, 0));
		_TransArrow_Vertex_pla.append(QVector3D(arrowSize, transArrow_halflong - arrowSize, 0));
		_TransArrow_Vertex_pla.append(QVector3D(0, transArrow_halflong, 0));

		_PlanarRC_Vertex = _Circle_Vertex + _RotAxis_Vertex_pla + _Cuboid_Vertex_pla + _Arrow_Vertex_rev + _TransArrow_Vertex_pla;


		/*
		* ����������RCCylindrical����
		*/
		float transArrowhalflong = 0.8;//ƽ�Ƽ�ͷ����һ��
		//ƽ�Ƽ�ͷ
		QVector<QVector3D> _TransArrow_Vertex_cyl;
		_TransArrow_Vertex_cyl.append(QVector3D(0, 0, -rlinehalflong));
		_TransArrow_Vertex_cyl.append(QVector3D(0, 0, rlinehalflong));
		_TransArrow_Vertex_cyl.append(QVector3D(-arrowSize, 0, transArrowhalflong - arrowSize));
		_TransArrow_Vertex_cyl.append(QVector3D(0, 0, rlinehalflong));
		_TransArrow_Vertex_cyl.append(QVector3D(arrowSize, 0, transArrowhalflong - arrowSize));
		_TransArrow_Vertex_cyl.append(QVector3D(0, 0, rlinehalflong));

		_Cylindrical_Vertex = _Circle_Vertex + _TransArrow_Vertex_cyl + _Arrow_Vertex_rev;

		/*
		* ����������RCSpherical����
		*/
		//Բ
		float radius_sph = 0.12;//Բ�İ뾶
		//xy
		ringAngle = 0;
		QVector<QVector3D> _Circle_Vertex_sph1, _Circle_Vertex_sph2, _Circle_Vertex_sph3;
		for (int i = 0; i < rings; ++i)
		{
			_Circle_Vertex_sph1.append(QVector3D(radius_sph*cos(ringAngle), radius_sph*sin(ringAngle), 0));
			ringAngle += ringDelta;
			_Circle_Vertex_sph1.append(QVector3D(radius_sph*cos(ringAngle), radius_sph*sin(ringAngle), 0));
		}
		//xz
		ringAngle = 0;
		for (int i = 0; i < rings; ++i)
		{
			_Circle_Vertex_sph2.append(QVector3D(radius_sph*cos(ringAngle), 0, radius_sph*sin(ringAngle)));
			ringAngle += ringDelta;
			_Circle_Vertex_sph2.append(QVector3D(radius_sph*cos(ringAngle), 0, radius_sph*sin(ringAngle)));
		}
		//yz
		ringAngle = 0;
		for (int i = 0; i < rings; ++i)
		{
			_Circle_Vertex_sph3.append(QVector3D(0, radius_sph*cos(ringAngle), radius_sph*sin(ringAngle)));
			ringAngle += ringDelta;
			_Circle_Vertex_sph3.append(QVector3D(0, radius_sph*cos(ringAngle), radius_sph*sin(ringAngle)));
		}
		//�߶�
		QVector<QVector3D> _Line_Vertex_sph;
		_Line_Vertex_sph.append(QVector3D(-radius_sph, 0, 0));
		_Line_Vertex_sph.append(QVector3D(radius_sph, 0, 0));
		_Line_Vertex_sph.append(QVector3D(0, -radius_sph, 0));
		_Line_Vertex_sph.append(QVector3D(0, radius_sph, 0));
		_Line_Vertex_sph.append(QVector3D(0, 0, -radius_sph));
		_Line_Vertex_sph.append(QVector3D(0, 0, radius_sph));

		_Sphere_Vertex = _Circle_Vertex_sph1 + _Circle_Vertex_sph2 + _Circle_Vertex_sph3 + _Line_Vertex_sph;



	}

}
