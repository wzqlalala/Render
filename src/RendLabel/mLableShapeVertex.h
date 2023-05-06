#pragma once

#include <QObject>
#include<QVector3D>
const static double M_PI = 3.1415926;

namespace MLableRend
{
	class mLableShapeVertex 
	{
	public:
		mLableShapeVertex();
		~mLableShapeVertex();
		/*
		* 获取单例指针
		*/
		static mLableShapeVertex* getInstance();

	private:
		/*
		* 构建顶点数据
		*/
		void createVertex();

	public:
		//X字框（旋转中心）
		QVector<QVector3D> _XVertices2;

		//单箭头顶点数据（集中力 重力 体力 均布载荷 初始速度）
		QVector<QVector3D> _Arrow_Vertices, _Arrow_Vertices_2; //_2用于双箭头叠加
		//位移 速度 加速度 虚拟约束
		QVector<QVector3D> _Cone_Vertices_0, _Cylinder_Vertices_0, _Cone_Line_Vertices, _Cylinder_Vertices;
		//三角形框（初始应力，热流密度,温度载荷）
		QVector<QVector3D> _TriVertices;
		//菱形框（对流换热）
		QVector<QVector3D> _RhombusVertices;
		//十字框（内部热源）
		QVector<QVector3D> _XVertices;
		//闪电类型图标（电荷源、电流源）
		QVector<QVector3D> _LightningVertices;
		//房屋性图标（温度约束）
		QVector<QVector3D> _HomeVertices;
		//正方形图标（显示面面绑定、失效绑定连接）
		QVector<QVector3D> _QuadVertices;
		//刚体连接顶点数据
		QVector<QVector3D> _Revolute_Vertex; //旋转副顶点数据
		QVector<QVector3D> _PlanarRC_Vertex; //平面副顶点数据
		QVector<QVector3D> _Cylindrical_Vertex; //圆柱副顶点数据
		QVector<QVector3D> _Prismatic_Vertex; //平移副顶点数据
		QVector<QVector3D> _Sphere_Vertex; //圆球铰副

	private:
		static mLableShapeVertex* _instance;

	};
}
