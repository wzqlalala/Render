//////////////////////////////////////////////////////////////////////
//����������Ⱦ �߽�������Լ�����ɶȵ�ͼ�꣨��ͷ��ϣ�/////////////////
//////////////////////////////////////////////////////////////////////
#pragma once
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
#include"mLableShapeVertex.h"
using namespace MDataLable;
namespace MLableRend
{
	//��ͷ�������ͼ����ƣ����ɶ�Լ����
	template<class T>
	class mLableRendBCDF : public mLableRendBase<mDrawElement>
	{
	public:
		explicit mLableRendBCDF(const QHash<QString, T> &data) :mLableRendBase<mDrawElement>(8, GL_LINES), _data(data) {};
		virtual ~mLableRendBCDF() override {};
		//λ�� �ٶ� ���ٶȱ߽�����ͼ�����״����
		enum ShapeType
		{
			Cone_0,	    //�̶�Բ׶
			Cylinder_0, //�̶�Բ��
			Cone_Line,  //�ǹ̶�Բ׶���ߣ���ͷ��
			Cylinder,   //�ǹ̶�Բ��

		};
		/*
		* ����ͼ�����
		*/
		virtual void updateSingleRend(LableOperateMode mode, const QString &name) override
		{
			auto tmp = _data.value(name);
			int ID = tmp.lableData.getLableID();
			bool showState = tmp.lableData.getLableShowState();
			bool highlightState = tmp.lableData.getLableHighlightState();
			bool depthState = tmp.lableData.getLableDepthState();
			float size = tmp.lableData.getLableSize();
			QVector3D color_light = tmp.lableData.getLableColor(); //ǳɫ
			QVector3D color_deep = color_light / 2;				//��ɫ
			QVector<QVector3D> pos = tmp.lableData.getParameter1();
			QMap<unsigned int, float> value = tmp.lableData.getParameter2();
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;
				//�����������λ��
				int indexBegin = _rendObject->_indexNum;

				//��ȡ��״�Ķ�������
				const QVector<QVector3D> &Cone_Vertices_0 = mLableShapeVertex::getInstance()->_Cone_Vertices_0;
				const QVector<QVector3D> &Cylinder_Vertices_0 = mLableShapeVertex::getInstance()->_Cylinder_Vertices_0;
				const QVector<QVector3D> &Cone_Line_Vertices = mLableShapeVertex::getInstance()->_Cone_Line_Vertices;
				const QVector<QVector3D> &Cylinder_Vertices = mLableShapeVertex::getInstance()->_Cylinder_Vertices;

				//��Ӷ��������Լ�����
				for (int i = 0; i < pos.size(); i++)
				{
					for (auto it = value.begin(); it != value.end(); it++)
					{
						if (it.key() == 1)
						{
							if (it.value() == 0)
							{
								//�ж�1=0ʱ �Ƿ�4Ҳ����0
								if (value.count(4) != 0 && value.value(4) == 0)
								{

									//Բ׶_0	
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180����ǳɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


									//Բ��_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180������ɫ��
									appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


								}
								else
								{
									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180����ǳɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));

								}

							}
							else if (it.value() < 0)
							{
								//�ж�1<0ʱ �Ƿ�4Ҳ<0
								if (value.count(4) != 0 && value.value(4) < 0)
								{
									//Բ׶+ֱ��(��ͷ)	
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180����ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


									//Բ��		
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180������ɫ��
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));

								}
								else
								{
									//Բ׶+ֱ��(��ͷ)	
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180����ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));
								}
							}
							else
							{
								//�ж�1>0ʱ �Ƿ�4Ҳ>0
								if (value.count(4) != 0 && value.value(4) > 0)
								{
									//Բ׶+ֱ��(��ͷ)	
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������ת����ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));


									//Բ��		
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//��Ӷ������� ������ת������ɫ��
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

								}
								else
								{
									//Բ׶+ֱ��(��ͷ)	
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������ת����ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));

								}

							}

						}
						else if (it.key() == 2)
						{
							if (it.value() == 0)
							{
								//�ж�2=0ʱ �Ƿ�5Ҳ����0
								if (value.count(5) != 0 && value.value(5) == 0)
								{
									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


									//Բ��_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180������ɫ��
									appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));

								}
								else
								{

									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

								}

							}
							else if (it.value() < 0)
							{
								//�ж�2<0ʱ �Ƿ�5Ҳ<0
								if (value.count(5) != 0 && value.value(5) < 0)
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


									//Բ��
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת-90�㣩����ɫ��
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


								}
								else
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

								}

							}
							else
							{
								//�ж�2>0ʱ �Ƿ�5Ҳ>0
								if (value.count(5) != 0 && value.value(5) > 0)
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));


									//Բ��
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת90�㣩����ɫ��
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

								}
								else
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));
								}
							}

						}
						else if (it.key() == 3)
						{
							if (it.value() == 0)
							{
								//�ж�3=0ʱ �Ƿ�6Ҳ����0
								if (value.count(6) != 0 && value.value(6) == 0)
								{
									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90����ǳɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


									//Բ��_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90������ɫ��
									appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));
								}
								else
								{
									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90����ǳɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

								}

							}
							else if (it.value() < 0)
							{
								//�ж�3<0ʱ �Ƿ�6Ҳ<0
								if (value.count(6) != 0 && value.value(6) < 0)
								{

									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


									//Բ��
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90�㣩����ɫ��
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


								}
								else
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

								}

							}
							else
							{
								//�ж�3>0ʱ �Ƿ�6Ҳ>0
								if (value.count(6) != 0 && value.value(6) > 0)
								{

									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת-90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, -90));


									//Բ��
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת-90�㣩����ɫ��
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));



								}
								else
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת-90�㣩��ǳɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, -90));
								}
							}
						}
						else if (it.key() == 4)
						{
							if (it.value() == 0 && (value.count(1) == 0 || value.value(1) != 0)) //�ж�ǰ���Ƿ��Ѿ�����1,4�����ڵ����
							{

								//Բ׶_0
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_0, _rendObject->_vertexNum);
								//��Ӷ������� ������Z����ת180�㣩����ɫ��
								appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


							}
							else if (it.value() < 0 && (value.count(1) == 0 || value.value(1) >= 0))
							{

								//Բ׶+ֱ��(��ͷ)
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//��Ӷ������� ������Z����ת180������ɫ��
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


							}
							else if (it.value() > 0 && (value.count(1) == 0 || value.value(1) <= 0))
							{
								//Բ׶+ֱ��(��ͷ)
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//��Ӷ������� ������ת������ɫ��
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

							}
						}
						else if (it.key() == 5)
						{
							if (it.value() == 0 && (value.count(2) == 0 || value.value(2) != 0))
							{
								//Բ׶_0
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_0, _rendObject->_vertexNum);
								//��Ӷ������� ������Z����ת - 90������ɫ��
								appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


							}
							else if (it.value() < 0 && (value.count(2) == 0 || value.value(2) >= 0))
							{
								//Բ׶+ֱ��(��ͷ)
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//��Ӷ������� ������Z����ת - 90������ɫ��
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


							}
							else if (it.value() > 0 && (value.count(2) == 0 || value.value(2) <= 0))
							{
								//Բ׶+ֱ��(��ͷ)
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//��Ӷ������� ������Z����ת 90������ɫ��
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

							}
						}
						else if (it.key() == 6)
						{
							if (it.value() == 0 && (value.count(3) == 0 || value.value(3) != 0))
							{
								//Բ׶_0
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_0, _rendObject->_vertexNum);
								//��Ӷ������� ������Y����ת90�㣩����ɫ��
								appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


							}
							else if (it.value() < 0 && (value.count(3) == 0 || value.value(3) >= 0))
							{
								//Բ׶+ֱ��(��ͷ)
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//��Ӷ������� ������Y����ת90�㣩����ɫ��
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


							}
							else if (it.value() > 0 && (value.count(3) == 0 || value.value(3) <= 0))
							{
								//Բ׶+ֱ��(��ͷ)
								//��� Բ׶_0 EBO����
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//��Ӷ������� ������Y����ת-90�㣩����ɫ��
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));

							}
						}
					}
				}

				//���¶�����������
				_rendObject->_indexNum = _EBOindex.size();
				//���ݼ�ͷ���������ID������ ����ɾ������
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				_elementIndex[ID] = { indexBegin ,(int)_EBOindex.size()- indexBegin };

				//����buffer
				initialBuffer_append_index<float, GLshort>(3, 4, _vertex_Pos, _vertex_Rotate, vertexBegin, _EBOindex);
				//�������
				deleteVertex();
			}
			else if (mode == Delete)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end() || _elementIndex.find(ID) == _elementIndex.end())
				{
					return;
				}
				int deleteVertexBegin = _vertexIndex[ID].first;
				int deleteVertexSize = _vertexIndex[ID].second;

				int deleteIndexBegin = _elementIndex[ID].first;
				int deleteIndexSize = _elementIndex[ID].second;
				//���¶�������
				_rendObject->_vertexNum -= deleteVertexSize;
				//������������
				_rendObject->_indexNum -= deleteIndexSize;

				//��������_EBOIndex
				vector<GLuint> tmpEBOIndex;
				for (int i = 0; i < _EBOindex.size(); ++i)
				{
					if (i < deleteIndexBegin)
					{
						tmpEBOIndex.push_back(_EBOindex[i]);
					}
					else if (i >= deleteIndexBegin + deleteIndexSize)
					{
						tmpEBOIndex.push_back(_EBOindex[i] - deleteVertexSize);
					}
				}
				swap(tmpEBOIndex,_EBOindex);

				//������Ⱦ
				initialBuffer_delete_index<float, GLshort>(3, 4, deleteVertexBegin, deleteVertexSize,_EBOindex);

				_vertexIndex.remove(ID);
				_elementIndex.remove(ID);
				//��������
				for (auto it = _vertexIndex.begin(); it != _vertexIndex.end(); ++it)
				{
					if (it.value().first > deleteVertexBegin)
					{
						_vertexIndex[it.key()] = { it.value().first - deleteVertexSize, it.value().second };
					}
				}
				for (auto it = _elementIndex.begin(); it != _elementIndex.end(); ++it)
				{
					if (it.value().first > deleteIndexBegin)
					{
						_elementIndex[it.key()] = { it.value().first - deleteIndexSize, it.value().second };
					}
				}


			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight || mode == SetColor || mode == SetSize)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end() || _elementIndex.find(ID) == _elementIndex.end())
				{
					return;
				}
				int updateVertexBegin = _vertexIndex[ID].first;
				int updateVertexSize = _vertexIndex[ID].second;
				if (mode == Show || mode == Highlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 1);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);
				}
				else if (mode == Hide || mode == NoHighlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 0);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);
				}
				else if (mode == SetColor)
				{
					vector<GLubyte> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(color_light.x());
						tmpVertex.push_back(color_light.y());
						tmpVertex.push_back(color_light.z());
					}
					initialBuffer_update<GLubyte>(mode, 3, tmpVertex, updateVertexBegin, updateVertexSize);
				}
				else if (mode == SetSize)
				{
					vector<float> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
					initialBuffer_update<float>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);
				}

			}
		}
		/*
		* ����ͼ�����
		*/
		virtual void updateAllRend(LableOperateMode mode) override
		{
			if (mode == Append)
			{
				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;
				//�����������λ��
				int indexBegin = _rendObject->_indexNum;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					int ID = tmp.lableData.getLableID();
					bool showState = tmp.lableData.getLableShowState();
					bool highlightState = tmp.lableData.getLableHighlightState();
					bool depthState = tmp.lableData.getLableDepthState();
					float size = tmp.lableData.getLableSize();
					QVector3D color_light = tmp.lableData.getLableColor(); //ǳɫ
					QVector3D color_deep = color_light / 2;				//��ɫ
					QVector<QVector3D> pos = tmp.lableData.getParameter1();
					QMap<unsigned int, float> value = tmp.lableData.getParameter2();

					//��ȡ��״�Ķ�������
					const QVector<QVector3D> &Cone_Vertices_0 = mLableShapeVertex::getInstance()->_Cone_Vertices_0;
					const QVector<QVector3D> &Cylinder_Vertices_0 = mLableShapeVertex::getInstance()->_Cylinder_Vertices_0;
					const QVector<QVector3D> &Cone_Line_Vertices = mLableShapeVertex::getInstance()->_Cone_Line_Vertices;
					const QVector<QVector3D> &Cylinder_Vertices = mLableShapeVertex::getInstance()->_Cylinder_Vertices;
					
					int tmpVertexBegin = _rendObject->_vertexNum;
					int tmpElementBegin = _rendObject->_indexNum;

					//��Ӷ��������Լ�����
					for (int i = 0; i < pos.size(); i++)
					{
						for (auto it = value.begin(); it != value.end(); it++)
						{
							if (it.key() == 1)
							{
								if (it.value() == 0)
								{
									//�ж�1=0ʱ �Ƿ�4Ҳ����0
									if (value.count(4) != 0 && value.value(4) == 0)
									{

										//Բ׶_0	
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180����ǳɫ��
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


										//Բ��_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180������ɫ��
										appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


									}
									else
									{
										//Բ׶_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180����ǳɫ��
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));

									}

								}
								else if (it.value() < 0)
								{
									//�ж�1<0ʱ �Ƿ�4Ҳ<0
									if (value.count(4) != 0 && value.value(4) < 0)
									{
										//Բ׶+ֱ��(��ͷ)	
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180����ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


										//Բ��		
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180������ɫ��
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));

									}
									else
									{
										//Բ׶+ֱ��(��ͷ)	
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180����ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));
									}
								}
								else
								{
									//�ж�1>0ʱ �Ƿ�4Ҳ>0
									if (value.count(4) != 0 && value.value(4) > 0)
									{
										//Բ׶+ֱ��(��ͷ)	
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������ת����ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));


										//Բ��		
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//��Ӷ������� ������ת������ɫ��
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

									}
									else
									{
										//Բ׶+ֱ��(��ͷ)	
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������ת����ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));

									}

								}

							}
							else if (it.key() == 2)
							{
								if (it.value() == 0)
								{
									//�ж�2=0ʱ �Ƿ�5Ҳ����0
									if (value.count(5) != 0 && value.value(5) == 0)
									{
										//Բ׶_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


										//Բ��_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת180������ɫ��
										appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));

									}
									else
									{

										//Բ׶_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

									}

								}
								else if (it.value() < 0)
								{
									//�ж�2<0ʱ �Ƿ�5Ҳ<0
									if (value.count(5) != 0 && value.value(5) < 0)
									{
										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


										//Բ��
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת-90�㣩����ɫ��
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


									}
									else
									{
										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת-90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

									}

								}
								else
								{
									//�ж�2>0ʱ �Ƿ�5Ҳ>0
									if (value.count(5) != 0 && value.value(5) > 0)
									{
										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));


										//Բ��
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת90�㣩����ɫ��
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

									}
									else
									{
										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Z����ת90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));
									}
								}

							}
							else if (it.key() == 3)
							{
								if (it.value() == 0)
								{
									//�ж�3=0ʱ �Ƿ�6Ҳ����0
									if (value.count(6) != 0 && value.value(6) == 0)
									{
										//Բ׶_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת90����ǳɫ��
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


										//Բ��_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת90������ɫ��
										appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));
									}
									else
									{
										//Բ׶_0
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת90����ǳɫ��
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

									}

								}
								else if (it.value() < 0)
								{
									//�ж�3<0ʱ �Ƿ�6Ҳ<0
									if (value.count(6) != 0 && value.value(6) < 0)
									{

										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


										//Բ��
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת90�㣩����ɫ��
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


									}
									else
									{
										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

									}

								}
								else
								{
									//�ж�3>0ʱ �Ƿ�6Ҳ>0
									if (value.count(6) != 0 && value.value(6) > 0)
									{

										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת-90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, -90));


										//Բ��
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת-90�㣩����ɫ��
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));



									}
									else
									{
										//Բ׶+ֱ��(��ͷ)
										//��� Բ׶_0 EBO����
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//��Ӷ������� ������Y����ת-90�㣩��ǳɫ��
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));
									}
								}
							}
							else if (it.key() == 4)
							{
								if (it.value() == 0 && (value.count(1) == 0 || value.value(1) != 0)) //�ж�ǰ���Ƿ��Ѿ�����1,4�����ڵ����
								{

									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180�㣩����ɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


								}
								else if (it.value() < 0 && (value.count(1) == 0 || value.value(1) >= 0))
								{

									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת180������ɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


								}
								else if (it.value() > 0 && (value.count(1) == 0 || value.value(1) <= 0))
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������ת������ɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

								}
							}
							else if (it.key() == 5)
							{
								if (it.value() == 0 && (value.count(2) == 0 || value.value(2) != 0))
								{
									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת - 90������ɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


								}
								else if (it.value() < 0 && (value.count(2) == 0 || value.value(2) >= 0))
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת - 90������ɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


								}
								else if (it.value() > 0 && (value.count(2) == 0 || value.value(2) <= 0))
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Z����ת 90������ɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

								}
							}
							else if (it.key() == 6)
							{
								if (it.value() == 0 && (value.count(3) == 0 || value.value(3) != 0))
								{
									//Բ׶_0
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90�㣩����ɫ��
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


								}
								else if (it.value() < 0 && (value.count(3) == 0 || value.value(3) >= 0))
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת90�㣩����ɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


								}
								else if (it.value() > 0 && (value.count(3) == 0 || value.value(3) <= 0))
								{
									//Բ׶+ֱ��(��ͷ)
									//��� Բ׶_0 EBO����
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//��Ӷ������� ������Y����ת-90�㣩����ɫ��
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));

								}
							}
						}
					}

					//���¶�����������
					_rendObject->_indexNum = (int)_EBOindex.size();

					int tmpVertexSize = _rendObject->_vertexNum - tmpVertexBegin;
					int tmpElementSize = _rendObject->_indexNum - tmpElementBegin;
					//���ݼ�ͷ���������ID������ ����ɾ������
					_vertexIndex[ID] = { tmpVertexBegin,tmpVertexSize };
					_elementIndex[ID] = { tmpElementBegin ,tmpElementSize };

	
				}
				//����buffer
				initialBuffer_append_index<float, GLshort>(3, 4, _vertex_Pos, _vertex_Rotate, vertexBegin, _EBOindex);
				//�������
				deleteVertex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				vector<GLuint>().swap(_EBOindex);
				//���¶�������
				_rendObject->_vertexNum = 0;
				//������������
				_rendObject->_indexNum = 0;
				//������Ⱦ
				initialBuffer_delete_index<float, GLshort>(3, 4, 0, deleteVertexSize, _EBOindex);
				_vertexIndex.clear();
				_elementIndex.clear();
			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight|| mode == SetColor|| mode == SetSize)
			{

				int updateVertexSize = _rendObject->_vertexNum;
				if (mode == Show || mode == Highlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 1);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, 0, updateVertexSize);
				}
				else if (mode == Hide || mode == NoHighlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 0);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, 0, updateVertexSize);
				}
				else if (mode == SetColor)
				{
					int updateVertexSize = _rendObject->_vertexNum;
					vector<GLubyte> tmpVertex;
					for (auto it = _data.begin(); it != _data.end(); ++it)
					{
						auto tmp = it.value();
						int ID = tmp.lableData.getLableID();
						QVector3D color = tmp.lableData.getLableColor();
						int vertexSize = _vertexIndex[ID].second;
						for (int i = 0; i < vertexSize; ++i)
						{
							tmpVertex.push_back(color.x());
							tmpVertex.push_back(color.y());
							tmpVertex.push_back(color.z());
						}
					}
					initialBuffer_update<GLubyte>(mode, 3, tmpVertex, 0, updateVertexSize);
				}
				else if (mode == SetSize)
				{
					int updateVertexSize = _rendObject->_vertexNum;
					vector<float> tmpVertex;
					for (auto it = _data.begin(); it != _data.end(); ++it)
					{
						auto tmp = it.value();
						int ID = tmp.lableData.getLableID();
						float size = tmp.lableData.getLableSize();
						int vertexSize = _vertexIndex[ID].second;
						for (int i = 0; i < vertexSize; ++i)
						{
							tmpVertex.push_back(size);
						}
					}
					initialBuffer_update<float>(mode, 1, tmpVertex, 0, updateVertexSize);
				}
			}
		}
		/*
		* ��Ӷ�������
		*/
		void appendVertex(const QVector<QVector3D> & shapeVertex, const bool highlightstate, const bool showstate, const bool depthstate, const float size,
			const QVector3D color, const QVector3D pos, const QVector4D rotate)
		{
			for (int i = 0; i < shapeVertex.size(); ++i)
			{
				appendCommonVertex(highlightstate, showstate, depthstate, size, color, shapeVertex[i]);
				//ͼ���λ�ã�ģ�������£�
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				//��ת�����ת�Ƕ�
				_vertex_Rotate.push_back(rotate.x());
				_vertex_Rotate.push_back(rotate.y());
				_vertex_Rotate.push_back(rotate.z());
				_vertex_Rotate.push_back(rotate.w());
			}
			_rendObject->_vertexNum += shapeVertex.size();

		}
		/*
		* ɾ����������
		*/
		inline void deleteVertex()
		{
			deleteCommonVerrtex();
			vector<float>().swap(_vertex_Pos);
			vector<GLshort>().swap(_vertex_Rotate);

			//ע�� ����ɾ��_EBOindex  
			//vector<GLuint>().swap(_EBOindex);


		}
		/*
		* ���EBO����
		*/
		inline void appendEBOIndex(ShapeType type, int begin)
		{
			//�̶�Բ׶
			if (type == Cone_0)
			{
				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 1);

				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 2);

				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 3);

				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 1);
				_EBOindex.push_back(begin + 2);

				_EBOindex.push_back(begin + 2);
				_EBOindex.push_back(begin + 3);

				_EBOindex.push_back(begin + 3);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 4);
				_EBOindex.push_back(begin + 1);
			}
			//�̶�Բ��
			else if (type == Cylinder_0)
			{
				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 1);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 2);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 3);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 4);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 5);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 6);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 7);
				_EBOindex.push_back(begin + 4);
			}
			//�ǹ̶� Բ׶��ֱ��
			else if (type == Cone_Line)
			{
				_EBOindex.push_back(begin);//0
				_EBOindex.push_back(begin + 1);//1		

				_EBOindex.push_back(begin + 1);//1
				_EBOindex.push_back(begin + 2);//2

				_EBOindex.push_back(begin + 1);//1
				_EBOindex.push_back(begin + 3);//3

				_EBOindex.push_back(begin + 1);//1
				_EBOindex.push_back(begin + 4);//4

				_EBOindex.push_back(begin + 1);//1
				_EBOindex.push_back(begin + 5);//5

				_EBOindex.push_back(begin + 2);//2
				_EBOindex.push_back(begin + 3);//3

				_EBOindex.push_back(begin + 3);//3
				_EBOindex.push_back(begin + 4);//4

				_EBOindex.push_back(begin + 4);//4
				_EBOindex.push_back(begin + 5);//5

				_EBOindex.push_back(begin + 5);//5
				_EBOindex.push_back(begin + 2);//2		
			}
			//�ǹ̶�Բ��
			else if (type == Cylinder)
			{
				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 1);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 2);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 3);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 4);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 5);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 6);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 7);
				_EBOindex.push_back(begin + 4);

			}
		}


	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
		//��������
		vector<float> _vertex_Pos;  //��Ҫ�ƶ�������
		vector<GLshort> _vertex_Rotate;	//��ת��ͽǶ�  ���磨1,0,0,180��  ����X����ת180��
		//EBO��������
		vector<GLuint> _EBOindex; //һֱ���� ������� ����ɾ������
		QHash<int, QPair<int, int>> _elementIndex;	//QHash<ID,��������ʼλ�ã������ĸ���>
	};
}
