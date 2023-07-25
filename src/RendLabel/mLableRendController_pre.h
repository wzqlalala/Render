#pragma once
#include <QObject>
#include <memory>
#include "mLableRendEnum.h"
#include "rendlabel_global.h"
#include "mLableDataStruct.h"
#include "mLableDataController_pre.h"
#include "mLableRendIntegrate_pre.h"
#include "mViewToolClass.h"
#include "mLableRendBase.h"
#include "mLableRendFont.h"

//#include "mGeoModelData1.h"
//#include "mMeshModelData1.h"
#define PI 3.1415926
using namespace std;
using namespace MDataLable;
//using namespace MViewBasic;
namespace MLableRend
{
	class RENDLABEL_EXPORT mLableRendController_pre : public QObject
	{
		Q_OBJECT

	public:
		mLableRendController_pre(shared_ptr<mLableRendIntegrateBase> lableRend) 
		{
			_lableRend = dynamic_pointer_cast<mLableRendIntegrate_pre>(lableRend);
		};
		~mLableRendController_pre() {};

		friend class mLableRendController_pre;

		////////////////////////单个数据操作////////////////////////

		/*
		* 添加文字渲染(固定文字  pos范围为（-1,1）)
		*/
		template<class T>
		void appendFontRendData(const QString &name, const QVector<POS_TXT> &pos_txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 255), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_txt.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//文字
			t.fontData.setLableID(_ID);
			t.fontData.setParameter(pos_txt);
			t.fontData.setLableShowState(showState);
			t.fontData.setLableHighlightState(false);
			t.fontData.setLableDepthState(depthState);
			t.fontData.setLableSize(size);
			t.fontData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();

			}
			_ID++;
		}
		/*
		* 添加单个渲染(箭头)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_dir.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos_dir);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(箭头+文字类型) （局部坐标系）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector3D point_origin, const QVector3D vec_x, const QVector3D vec_xy, const  LocalAxisType type, const float size = 0.8,
			const QVector3D color_x = QVector3D(255, 0, 0), const QVector3D color_y = QVector3D(0, 255, 0), const QVector3D color_z = QVector3D(0, 0, 255),
			const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			vec_x.normalized();
			QVector3D vec_z = QVector3D::crossProduct(vec_x, vec_xy).normalized();//Z轴向量
			QVector3D vec_y = QVector3D::crossProduct(vec_z, vec_x).normalized();//Y轴向量

			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter({ { point_origin ,vec_x } });
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color_x);

			t.lableData2.setLableID(_ID);
			t.lableData2.setParameter({ { point_origin ,vec_y } });
			t.lableData2.setLableShowState(showState);
			t.lableData2.setLableHighlightState(false);
			t.lableData2.setLableDepthState(depthState);
			t.lableData2.setLableSize(size);
			t.lableData2.setLableColor(color_y);

			t.lableData3.setLableID(_ID);
			t.lableData3.setParameter({ { point_origin ,vec_z } });
			t.lableData3.setLableShowState(showState);
			t.lableData3.setLableHighlightState(false);
			t.lableData3.setLableDepthState(depthState);
			t.lableData3.setLableSize(size);
			t.lableData3.setLableColor(color_z);

			//添加文字数据文字
			QString x, y, z;
			switch (type)
			{
			case LocalAxisType_Rectangular:x = "X"; y = "Y"; z = "Z";
				break;
			case LocalAxisType_Cylinder:x = "R"; y = "T"; z = "Z";
				break;
			case LocalAxisType_Sphere:x = "R"; y = "T"; z = "P";
				break;
			default:
				break;
			}
			QVector<POS_TXT_DIR> ptd = { { point_origin ,{x,vec_x} }, { point_origin ,{y,vec_y} } ,{ point_origin ,{z,vec_z} } };
			appendFontData(t.fontData, showState, false, depthState, size, QVector3D(255, 255, 255), ptd); //文字默认为白色
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}

		/*
		* 添加单个渲染(箭头+文字类型 文字只有一种) （集中力、重力、力矩、线载荷、压强、预紧力、离心力 、点载荷、面载荷） 
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const QString &txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_dir.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos_dir);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加文字数据文字
			appendFontData(t.fontData, showState, false, depthState, size, color, txt, pos_dir);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}
		/*
		* 添加单个渲染(箭头+文字类型 文字有多种) （初始速度）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_TXT_DIR> &pos_txt_dir,const float size = 1.0,const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, 
			const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos_txt_dir.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			QVector<POS_DIR> pos_dir;
			for (auto v : pos_txt_dir)
			{
				POS_DIR pd;
				pd.first = v.first;
				pd.second = v.second.second;
				pos_dir.push_back(pd);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos_dir);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加文字数据文字
			appendFontData(t.fontData, showState, false, depthState, size, color, pos_txt_dir);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}
		/*
		* 添加单个渲染(固定形状)（初始温度，温度边界，电势, 圆球铰副,);
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);

			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(固定形状+文字类型) （热流密度，电荷密度、对流换热，热辐射、内部热源、温度载荷、电荷、节点/表面/体 电荷   节点/表面 电流、表面电荷密度 表面电流密度 表面磁通密度 表面电场强度）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QString &txt, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加文字数据文字
			appendFontData(t.fontData, showState, false, depthState, size, color, txt, pos);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(约束自由度类型 箭头组合) （位移转角、 固定/对称/反对称 、初始速度  、初始位移 、虚拟约束、刚体约束、刚体运动）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QMap<unsigned int, float> &value, const float size = 1.0,
			const QVector3D color = QVector3D(0, 0, 255), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty() || value.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter1(pos);
			t.lableData.setParameter2(value);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);
			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}
		/*
		* 添加单个渲染(直线+文字)  （刚性、柔性、mpc连接）1对1  1对多
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector3D &pos, const QVector<QVector3D> &pos1, const QString &txt, const QVector3D color = QVector3D(255, 255, 0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos1.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			//整合顶点数据
			QVector3D fontPos = pos; //文字位置
			QVector<QVector3D> vertex{};
			for (auto p : pos1)
			{
				vertex.push_back(pos);
				vertex.push_back(p);
				fontPos += p;
			}
			fontPos /= (pos1.size() + 1);

			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(vertex);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //这类图标没有size大小
			t.lableData.setLableColor(color);

			//添加文字数据文字
			appendFontData(t.fontData, showState, false, depthState, 1.0, color, txt, { fontPos });
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(直线+文字)  （节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮-齿条传动、对接焊缝、角焊缝、铆接、运动耦合、
									//分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度） 1对1
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QPair<QVector3D,QVector3D>> &pos, const QString &txt, const QVector3D color = QVector3D(255, 255, 0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			//整合顶点数据
			QVector<QVector3D> fontPos ; //文字位置
			QVector<QVector3D> vertex;
			for (auto p : pos)
			{
				vertex.push_back(p.first);
				vertex.push_back(p.second);
				fontPos.push_back((p.first+ p.second)/2.0);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(vertex);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //这类图标没有size大小
			t.lableData.setLableColor(color);

			//添加文字数据文字
			appendFontData(t.fontData, showState, false, depthState, 1.0, color, txt, fontPos);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加单个渲染(直线)  （弹簧 阻尼）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector3D &pos1, const QVector3D &pos2, const QVector3D color = QVector3D(255, 255, 0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter({ pos1 ,pos2 });
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //这类图标没有size大小
			t.lableData.setLableColor(color);

			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}

		/*
		* 添加单个渲染(主从面)  //（连接中的面面绑定 、接触）
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos1, const QVector<QVector3D> &pos2, const QVector3D color1 = QVector3D(255, 255, 0),
			const QVector3D color2 = QVector3D(160, 32, 240), const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos1.empty() || pos2.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}

			T t;
			//图标
			//主面
			t.lableData.setLableID(_ID);
			t.lableData.setParameter(pos1);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(1.0); //这类图标没有size大小
			t.lableData.setLableColor(color1);

			//从面
			t.lableData2.setLableID(_ID);
			t.lableData2.setParameter(pos2);
			t.lableData2.setLableShowState(showState);
			t.lableData2.setLableHighlightState(false);
			t.lableData2.setLableDepthState(depthState);
			t.lableData2.setLableSize(1.0); //这类图标没有size大小
			t.lableData2.setLableColor(color2);

			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}

		/*
		* 添加刚体连接(旋转副，平面副、圆柱形副)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<POS_DIR> &pos_dir, const FixShapeRotateType type, const float size = 1.0,
			const QVector3D color = QVector3D(255, 255, 0), const bool showState = true, const bool depthState = false, const bool isUpdate = true)//重载占位参数
		{
			if (_lableRend == nullptr) return;
			if (pos_dir.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter1(pos_dir);
			t.lableData.setParameter2(type);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;
		}
		/*
		* 添加刚体连接(平移)
		*/
		template<class T>
		void appendLableRendData(const QString &name, const QVector<QVector3D> &pos, const QVector3D vec1, const QVector3D vec2, const FixShapeRotateType type,
			const float size = 1.0, const QVector3D color = QVector3D(0, 255, 0), const bool showState = true, const bool depthState = false, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			if (pos.empty()) return;
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<T>(name))
			{
				deleteLableRendData<T>(name);
			}
			T t;
			//图标
			t.lableData.setLableID(_ID);
			t.lableData.setParameter1(pos);
			t.lableData.setParameter2({ vec1 ,vec2 });
			t.lableData.setParameter3(type);
			t.lableData.setLableShowState(showState);
			t.lableData.setLableHighlightState(false);
			t.lableData.setLableDepthState(depthState);
			t.lableData.setLableSize(size);
			t.lableData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<T>(name, t);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<T>(name, Append);
				emit updateView();
			}
			_ID++;

		}
		/*
		* 添加刚性墙渲染
		*/
		//无限大平面刚性
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, 
			QVector3D minEdge = QVector3D(0,0,0), QVector3D maxEdge = QVector3D(0,0,0),
			const QVector3D color = QVector3D(188.0, 188.0, 188.0),const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name) && _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			//模型几何中心坐标
			//网格数据
			//模型中心
			QVector3D ModelCenterPos = (minEdge + maxEdge / 2);
			//获取以模型几何中心为中心的最大旋转半径
			float max_x = std::max(std::abs(minEdge.x() - ModelCenterPos.x()), std::abs(maxEdge.x() - ModelCenterPos.x()));
			float max_y = std::max(std::abs(minEdge.y() - ModelCenterPos.y()), std::abs(maxEdge.y() - ModelCenterPos.y()));
			float max_z = std::max(std::abs(minEdge.z() - ModelCenterPos.z()), std::abs(maxEdge.z() - ModelCenterPos.z()));
			float MaxRadius = sqrt(pow(max_x, 2) + pow(max_y, 2) + pow(max_z, 2));
			//模型几何中心到平面最短距离
			float distance = ModelCenterPos.distanceToPlane(pos, normal);

			//模型几何中心到平面的映射点，也就是平面的中心点坐标（法向量方向上）
			QVector3D PlanCenterPos;

			if (std::abs((ModelCenterPos + normal * distance).distanceToPlane(pos, normal)) < 0.1)
			{
				PlanCenterPos = ModelCenterPos + normal * distance;
			}
			else if (std::abs((ModelCenterPos - normal * distance).distanceToPlane(pos, normal)) < 0.1)
			{
				PlanCenterPos = ModelCenterPos - normal * distance;
			}
			else
			{
				return;
			}
			//设置无限大平面的长和宽 （1.2倍模型最大尺寸）
			float Size = MaxRadius == 0 ? 10 : 2.2 * MaxRadius;

			//保证平面的一条边在X，Z平面内
			QVector3D L_vec, W_vec;
			if (normal == QVector3D(0, 1, 0))
			{
				L_vec = QVector3D(1, 0, 0);
				W_vec = QVector3D(0, 0, 1);
			}
			else
			{
				L_vec = QVector3D::crossProduct(normal, QVector3D(0, 1, 0)).normalized(); //确定长度方向的向量
				W_vec = QVector3D::crossProduct(normal, L_vec).normalized();//确定宽度方向的向量
			}
			//获取平面的四个顶点坐标
			QVector3D PlanPointPos_1 = PlanCenterPos + Size / 2 * (-L_vec - W_vec);
			QVector3D PlanPointPos_2 = PlanCenterPos + Size / 2 * (-L_vec + W_vec);
			QVector3D PlanPointPos_3 = PlanCenterPos + Size / 2 * (L_vec + W_vec);
			QVector3D PlanPointPos_4 = PlanCenterPos + Size / 2 * (L_vec - W_vec);
			//面坐标
			QVector<QVector3D> PlanPointPos = { PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4 };


			//面
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //这类图标没有size大小
			f.lableData.setLableColor(color);

			//坐标系
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,normal } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(0, 0, 255));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,normal } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 0, 255));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//坐标系文字
			QVector<POS_TXT_DIR> ptd = { { pos ,{"N",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //文字默认为白色

			//添加数据库
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}


			_ID++;
		}
		//有限大平面刚性墙
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, QVector3D L_vec, const float L, const float W, const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name) 
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			normal = normal.normalized();
			L_vec = L_vec.normalized();
			QVector3D W_vec = QVector3D::crossProduct(normal, L_vec);
			//第一个坐标 起始坐标
			QVector3D PlanPointPos_1 = pos;
			//第二个坐标 
			QVector3D PlanPointPos_2 = pos + L_vec * L;
			//第三个坐标
			QVector3D PlanPointPos_3 = pos + L_vec * L + W_vec * W;
			//第四个坐标
			QVector3D PlanPointPos_4 = pos + W_vec * W;
			//线数据
			QVector<QVector3D> LinePointPos = { PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_2,PlanPointPos_3 ,PlanPointPos_3,PlanPointPos_4,PlanPointPos_4,PlanPointPos_1 };
			//面坐标
			QVector<QVector3D> PlanPointPos = { PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4 };
			//线
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //这类图标没有size大小
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //边界线固定为黑色
			//面
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //这类图标没有size大小
			f.lableData.setLableColor(color);

			//坐标系
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,L_vec } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(255, 0, 0));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,W_vec } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 255, 0));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//坐标系文字
			QVector<POS_TXT_DIR> ptd = { { pos ,{"L",L_vec}},{ pos ,{"W",W_vec}}, { pos ,{"N",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //文字默认为白色

			//添加数据库
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}
		//长方体刚性墙
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D P_normal, QVector3D L_vec, QVector3D M_vec, const float P, const float L, const float M, const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			P_normal = P_normal.normalized();
			L_vec = L_vec.normalized();
			M_vec = M_vec.normalized();
			//第一个坐标 起始坐标
			QVector3D PlanPointPos_1 = pos;
			//第二个坐标L
			QVector3D PlanPointPos_2 = pos + L_vec * L;
			//第三个坐标
			QVector3D PlanPointPos_3 = pos + L_vec * L + M_vec * M;
			//第四个坐标
			QVector3D PlanPointPos_4 = pos + M_vec * M;
			//第5个坐标
			QVector3D PlanPointPos_5 = pos - P_normal * P;
			//第6坐标
			QVector3D PlanPointPos_6 = pos - P_normal * P + L_vec * L;
			//第7坐标
			QVector3D PlanPointPos_7 = pos - P_normal * P + L_vec * L + M_vec * M;
			//第8坐标
			QVector3D PlanPointPos_8 = pos - P_normal * P + M_vec * M;

			//线数据
			QVector<QVector3D> LinePointPos = {
				PlanPointPos_1 ,PlanPointPos_2 ,
				PlanPointPos_2,PlanPointPos_3 ,
				PlanPointPos_3,PlanPointPos_4,
				PlanPointPos_4,PlanPointPos_1 ,
				PlanPointPos_5 ,PlanPointPos_6 ,
				PlanPointPos_6,PlanPointPos_7 ,
				PlanPointPos_7,PlanPointPos_8,
				PlanPointPos_8,PlanPointPos_5,
				PlanPointPos_1,PlanPointPos_5,
				PlanPointPos_2,PlanPointPos_6,
				PlanPointPos_3,PlanPointPos_7,
				PlanPointPos_4,PlanPointPos_8,
			};
			//面坐标
			QVector<QVector3D> PlanPointPos = { 
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4,
				PlanPointPos_5 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_8,
				PlanPointPos_1 ,PlanPointPos_5 ,PlanPointPos_8 ,PlanPointPos_4,
				PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_3,
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_5,
				PlanPointPos_4 ,PlanPointPos_3 ,PlanPointPos_7 ,PlanPointPos_8,
			};
			//线
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //这类图标没有size大小
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //边界线固定为黑色
			//面
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //这类图标没有size大小
			f.lableData.setLableColor(color);

			//坐标系
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,M_vec } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(255, 0, 0));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,L_vec } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 255, 0));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,P_normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//坐标系文字
			QVector<POS_TXT_DIR> ptd = { { pos ,{"L",L_vec}},{ pos ,{"M",M_vec}}, { pos ,{"P",P_normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //文字默认为白色

			//添加数据库
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}
		//圆柱体刚性墙
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, const float radius, const float L, const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			QVector3D rVec = mViewToolClass::getNormVector(normal);
			QVector<QVector3D> ShapePointPos, LinePointPos;
			getCylinderVertex(ShapePointPos, LinePointPos, pos, normal, rVec, radius, L, 5);

		
			//线
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //这类图标没有size大小
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //边界线固定为黑色
			//面
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(ShapePointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //这类图标没有size大小
			f.lableData.setLableColor(color);

			//坐标系
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,normal } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(0, 0, 255));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,normal } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 0, 255));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//坐标系文字
			QVector<POS_TXT_DIR> ptd = { { pos ,{"P",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //文字默认为白色

			//添加数据库
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}
		//球体刚性墙
		void appendLableRendData_RigidWall(const QString &name, const QVector3D pos, QVector3D normal, const float radius,  const QVector3D color = QVector3D(188.0, 188.0, 188.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			if (_lableRend->_lableDataController->isLableDataExist<Face_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Axis_RW_Explicit>(name)
				&& _lableRend->_lableDataController->isLableDataExist<Line_RW_Explicit>(name))
			{
				deleteLableRendData_RigidWall(name);
			}
			normal = normal.normalized();
			QVector<QVector3D> SpherePointPos, LinePointPos;
			getSphereVertex(SpherePointPos, LinePointPos, pos, radius, 5);


			//线
			Line_RW_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //这类图标没有size大小
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //边界线固定为黑色
			//面
			Face_RW_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(SpherePointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //这类图标没有size大小
			f.lableData.setLableColor(color);

			//坐标系
			Axis_RW_Explicit a;
			a.lableData.setLableID(_ID);
			a.lableData.setParameter({ { pos ,normal } });
			a.lableData.setLableShowState(showState);
			a.lableData.setLableHighlightState(false);
			a.lableData.setLableDepthState(false);
			a.lableData.setLableSize(0.8);
			a.lableData.setLableColor(QVector3D(0, 0, 255));

			a.lableData2.setLableID(_ID);
			a.lableData2.setParameter({ { pos ,normal } });
			a.lableData2.setLableShowState(showState);
			a.lableData2.setLableHighlightState(false);
			a.lableData2.setLableDepthState(false);
			a.lableData2.setLableSize(0.8);
			a.lableData2.setLableColor(QVector3D(0, 0, 255));

			a.lableData3.setLableID(_ID);
			a.lableData3.setParameter({ { pos ,normal } });
			a.lableData3.setLableShowState(showState);
			a.lableData3.setLableHighlightState(false);
			a.lableData3.setLableDepthState(false);
			a.lableData3.setLableSize(0.8);
			a.lableData3.setLableColor(QVector3D(0, 0, 255));
			//坐标系文字
			QVector<POS_TXT_DIR> ptd = { { pos ,{"N",normal} } };
			appendFontData(a.fontData, showState, false, false, 1.0, QVector3D(255, 255, 255), ptd); //文字默认为白色

			//添加数据库
			_lableRend->_lableDataController->appendLableData<Line_RW_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_RW_Explicit>(name, f);
			_lableRend->_lableDataController->appendLableData<Axis_RW_Explicit>(name, a);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, Append);
				updateRend_Single<Face_RW_Explicit>(name, Append);
				updateRend_Single<Axis_RW_Explicit>(name, Append);
				emit updateView();
			}

			_ID++;
		}

		/*
		* 添加盒子
		*/
		void appendLableRendData_Box(const QString &name, const QVector3D pos, QVector3D normalx, QVector3D normaly, QVector3D normalz, const float l_x, const float l_y, const float l_z, const QVector3D color = QVector3D(74.0, 181.0, 217.0),
			const bool showState = true, const bool depthState = true, const bool isUpdate = true)
		{
			//判断数据是否存在
			if (_lableRend->_lableDataController->isLableDataExist<Face_Box_Explicit>(name) && _lableRend->_lableDataController->isLableDataExist<Line_Box_Explicit>(name))
			{
				deleteLableRendData_Box(name);
			}
			normalx = normalx.normalized();
			normaly = normaly.normalized();
			normalz = normalz.normalized();
			//第一个坐标 起始坐标
			QVector3D PlanPointPos_1 = pos + normaly * l_y + normalz * l_z;
			//第二个坐标L
			QVector3D PlanPointPos_2 = pos + normalx * l_x + normaly * l_y + normalz * l_z;
			//第三个坐标
			QVector3D PlanPointPos_3 = pos + normalx * l_x + normaly * l_y;
			//第四个坐标
			QVector3D PlanPointPos_4 = pos + normaly * l_y;
			//第5个坐标
			QVector3D PlanPointPos_5 = pos + normalz * l_z;
			//第6坐标
			QVector3D PlanPointPos_6 = pos + normalx * l_x + normalz * l_z;
			//第7坐标
			QVector3D PlanPointPos_7 = pos + normalx * l_x;
			//第8坐标
			QVector3D PlanPointPos_8 = pos;
			//线数据
			QVector<QVector3D> LinePointPos = {
				PlanPointPos_1 ,PlanPointPos_2 ,
				PlanPointPos_2,PlanPointPos_3 ,
				PlanPointPos_3,PlanPointPos_4,
				PlanPointPos_4,PlanPointPos_1 ,
				PlanPointPos_5 ,PlanPointPos_6 ,
				PlanPointPos_6,PlanPointPos_7 ,
				PlanPointPos_7,PlanPointPos_8,
				PlanPointPos_8,PlanPointPos_5,
				PlanPointPos_1,PlanPointPos_5,
				PlanPointPos_2,PlanPointPos_6,
				PlanPointPos_3,PlanPointPos_7,
				PlanPointPos_4,PlanPointPos_8,
			};
			//面坐标
			QVector<QVector3D> PlanPointPos = {
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_3 ,PlanPointPos_4,
				PlanPointPos_5 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_8,
				PlanPointPos_1 ,PlanPointPos_5 ,PlanPointPos_8 ,PlanPointPos_4,
				PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_7 ,PlanPointPos_3,
				PlanPointPos_1 ,PlanPointPos_2 ,PlanPointPos_6 ,PlanPointPos_5,
				PlanPointPos_4 ,PlanPointPos_3 ,PlanPointPos_7 ,PlanPointPos_8,
			};
			//线
			Line_Box_Explicit l;
			l.lableData.setLableID(_ID);
			l.lableData.setParameter(LinePointPos);
			l.lableData.setLableShowState(showState);
			l.lableData.setLableHighlightState(false);
			l.lableData.setLableDepthState(depthState);
			l.lableData.setLableSize(1.0); //这类图标没有size大小
			l.lableData.setLableColor(QVector3D(0, 0, 0)); //边界线固定为黑色

			//面
			Face_Box_Explicit f;
			f.lableData.setLableID(_ID);
			f.lableData.setParameter(PlanPointPos);
			f.lableData.setLableShowState(showState);
			f.lableData.setLableHighlightState(false);
			f.lableData.setLableDepthState(depthState);
			f.lableData.setLableSize(1.0); //这类图标没有size大小
			f.lableData.setLableColor(color);
			//添加数据库
			_lableRend->_lableDataController->appendLableData<Line_Box_Explicit>(name, l);
			_lableRend->_lableDataController->appendLableData<Face_Box_Explicit>(name, f);

			//渲染更新
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, Append);
				updateRend_Single<Face_Box_Explicit>(name, Append);
				emit updateView();
			}
			_ID++;
		}
	
		
		/*
	    * 删除单个渲染（通用）  
		*/
		template<class T>
		void deleteLableRendData(const QString &name, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T> &data = _lableRend->_lableDataController->getLableData<T>();
			if (data.find(name) == data.end())
			{
				return;
			}
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T>(name, Delete);
				emit updateView();
			}
			//删除数据库中数据		
			_lableRend->_lableDataController->deleteLableData<T>(name);
						
		}
		//刚性墙
		void deleteLableRendData_RigidWall(const QString &name, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//更新渲染
			if (isUpdate)
			{		
				updateRend_Single<Line_RW_Explicit>(name, Delete);
				updateRend_Single<Face_RW_Explicit>(name, Delete);
				updateRend_Single<Axis_RW_Explicit>(name, Delete);	
				emit updateView();
			}
			//删除数据库中数据	
			_lableRend->_lableDataController->deleteLableData<Line_RW_Explicit>(name);
			_lableRend->_lableDataController->deleteLableData<Face_RW_Explicit>(name);
			_lableRend->_lableDataController->deleteLableData<Axis_RW_Explicit>(name);

		}
		//盒子
		void deleteLableRendData_Box(const QString &name, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, Delete);
				updateRend_Single<Face_Box_Explicit>(name, Delete);

				emit updateView();
			}
			//删除数据库中数据	
			_lableRend->_lableDataController->deleteLableData<Line_Box_Explicit>(name);
			_lableRend->_lableDataController->deleteLableData<Face_Box_Explicit>(name);


		}

		/*
		* 设置单个显隐状态（通用）  //T1为数据类型  T2为 Font Lable  Lable_Lable Lable_Font  Lable_Lable_Lable_Font 看只修改文字  图标  或者 文字+图标
		*/
		template<class T1, class T2>
		void setLableRendShowState(const QString &name, const bool state, const bool isUpdate = true)
		{
			if(_lableRend==nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			LableOperateMode mode;
			//更新数据库中数据
			mode = (state == true ? Show : Hide);
			_lableRend->_lableDataController->updateLableData_ShowState<T1>(name, state, T2());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, mode);
				emit updateView();

			}
		}
		//刚性墙
		void setLableRendShowState_RigidWall(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//更新数据库中数据
			mode = (state == true ? Show : Hide);

			_lableRend->_lableDataController->updateLableData_ShowState<Line_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_ShowState<Face_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_ShowState<Axis_RW_Explicit>(name, state, Lable_Lable_Lable_Font());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, mode);
				updateRend_Single<Face_RW_Explicit>(name, mode);
				updateRend_Single<Axis_RW_Explicit>(name, mode);
				emit updateView();
			}
			
		}
		//盒子
		void setLableRendShowState_Box(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//更新数据库中数据
			mode = (state == true ? Show : Hide);

			_lableRend->_lableDataController->updateLableData_ShowState<Line_Box_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_ShowState<Face_Box_Explicit>(name, state, Lable());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, mode);
				updateRend_Single<Face_Box_Explicit>(name, mode);
				emit updateView();
			}

		}


		/*
		* 设置单个高亮状态（通用）  //T1为数据类型  T2为 Font Lable Lable_Lable Lable_Font  看只修改文字  图标  或者 文字+图标
		*/
		template<class T1, class T2>
		void setLableRendHightlightState(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			LableOperateMode mode;
			//更新数据库中数据		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<T1>(name, state, T2());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, mode);
				emit updateView();
			}
		}
		//刚性墙
		void setLableRendHightlightState_RigidWall(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//更新数据库中数据		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<Line_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_HighlightState<Face_RW_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_HighlightState<Axis_RW_Explicit>(name, state, Lable_Lable_Lable_Font());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Line_RW_Explicit>(name, mode);
				updateRend_Single<Face_RW_Explicit>(name, mode);
				updateRend_Single<Axis_RW_Explicit>(name, mode);
				emit updateView();
			}
			
		}
		//盒子
		void setLableRendHightlightState_Box(const QString &name, const bool state, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			LableOperateMode mode;
			//更新数据库中数据		
			mode = (state == true ? Highlight : NoHighlight);
			_lableRend->_lableDataController->updateLableData_HighlightState<Line_Box_Explicit>(name, state, Lable());
			_lableRend->_lableDataController->updateLableData_HighlightState<Face_Box_Explicit>(name, state, Lable());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Line_Box_Explicit>(name, mode);
				updateRend_Single<Face_Box_Explicit>(name, mode);	
				emit updateView();
			}

		}


		/*
		* 设置单个颜色（通用） //T1为数据类型  T2为 Font Lable Lable_Lable Lable_Font  看只修改文字、图标 、或者 文字+图标
		*/
		template<class T1, class T2>
		void setLableRendColor(const QString &name, const QVector3D color, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			_lableRend->_lableDataController->updateLableData_Color<T1>(name, color, T2());

			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, SetColor);
				emit updateView();
			}
		}
		//刚性墙(只能设置面的颜色)
		void setLableRendColor_RigidWall(const QString &name, const QVector3D color, const AnalysisModule module, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			_lableRend->_lableDataController->updateLableData_Color<Face_RW_Explicit>(name, color, Lable());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Face_RW_Explicit>(name, SetColor);
				emit updateView();
			}
		}
		//盒子(只能设置面的颜色)
		void setLableRendColor_Box(const QString &name, const QVector3D color, const AnalysisModule module, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;

			_lableRend->_lableDataController->updateLableData_Color<Face_Box_Explicit>(name, color, Lable());
			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<Face_Box_Explicit>(name, SetColor);
				emit updateView();
			}
		}

		/*
		* 设置单个大小（通用）   //T1为数据类型  T2为 Font Lable Lable_Lable  看只修改文字、图标  不支持同时修改
		*/
		template<class T1, class T2>
		void setLableRendSize(const QString &name, const float size, const bool isUpdate = true)
		{
			if (_lableRend == nullptr) return;
			QHash<QString, T1> &data = _lableRend->_lableDataController->getLableData<T1>();
			if (data.find(name) == data.end())
			{
				return;
			}
			_lableRend->_lableDataController->updateLableData_Size<T1>(name, size, T2());

			//更新渲染
			if (isUpdate)
			{
				updateRend_Single<T1>(name, SetSize);
				emit updateView();
			}
		}

		////////////////////////某一分析类型同一类型数据操作////////////////////////
		/*
		* 添加某一分析类型同一类型所有渲染  用于读文件后添加
		*/
		void appendLableRendData_SameType(const AnalysisModule module, const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			updateAllRend_SameType(module, type, Append);
		}
		/*
		* 删除某一分析类型同一类型所有渲染 
		*/
		void deleteLableRendData_SameType(const AnalysisModule module, const LableOperateType type)
		{
			if (_lableRend == nullptr) return;
			//更新渲染
			updateAllRend_SameType(module, type, Delete);
			//删除数据库中数据	
			_lableRend->_lableDataController->deleteLableData_SameType(module, type);		
		}
		/*
		* 设置某一分析类型同一类型所有显隐状态
		*/
		void setLableRend_ShowState_SameType(const AnalysisModule module, const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_ShowState_SameType(module, type, state);
			//更新渲染
			if (state)
			{
				updateAllRend_SameType(module, type, Show);
			}
			else
			{
				updateAllRend_SameType(module, type, Hide);
			}
		}
		/*
		* 设置某一分析类型同一类型所有高亮状态
		*/
		void setLableRend_Highlight_SameType(const AnalysisModule module, const LableOperateType type, const bool state)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_HighlightState_SameType(module, type, state);
			//更新渲染
			if (state)
			{
				updateAllRend_SameType(module, type, Highlight);
			}
			else
			{
				updateAllRend_SameType(module, type, NoHighlight);
			}
		}

		/*
		* 设置某一分析类型同一类型所有颜色
		*/
		void setLableRend_Color_SameType(const AnalysisModule module, const LableOperateType type, const QVector3D color)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_Color_SameType(module, type, color);
			//更新渲染		
			updateAllRend_SameType(module, type, SetColor);

		}
		/*
		* 设置某一分析类型同一类型所有大小(目前只支持修改图标大小 不支持文字)
		*/
		void setLableRend_Size_SameType(const AnalysisModule module, const LableOperateType type, const float size)
		{
			if (_lableRend == nullptr) return;
			//更新数据库中数据
			_lableRend->_lableDataController->updateLableData_Size_SameType<Lable>(module, type, size);  //只更新图标大小
			//更新渲染		
			updateAllRend_SameType(module, type, SetSize);

		}


		////////////////////////某一分析类型所有渲染操作////////////////////////
		/*
		* 删除某一分析类型所有渲染  
		*/
		void deleteAllLableRendData(const AnalysisModule module)
		{
			deleteLableRendData_SameType(module, LableOperateType_LocalAxis);
			deleteLableRendData_SameType(module, LableOperateType_Load);
			deleteLableRendData_SameType(module, LableOperateType_BC);
			deleteLableRendData_SameType(module, LableOperateType_Connect);
			deleteLableRendData_SameType(module, LableOperateType_Contact);
			deleteLableRendData_SameType(module, LableOperateType_RigidConnect);
			deleteLableRendData_SameType(module, LableOperateType_RigidConstraint);
			deleteLableRendData_SameType(module, LableOperateType_RigidWall);
			deleteLableRendData_SameType(module, LableOperateType_InitCondition);
			deleteLableRendData_SameType(module, LableOperateType_Box);
			deleteLableRendData_SameType(module, LableOperateType_Vector);
			deleteLableRendData_SameType(module, LableOperateType_PlyCoordinate);
		}
		/*
		* 设置某一分析类型所有显隐状态
		*/
		void setAllLableRend_ShowState(const AnalysisModule module, const bool state)
		{
			setLableRend_ShowState_SameType(module, LableOperateType_LocalAxis, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Load, state);
			setLableRend_ShowState_SameType(module, LableOperateType_BC, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Connect, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Contact, state);
			setLableRend_ShowState_SameType(module, LableOperateType_RigidConnect, state);
			setLableRend_ShowState_SameType(module, LableOperateType_RigidConstraint, state);
			setLableRend_ShowState_SameType(module, LableOperateType_RigidWall, state);
			setLableRend_ShowState_SameType(module, LableOperateType_InitCondition, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Box, state);
			setLableRend_ShowState_SameType(module, LableOperateType_Vector, state);
			setLableRend_ShowState_SameType(module, LableOperateType_PlyCoordinate, state);
		}
		/*
		* 设置某一分析类型所有高亮状态
		*/
		void setAllLableRend_Highlight(const AnalysisModule module, const bool state)
		{
			setLableRend_Highlight_SameType(module, LableOperateType_LocalAxis, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Load, state);
			setLableRend_Highlight_SameType(module, LableOperateType_BC, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Connect, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Contact, state);
			setLableRend_Highlight_SameType(module, LableOperateType_RigidConnect, state);
			setLableRend_Highlight_SameType(module, LableOperateType_RigidConstraint, state);
			setLableRend_Highlight_SameType(module, LableOperateType_RigidWall, state);
			setLableRend_Highlight_SameType(module, LableOperateType_InitCondition, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Box, state);
			setLableRend_Highlight_SameType(module, LableOperateType_Vector, state);
			setLableRend_Highlight_SameType(module, LableOperateType_PlyCoordinate, state);
		}
		/*
		* 设置某一分析类型所有颜色(一般不用此接口 各类型图标颜色不同)
		*/
		void setAllLableRendColor(const AnalysisModule module, const QVector3D color)
		{
			setLableRend_Color_SameType(module, LableOperateType_LocalAxis, color);
			setLableRend_Color_SameType(module, LableOperateType_Load, color);
			setLableRend_Color_SameType(module, LableOperateType_BC, color);
			setLableRend_Color_SameType(module, LableOperateType_Connect, color);
			setLableRend_Color_SameType(module, LableOperateType_Contact, color);
			setLableRend_Color_SameType(module, LableOperateType_RigidConnect, color);
			setLableRend_Color_SameType(module, LableOperateType_RigidConstraint, color);
			setLableRend_Color_SameType(module, LableOperateType_RigidWall, color);
			setLableRend_Color_SameType(module, LableOperateType_InitCondition, color);
			setLableRend_Color_SameType(module, LableOperateType_Box, color);
			setLableRend_Color_SameType(module, LableOperateType_Vector, color);
			setLableRend_Color_SameType(module, LableOperateType_PlyCoordinate, color);
		}
		/*
		* 设置某一分析类型所有大小
		*/
		void setAllLableRendSize(const AnalysisModule module, const float size)
		{

			setLableRend_Size_SameType(module, LableOperateType_LocalAxis, size);
			setLableRend_Size_SameType(module, LableOperateType_Load, size);
			setLableRend_Size_SameType(module, LableOperateType_BC, size);
			setLableRend_Size_SameType(module, LableOperateType_Connect, size);
			setLableRend_Size_SameType(module, LableOperateType_Contact, size);
			setLableRend_Size_SameType(module, LableOperateType_RigidConnect, size);
			setLableRend_Size_SameType(module, LableOperateType_RigidConstraint, size);
			setLableRend_Size_SameType(module, LableOperateType_RigidWall, size);
			setLableRend_Size_SameType(module, LableOperateType_InitCondition, size);
			setLableRend_Size_SameType(module, LableOperateType_Box, size);
			setLableRend_Size_SameType(module, LableOperateType_Vector, size);
			setLableRend_Size_SameType(module, LableOperateType_PlyCoordinate, size);
		}

		////////////////////////所有分析类型所有渲染操作////////////////////////
		/*
		* 删除所有分析类型所有渲染
		*/
		void deleteAllLableRendData()
		{
			deleteAllLableRendData(ImplicitAnalysis);
			deleteAllLableRendData(ExplicitAnalysis);
			deleteAllLableRendData(MagnetAnalysis);
		}
		/*
		* 设置所有分析类型所有显隐状态
		*/
		void setAllLableRend_ShowState(const bool state)
		{
			setAllLableRend_ShowState(ImplicitAnalysis, state);
			setAllLableRend_ShowState(ExplicitAnalysis, state);
			setAllLableRend_ShowState(MagnetAnalysis, state);
		}
		/*
		* 设置所有分析类型所有高亮状态
		*/
		void setAllLableRend_Highlight(const bool state)
		{
			setAllLableRend_Highlight(ImplicitAnalysis, state);
			setAllLableRend_Highlight(ExplicitAnalysis, state);
			setAllLableRend_Highlight(MagnetAnalysis, state);
		}
		/*
		* 设置所有分析类型所有颜色(一般不用此接口 各类型图标颜色不同)
		*/
		void setAllLableRendColor(const QVector3D color)
		{
			setAllLableRendColor(ImplicitAnalysis, color);
			setAllLableRendColor(ExplicitAnalysis, color);
			setAllLableRendColor(MagnetAnalysis, color);
		}
		/*
		* 设置所有分析类型所有大小
		*/
		void setAllLableRendSize(const float size)
		{
			setAllLableRendSize(ImplicitAnalysis, size);
			setAllLableRendSize(ExplicitAnalysis, size);
			setAllLableRendSize(MagnetAnalysis, size);
		}
		////////////////////////////////////////////////////////////
		/*
		*  重命名
		*/
		template<class T>
		void resetName(const QString& oldname, const QString& newname)
		{
			if (_lableRend == nullptr) return;
			_lableRend->_lableDataController->resetName<T>(oldname, newname);
		}
		//刚性墙
		void resetName_RigidWall(const QString& oldname, const QString& newname)
		{
			if (_lableRend == nullptr) return;
			_lableRend->_lableDataController->resetName<Line_RW_Explicit>(oldname, newname);
			_lableRend->_lableDataController->resetName<Face_RW_Explicit>(oldname, newname);
			_lableRend->_lableDataController->resetName<Axis_RW_Explicit>(oldname, newname);
		}		
		//盒子
		void resetName_Box(const QString& oldname, const QString& newname)
		{
			if (_lableRend == nullptr) return;
			_lableRend->_lableDataController->resetName<Line_Box_Explicit>(oldname, newname);
			_lableRend->_lableDataController->resetName<Face_Box_Explicit>(oldname, newname);

		}
		
	private:																		
		/*
		* 添加文字数据
		*/
		void appendFontData(LableData1<QVector<POS_TXT_DIR>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QString& txt, const QVector<POS_DIR> &para)
		{
			if (_lableRend == nullptr) return;
			fontData.setLableID(_ID);
			fontData.setLableShowState(showState);
			fontData.setLableHighlightState(highlightState);
			fontData.setLableDepthState(depthState);
			fontData.setLableSize(size);
			fontData.setLableColor(color);
			QVector<POS_TXT_DIR> t;
			//带矢量方向参数

			for (auto val : para)
			{
				QVector3D pos = val.first;
				QVector3D dir = val.second;
				t.push_back({ pos,{txt,dir} });
			}
			//添加数据
			fontData.setParameter(t);
		}
		void appendFontData(LableData1<QVector<POS_TXT_DIR>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QVector<POS_TXT_DIR> &para)
		{
			if (_lableRend == nullptr) return;
			fontData.setLableID(_ID);
			fontData.setLableShowState(showState);
			fontData.setLableHighlightState(highlightState);
			fontData.setLableDepthState(depthState);
			fontData.setLableSize(size);
			fontData.setLableColor(color);
			fontData.setParameter(para);
		}
		void appendFontData(LableData1<QVector<POS_TXT>> &fontData, const bool showState, const bool highlightState, const bool depthState,
			const float size, const QVector3D color, const QString& txt, const QVector<QVector3D> &para)
		{
			if (_lableRend == nullptr) return;
			fontData.setLableID(_ID);
			fontData.setLableShowState(showState);
			fontData.setLableHighlightState(highlightState);
			fontData.setLableDepthState(depthState);
			fontData.setLableSize(size);
			fontData.setLableColor(color);
			QVector<POS_TXT> t;
			//带矢量方向参数

			for (auto pos : para)
			{
				t.push_back({ pos,txt });
			}
			//添加数据
			fontData.setParameter(t);
		}

		/*
		* 更新单个渲染
		*/
		template<class T>
		void updateRend_Single(const QString &name, const LableOperateMode type)
		{
			if (_lableRend == nullptr)
			{
				return;
			}			
			//箭头 + 文字组合类型图标渲染对象 （局部坐标系）
			if (is_same<T, MDataLable::Arrow_Font_Axis_Implicit>::value)
			{
				_lableRend->_arrowRend_axis_i->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_axis_i->updateSingleRend(type, name);
			}
			//箭头 + 文字组合类型图标渲染对象 （局部坐标系）
			else if (is_same<T, MDataLable::Arrow_Font_Axis_Explicit>::value)
			{
				_lableRend->_arrowRend_axis_e->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_axis_e->updateSingleRend(type, name);
			}
			//箭头 + 文字组合类型图标渲染对象 （局部坐标系）
			else if (is_same<T, MDataLable::Arrow_Font_Axis_Magnet>::value)
			{
				_lableRend->_arrowRend_axis_mag->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_axis_mag->updateSingleRend(type, name);
			}
			//箭头 + 文字组合类型图标渲染对象 （集中力、重力、力矩、线载荷、压强、预紧力、离心力）
			else if (is_same<T, MDataLable::Arrow_Font_Load_Implicit>::value)
			{
				_lableRend->_arrowRend_load_i->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_load_i->updateSingleRend(type, name);
			}
			//箭头 + 文字组合类型图标渲染对象 （初始速度）
			else if (is_same<T, MDataLable::Arrow_Font_InitCondition_Explicit>::value)
			{
				_lableRend->_arrowRend_initCondition_e->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_initCondition_e->updateSingleRend(type, name);
			}
			//箭头 + 文字组合类型图标渲染对象 （点载荷、面载荷、重力）
			else if (is_same<T, MDataLable::Arrow_Font_Load_Explicit>::value)
			{
				_lableRend->_arrowRend_load_e->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_load_e->updateSingleRend(type, name);
			}
			//箭头 + 文字组合类型图标渲染对象 （边 电荷/电流   磁化强度）
			else if (is_same<T, MDataLable::Arrow_Font_Load_Magnet>::value)
			{
				_lableRend->_arrowRend_load_mag->updateSingleRend(type, name);
				_lableRend->_arrowFontRend_load_mag->updateSingleRend(type, name);
			}
			//箭头类型图标渲染对象 （滑移）
			else if (is_same<T, MDataLable::Arrow_BC_Explicit>::value)
			{
				_lableRend->_arrowRend_BC_e->updateSingleRend(type, name);		
			}
			//箭头类型图标渲染对象 （向量）
			else if (is_same<T, MDataLable::Arrow_Vector_Explicit>::value)
			{
				_lableRend->_arrowRend_vector_e->updateSingleRend(type, name);
			}
			//箭头类型图标渲染对象 （磁矢量位）
			else if (is_same<T, MDataLable::Arrow_BC_Magnet>::value)
			{
				_lableRend->_arrowRend_BC_mag->updateSingleRend(type, name);
			}
			//箭头类型图标渲染对象 （材料铺层坐标系）
			else if (is_same<T, MDataLable::Arrow_PlyCoordinate_Implicit>::value)
			{
				_lableRend->_arrowRend_plyCoordinate_i->updateSingleRend(type, name);
			}
			//三角形+文字 组合类型图标渲染对象 （热流密度，电荷密度）
			else if (is_same<T, MDataLable::Triangle_Font_Load_Implicit>::value)
			{
				_lableRend->_triangleRend_load_i->updateSingleRend(type, name);
				_lableRend->_triangleFontRend_load_i->updateSingleRend(type, name);
			}
			//三角形+文字 组合类型图标渲染对象 （初始应力、SPH初始应力、初始炸点）
			else if (is_same<T, MDataLable::Triangle_Font_InitCondition_Explicit>::value)
			{
				_lableRend->_triangleRend_initCondition_e->updateSingleRend(type, name);
				_lableRend->_triangleFontRend_initCondition_e->updateSingleRend(type, name);
			}
			//三角形+文字 组合类型图标渲染对象 （表面电荷密度 表面电流密度 表面磁通密度 表面电场强度）
			else if (is_same<T, MDataLable::Triangle_Font_Load_Magnet>::value)
			{
				_lableRend->_triangleRend_load_mag->updateSingleRend(type, name);
				_lableRend->_triangleFontRend_load_mag->updateSingleRend(type, name);
			}
			//菱形图标+文字 组合类型图标渲染对象（对流换热，热辐射）
			else if (is_same<T, MDataLable::Rhombus_Font_Load_Implicit>::value)
			{
				_lableRend->_rhombusRend_load_i->updateSingleRend(type, name);
				_lableRend->_rhombusFontRend_load_i->updateSingleRend(type, name);
			}
			//十字图标+文字 组合类型图标渲染对象(内部热源,温度载荷)
			else if (is_same<T, MDataLable::X_Font_Load_Implicit>::value)
			{
				_lableRend->_XRend_load_i->updateSingleRend(type, name);
				_lableRend->_XFontRend_load_i->updateSingleRend(type, name);
			}
			//闪电型图标+文字 组合类型图标渲染对象(电荷)
			else if (is_same<T, MDataLable::Lightning_Font_Load_Implicit>::value)
			{
				_lableRend->_lightningRend_load_i->updateSingleRend(type, name);
				_lableRend->_lightningFontRend_load_i->updateSingleRend(type, name);
			}
			//闪电型图标+文字 组合类型图标渲染对象(节点/表面/体 电荷   节点/表面 电流)
			else if (is_same<T, MDataLable::Lightning_Font_Load_Magnet>::value)
			{
				_lableRend->_lightningRend_load_mag->updateSingleRend(type, name);
				_lableRend->_lightningFontRend_load_mag->updateSingleRend(type, name);
			}
			//自由度约束类型 (固定 对称 位移转角 初始条件 虚拟约束)
			else if (is_same<T, MDataLable::DF_BC_Implicit>::value)
			{
				_lableRend->_dfRend_BC_i->updateSingleRend(type, name);
			}
			//自由度约束类型 (刚体载荷)
			else if (is_same<T, MDataLable::DF_Load_Explicit>::value)
			{
				_lableRend->_dfRend_load_e->updateSingleRend(type, name);
			}
			//自由度约束类型 (位移 速度 加速度)
			else if (is_same<T, MDataLable::DF_BC_Explicit>::value)
			{
				_lableRend->_dfRend_BC_e->updateSingleRend(type, name);
			}
			//自由度约束类型 (刚体运动)
			else if (is_same<T, MDataLable::DF_Connect_Explicit>::value)
			{
				_lableRend->_dfRend_connect_e->updateSingleRend(type, name);
			}
			//房屋型 类型（初始边界条件(初始温度），温度边界，电势））
			else if (is_same<T, MDataLable::Home_BC_Implicit>::value)
			{
				_lableRend->_homeRend_BC_i->updateSingleRend(type, name);
			}
			//房屋型 类型（标量电位）
			else if (is_same<T, MDataLable::Home_BC_Magnet>::value)
			{
				_lableRend->_homeRend_BC_mag->updateSingleRend(type, name);
			}

			//直线+文字 类型（刚性、柔性、mpc连接）
			else if (is_same<T, MDataLable::Line_Connect_Implicit>::value)
			{
				_lableRend->_lineRend_connect_i->updateSingleRend(type, name);
				_lableRend->_lineFontRend_connect_i->updateSingleRend(type, name);
			}
			//直线+文字 类型（电焊、节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮-齿条传动、对接焊缝、角焊缝、铆接、运动耦合、
			//分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度）
			else if (is_same<T, MDataLable::Line_Connect_Explicit>::value)
			{
				_lableRend->_lineRend_connect_e->updateSingleRend(type, name);
				_lableRend->_lineFontRend_connect_e->updateSingleRend(type, name);
			}
			//主从面 类型（连接中面面绑定）
			else if (is_same<T, MDataLable::FaceToFace_Connect_Implicit>::value)
			{
				_lableRend->_faceTofaceRend_connect_i->updateSingleRend(type, name);
			}
			//主从面 类型（连接中面面绑定、失效绑定）
			else if (is_same<T, MDataLable::FaceToFace_Connect_Explicit>::value)
			{
				_lableRend->_faceTofaceRend_connect_e->updateSingleRend(type, name);
			}
			//主从面 类型（接触中 点对面 面对面）
			else if (is_same<T, MDataLable::FaceToFace_Contact_Explicit>::value)
			{
				_lableRend->_faceTofaceRend_contact_e->updateSingleRend(type, name);
			}
			//主从面 类型（接触）
			else if (is_same<T, MDataLable::FaceToFace_Contact_Implicit>::value)
			{
				_lableRend->_faceTofaceRend_contact_i->updateSingleRend(type, name);
			}
			//刚体连接
			else if (is_same<T, MDataLable::Type1_RC_Implicit>::value)
			{
				_lableRend->_RCRend_type1_i->updateSingleRend(type, name);
			}
			else if (is_same<T, MDataLable::Type2_RC_Implicit>::value)
			{
				_lableRend->_RCRend_type2_i->updateSingleRend(type, name);
			}
			//直线类型（弹簧 阻尼）
			else if (is_same<T, MDataLable::Line_RC_Implicit>::value)
			{
				_lableRend->_lineRend_RC_i->updateSingleRend(type, name);
			}
			//圆球铰副（圆球铰副）
			else if (is_same<T, MDataLable::Sphere_RC_Implicit>::value)
			{
				_lableRend->_sphereRend_RC_i->updateSingleRend(type, name);
			}
			//刚体约束
			else if (is_same<T, MDataLable::DF_RBC_Implicit>::value)
			{
				_lableRend->_dfRend_RBC_i->updateSingleRend(type, name);
			}
			//刚性墙（线）
			else if (is_same<T, MDataLable::Line_RW_Explicit>::value)
			{
				_lableRend->_lineRend_RW_e->updateSingleRend(type, name);
			}
			//刚性墙（面）
			else if (is_same<T, MDataLable::Face_RW_Explicit>::value)
			{
				_lableRend->_faceRend_RW_e->updateSingleRend(type, name);
			}
			//刚性墙（坐标系）
			else if (is_same<T, MDataLable::Axis_RW_Explicit>::value)
			{
				_lableRend->_axisRend_RW_e->updateSingleRend(type, name);
				_lableRend->_axisFontRend_RW_e->updateSingleRend(type, name);
			}
			//盒子（面）
			else if (is_same<T, MDataLable::Face_Box_Explicit>::value)
			{
				_lableRend->_faceRend_box_e->updateSingleRend(type, name);	
			}
			//盒子（线）
			else if (is_same<T, MDataLable::Line_Box_Explicit>::value)
			{
				_lableRend->_lineRend_box_e->updateSingleRend(type, name);
			}
			emit updateView();
		}
		/*
		* 更新同一类型所有渲染
		*/
		void updateAllRend_SameType(const AnalysisModule module, const LableOperateType type, const LableOperateMode mode)
		{
			if (_lableRend == nullptr)
			{
				return;
			}
			if (module == ImplicitAnalysis)
			{
				switch (type)
				{
				case LableOperateType_LocalAxis:
					_lableRend->_arrowRend_axis_i->updateAllRend(mode);
					_lableRend->_arrowFontRend_axis_i->updateAllRend(mode);
					break;
				case LableOperateType_Load:
					_lableRend->_arrowRend_load_i->updateAllRend(mode);
					_lableRend->_arrowFontRend_load_i->updateAllRend(mode);
					_lableRend->_triangleRend_load_i->updateAllRend(mode);
					_lableRend->_triangleFontRend_load_i->updateAllRend(mode);
					_lableRend->_rhombusRend_load_i->updateAllRend(mode);
					_lableRend->_rhombusFontRend_load_i->updateAllRend(mode);
					_lableRend->_XRend_load_i->updateAllRend(mode);
					_lableRend->_XFontRend_load_i->updateAllRend(mode);
					_lableRend->_lightningRend_load_i->updateAllRend(mode);
					_lableRend->_lightningFontRend_load_i->updateAllRend(mode);
					break;
				case LableOperateType_BC:
					_lableRend->_dfRend_BC_i->updateAllRend(mode);
					_lableRend->_homeRend_BC_i->updateAllRend(mode);
					break;
				case LableOperateType_Connect:
					_lableRend->_lineRend_connect_i->updateAllRend(mode);
					_lableRend->_lineFontRend_connect_i->updateAllRend(mode);
					_lableRend->_faceTofaceRend_connect_i->updateAllRend(mode);
					break;
				case LableOperateType_Contact:
					_lableRend->_faceTofaceRend_contact_i->updateAllRend(mode);
					break;
				case LableOperateType_RigidConnect:
					_lableRend->_RCRend_type1_i->updateAllRend(mode);
					_lableRend->_RCRend_type2_i->updateAllRend(mode);
					_lableRend->_lineRend_RC_i->updateAllRend(mode);
					_lableRend->_sphereRend_RC_i->updateAllRend(mode);
					break;
				case LableOperateType_RigidConstraint:
					_lableRend->_dfRend_RBC_i->updateAllRend(mode);
					break;
				case LableOperateType_PlyCoordinate:
					_lableRend->_arrowRend_plyCoordinate_i->updateAllRend(mode);
				default:
					break;
				}
			}
			else if (module == ExplicitAnalysis)
			{
				switch (type)
				{
				case LableOperateType_LocalAxis:
					_lableRend->_arrowRend_axis_e->updateAllRend(mode);
					_lableRend->_arrowFontRend_axis_e->updateAllRend(mode);
					break;
				case LableOperateType_Load:
					_lableRend->_arrowRend_load_e->updateAllRend(mode);
					_lableRend->_arrowFontRend_load_e->updateAllRend(mode);
					_lableRend->_dfRend_load_e->updateAllRend(mode);
					break;
				case LableOperateType_BC:
					_lableRend->_dfRend_BC_e->updateAllRend(mode);
					_lableRend->_arrowRend_BC_e->updateAllRend(mode);
					break;
				case LableOperateType_Connect:
					_lableRend->_lineRend_connect_e->updateAllRend(mode);
					_lableRend->_lineFontRend_connect_e->updateAllRend(mode);
					_lableRend->_faceTofaceRend_connect_e->updateAllRend(mode);
					_lableRend->_dfRend_connect_e->updateAllRend(mode);
					break;
				case LableOperateType_Contact:
					_lableRend->_faceTofaceRend_contact_e->updateAllRend(mode);
					break;
				case LableOperateType_InitCondition:
					_lableRend->_arrowRend_initCondition_e->updateAllRend(mode);
					_lableRend->_arrowFontRend_initCondition_e->updateAllRend(mode);
					_lableRend->_triangleRend_initCondition_e->updateAllRend(mode);
					_lableRend->_triangleFontRend_initCondition_e->updateAllRend(mode);
					break;
				case LableOperateType_RigidWall:
					_lableRend->_lineRend_RW_e->updateAllRend(mode);
					_lableRend->_faceRend_RW_e->updateAllRend(mode);
					_lableRend->_axisRend_RW_e->updateAllRend(mode);
					_lableRend->_axisFontRend_RW_e->updateAllRend(mode);
					break;
				case LableOperateType_Box:
					_lableRend->_faceRend_box_e->updateAllRend(mode);
					_lableRend->_lineRend_box_e->updateAllRend(mode);
					break;
				case LableOperateType_Vector:
					_lableRend->_arrowRend_vector_e->updateAllRend(mode);

					break;
				default:
					break;
				}
			}
			else if (module == MagnetAnalysis)
			{
				switch (type)
				{
				case LableOperateType_Load:
					_lableRend->_arrowRend_load_mag->updateAllRend(mode);
					_lableRend->_arrowFontRend_load_mag->updateAllRend(mode);
					_lableRend->_triangleRend_load_mag->updateAllRend(mode);
					_lableRend->_triangleFontRend_load_mag->updateAllRend(mode);
					_lableRend->_lightningRend_load_mag->updateAllRend(mode);
					_lableRend->_lightningFontRend_load_mag->updateAllRend(mode);
					break;
				case LableOperateType_BC:
					_lableRend->_arrowRend_BC_mag->updateAllRend(mode);
					_lableRend->_homeRend_BC_mag->updateAllRend(mode);
					break;
				case LableOperateType_LocalAxis:
					_lableRend->_arrowRend_axis_mag->updateAllRend(mode);
					_lableRend->_arrowFontRend_axis_mag->updateAllRend(mode);
					break;
				default:
					break;
				}
			}
			
			emit updateView();
		}


	private:
		//获取圆柱体顶点数据
		void getCylinderVertex(QVector<QVector3D> &ShapePointPos, QVector<QVector3D> &linePointPos, QVector3D pos, QVector3D normVec, QVector3D rVec, float r, float L, float angle)
		{
			rVec.normalized();
			normVec.normalized();
			QVector<QVector3D> face1, face2, face3;
			float cur_Angle = 0;
			while (cur_Angle < 360)
			{
				cur_Angle += angle;

				QMatrix4x4 matrixRotate;
				matrixRotate.setToIdentity();
				matrixRotate.rotate(cur_Angle, normVec);
				QVector4D tmp = QVector4D(rVec, 1.0) * matrixRotate;
				QVector3D next_rVec = QVector3D(tmp.x(), tmp.y(), tmp.z());
				next_rVec.normalized();

				//求出三角形坐标
				QVector3D pos1 = pos;
				QVector3D pos2;
				if (face1.empty())
				{
					pos2 = pos + rVec * r;
				}
				else
				{
					pos2 = face1.back();
				}
				QVector3D pos3 = pos + next_rVec * r;
				//顶面
				face1.push_back(pos1);
				face1.push_back(pos1);
				face1.push_back(pos2);
				face1.push_back(pos3);
				linePointPos.push_back(pos2);
				linePointPos.push_back(pos3);
				//底面
				face2.push_back(pos1 - normVec * L);
				face2.push_back(pos1 - normVec * L);
				face2.push_back(pos2 - normVec * L);
				face2.push_back(pos3 - normVec * L);
				linePointPos.push_back(pos2 - normVec * L);
				linePointPos.push_back(pos3 - normVec * L);
				//侧面线
				if (cur_Angle - angle == 0 || cur_Angle - angle == 180)
				{
					linePointPos.push_back(pos2);
					linePointPos.push_back(pos2 - normVec * L);

				}

				//侧面
				int id = face1.size() - 1;
				face3.push_back(face1[id - 1]);		
				face3.push_back(face2[id - 1]);		
				face3.push_back(face2[id]);
				face3.push_back(face1[id]);



			}
			ShapePointPos = face1+face2+face3;
		}
		//获取球体顶点数据
		void getSphereVertex(QVector<QVector3D>& shperePointPos, QVector<QVector3D>& linePointPos, QVector3D pos, double r, double angle)
		{
			QVector<QVector3D> curPos;
			QVector<QVector3D> lastPos;
			for (double i = 0; i <= 180; i += angle)
			{
				curPos.clear();
				double curR = r * sin(i / 180 * PI); //此时圆的半径
				double z = pos.z() + r * cos(i / 180 * PI);  //z坐标
				for (double j = 0; j <= 360; j += angle)
				{
					double x = pos.x() + curR * cos(j / 180 * PI);
					double y = pos.y() + curR * sin(j / 180 * PI);
					QVector3D onePoint = QVector3D(x, y, z);
					curPos.push_back(onePoint);
				}

				if (!lastPos.isEmpty())
				{
					for (int j = 0; j < lastPos.size() - 1; ++j)
					{
						//添加轮廓1
						if (i == 90)
						{
							linePointPos.push_back(curPos[j]);
							linePointPos.push_back(curPos[j + 1]);
						}
						//添加轮廓2
						if (j*angle == 0 || j * angle == 180)
						{
							linePointPos.push_back(lastPos[j]);
							linePointPos.push_back(curPos[j]);
						}
						//矩形顶点数据
						shperePointPos.push_back(lastPos[j]);
						shperePointPos.push_back(lastPos[j + 1]);
						shperePointPos.push_back(curPos[j + 1]);
						shperePointPos.push_back(curPos[j]);
					}
				}
				lastPos = curPos;
			}


		}
		//渲染对象ID
		int _ID = 0;

	signals:
		void updateView();


	private:
		//总渲染对象
		shared_ptr<mLableRendIntegrate_pre> _lableRend = nullptr;
	};
}
