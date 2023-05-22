#pragma once

#include"mLableRendBase.h"
#include"mLableDataStruct.h"
#include <QOpenGLWidget>
#include<memory>
//Qt
#include<qapplication.h>
#include<QVector>

using namespace MDataLable;
namespace MLableRend
{
    //�������ݴ洢�ṹ��
    struct Character
    {
        //�����������ε�����ԭ��ʹ�С
        QVector2D  Positon;		//������ԭ�㣨���½ǣ������ִ������ϵ���������
        QVector2D  Size;       // ���δ�С(���ش�С Width Height)

        //�������û������εĶ������ݣ����ּ����
        QVector2D  Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
        GLuint     Advance;    // ԭ�����һ������ԭ��ľ���

    };
    //��������������
    class mFontTexture: public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
    {
    public:
        mFontTexture()
        {
            this->initializeOpenGLFunctions();
            LoadFontTexture();
        }
        ~mFontTexture() {};
        /*
        * ��Ԫģ��
        */
        template<class T>
        friend class mLableRendFont;
		template<class T>
		friend class mLableRendFont2;
		template<class T>
		friend class mLableRendFont3;
        /*
        * ���������������������ּ��ص�һ�������ϣ�
        */
		void LoadFontTexture();
    
    private:
        //����
        GLuint _texture;
        //�������ݿ�
        QMap<GLchar, Character> _characters;
    };

    //��������ͼ�����(ͼ��+����ͬʱ����) ��ͼ�����һ��
    template<class T>
    class mLableRendFont :public mLableRendBase<mDrawTexture>
    {
    public:
        //�̶�ͼ������VBOnum = 9  ��ͷĩ�˵����� VBOnum = 10 mFontTexture ���ִ�������Ϣ��
        explicit mLableRendFont(const QHash<QString, T> &data, const int VBOnum, const shared_ptr<mFontTexture> fontTextrue) :
            mLableRendBase<mDrawTexture>(VBOnum, GL_QUADS), _data(data), _fontTexture(fontTextrue)
        {
			switch (VBOnum)
			{
			case 9: _fontType = FontType_FixShape;
				break;
			case 10: _fontType = FontType_Arrow;
				break;
			default:
				break;
			}
            //������
            _rendObject->_texture = fontTextrue->_texture;
        };

        virtual ~mLableRendFont() override {};
        /*
        * ����ͼ�����
        */
        virtual void updateSingleRend(LableOperateMode mode, const QString &name) override
        {
			auto tmp = this->_data[name];
			long long ID = tmp.fontData.getLableID();
			bool showState = tmp.fontData.getLableShowState();
			bool highlightState = tmp.fontData.getLableHighlightState();
			bool depthState = tmp.fontData.getLableDepthState();
			float size = tmp.fontData.getLableSize();
			QVector3D color = tmp.fontData.getLableColor();
            //�غ���ͼ��
            if (mode == Append)
            {
                if (_vertexIndex.contains(ID))
                {
                    updateSingleRend(Delete, name);
                }
                //������������Ҫ������
                int vertexBegin = _rendObject->_vertexNum;
                //��Ӷ�������
                auto data = tmp.fontData.getParameter();
				if (data.isEmpty()) return;
                appendFontVertex(name, data, highlightState, showState, depthState, size, color);
                //��������
                _vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
                //���¶�������
                _rendObject->_vertexNum += _vertex_HighlightState.size();
                //����buffer
				if (_fontType == FontType_Arrow)
                {
                    initialBuffer_append <GLushort, float, float, float>(2, 3, 1, 3, _vertex_TexCoord, _vertex_Pos, _vertex_LableSize, _vertex_Dir,  vertexBegin);
                }
				else if (_fontType == FontType_FixShape)
				{
					initialBuffer_append <GLushort, float, float>(2, 3, 1, _vertex_TexCoord, _vertex_Pos, _vertex_LableSize, vertexBegin);
				}
                else
                {
                    initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
                }
                //�������
                deleteSpecificVertex();
                deleteCommonVerrtex();
            }
            else if (mode == Delete)
            {
                if (_vertexIndex.find(ID) == _vertexIndex.end())
                {
                    return;
                }
                int deleteVertexBegin = _vertexIndex[ID].first;
                int deleteVertexSize = _vertexIndex[ID].second;
                //���¶�������
                _rendObject->_vertexNum -= deleteVertexSize;


                //������Ⱦ
				if (_fontType == FontType_Arrow)
                {
                    initialBuffer_delete<GLushort, float, float, float>(2, 3, 1, 3, deleteVertexBegin, deleteVertexSize);
                }
				else if (_fontType == FontType_FixShape)
				{
					initialBuffer_delete<GLushort, float, float>(2, 3, 1, deleteVertexBegin, deleteVertexSize);
				}
                else
                {
                    initialBuffer_delete<GLushort, float>(2, 3, deleteVertexBegin, deleteVertexSize);
                }

                _vertexIndex.remove(ID);

                //��������
                for (auto it = _vertexIndex.begin(); it != _vertexIndex.end(); ++it)
                {
                    if (it.value().first > deleteVertexBegin)
                    {
                        _vertexIndex[it.key()] = { it.value().first - deleteVertexSize, it.value().second };
                    }
                }
            }
            else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight || mode == SetColor || mode == SetSize)
            {
                if (_vertexIndex.find(ID) == _vertexIndex.end())
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
                        tmpVertex.push_back(color.x());
                        tmpVertex.push_back(color.y());
                        tmpVertex.push_back(color.z());
                    }
                    initialBuffer_update<GLubyte>(mode, 3, tmpVertex, updateVertexBegin, updateVertexSize);
                }
				else if (mode == SetSize)
				{
					//�������ִ�С
					vector<float> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
					initialBuffer_update<float>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);

					//��������λ�� ��ͷĩ������ �� �̶�����ͼ�����ֲ���
					if (_fontType == FontType_Arrow || _fontType == FontType_FixShape)
					{
						tmpVertex.clear();
						auto tmp = this->_data[name];
						float lableSize = tmp.lableData.getLableSize();
						for (int i = 0; i < updateVertexSize; ++i)
						{
							tmpVertex.push_back(lableSize);
						}

						initialBuffer_update<float>(1, tmpVertex, updateVertexBegin, updateVertexSize);
					}
				}

            };
        }

        /*
        * ����ͼ�����
        */
        virtual void updateAllRend(LableOperateMode mode) override
        {
            //�غ���ͼ��
            if (mode == Append)
            {
                //����������λ��
                int vertexBegin = _rendObject->_vertexNum;

                for (auto it = _data.begin(); it != _data.end(); ++it)
                {
					int vertexTmpBegin = _rendObject->_vertexNum;
                    auto tmp = it.value();
					long long ID = tmp.fontData.getLableID();
                    bool showState = tmp.fontData.getLableShowState();
                    bool highlightState = tmp.fontData.getLableHighlightState();
                    bool depthState = tmp.fontData.getLableDepthState();
                    float size = tmp.fontData.getLableSize();
                    QVector3D color = tmp.fontData.getLableColor();

                    //��Ӷ�������
                    auto data = tmp.fontData.getParameter();
					if (data.isEmpty()) continue;
                    appendFontVertex(it.key(), data, highlightState, showState, depthState, size, color);
                    //��������
                    _vertexIndex[ID] = { vertexTmpBegin,(int)_vertex_HighlightState.size()- vertexTmpBegin };
                    //���¶�������
                    _rendObject->_vertexNum += (int)_vertex_HighlightState.size()- vertexTmpBegin;
                }
                //����buffer
				if (_fontType == FontType_Arrow)
				{
					initialBuffer_append <GLushort, float, float, float>(2, 3, 1, 3, _vertex_TexCoord, _vertex_Pos, _vertex_LableSize, _vertex_Dir, vertexBegin);
				}
				else if (_fontType == FontType_FixShape)
				{
					initialBuffer_append <GLushort, float, float>(2, 3, 1, _vertex_TexCoord, _vertex_Pos, _vertex_LableSize, vertexBegin);
				}
				else
				{
					initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				}
                //�������
                deleteSpecificVertex();
                deleteCommonVerrtex();
            }
            else if (mode == Delete)
            {
                int deleteVertexSize = _rendObject->_vertexNum;
                //���¶�������
                _rendObject->_vertexNum = 0;

                //������Ⱦ
				if (_fontType == FontType_Arrow)
                {
                    initialBuffer_delete<GLushort, float, float, float>(2, 3, 1, 3, 0, deleteVertexSize);
                }
				else if (_fontType == FontType_FixShape)
				{
					initialBuffer_delete<GLushort, float, float>(2, 3, 1, 0, deleteVertexSize);
				}
                else
                {
                    initialBuffer_delete<GLushort, float>(2, 3, 0, deleteVertexSize);
                }
                _vertexIndex.clear();
            }
            else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight)
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
            }
			else if (mode == SetColor)
			{
				int updateVertexSize = _rendObject->_vertexNum;
				vector<GLubyte> tmpVertex;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					int ID = tmp.fontData.getLableID();
					QVector3D color = tmp.fontData.getLableColor();
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
					int ID = tmp.fontData.getLableID();
					float size = tmp.fontData.getLableSize();
					int vertexSize = _vertexIndex[ID].second;
					for (int i = 0; i < vertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
				}
				initialBuffer_update<float>(mode, 1, tmpVertex, 0, updateVertexSize);

				//��������λ�� ��ͷĩ������ �� �̶�����ͼ�����ֲ���
				if (_fontType == FontType_Arrow || _fontType == FontType_FixShape)
				{
					tmpVertex.clear();
					for (auto it = _data.begin(); it != _data.end(); ++it)
					{
						auto tmp = it.value();
						int ID = tmp.lableData.getLableID();
						float lableSize = tmp.lableData.getLableSize();
						int vertexSize = _vertexIndex[ID].second;
						for (int i = 0; i < vertexSize; ++i)
						{
							tmpVertex.push_back(lableSize);
						}					
					}
					initialBuffer_update<float>(1, tmpVertex, 0, updateVertexSize);
				}
			}
        };


        /*
        * ɾ�����еĵĶ�������
        */
        inline void deleteSpecificVertex()
        {
            vector<GLushort>().swap(_vertex_TexCoord);
            vector<float>().swap(_vertex_Pos);
			vector<float>().swap(_vertex_LableSize);
            vector<float>().swap(_vertex_Dir);
            
        };

        /*
        * ��ʼ����������еĶ�������
        */
		//��ͷ����
        void appendFontVertex(const QString &name,const QVector<POS_TXT_DIR>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
            const QVector3D color)
        {
            auto tmp = this->_data[name];
            float arrowSize = tmp.lableData.getLableSize();
            for (auto val : data)
            {
                QVector3D pos = val.first;
                QString txt = val.second.first;
                QVector3D dir = val.second.second;
                beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos, arrowSize, dir);
            }
        };
		//�̶�ͼ������ ���� ͨ������
		void appendFontVertex(const QString &name,const QVector<POS_TXT>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
            const QVector3D color)
        {
			if (_fontType == FontType_FixShape)
			{
				auto tmp = this->_data[name];
				float lableSize = tmp.lableData.getLableSize();
				for (auto val : data)
				{
					QVector3D pos = val.first;
					QString txt = val.second;
					beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos, lableSize);
				}
			}
			else if (_fontType == FontType_Commom)
			{
				for (auto val : data)
				{
					QVector3D pos = val.first;
					QString txt = val.second;
					beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos);
				}
			}
        };
        //��ʼ���
        void beginAppendFontVertex(const QString &txt, const bool highlightState, const bool showState, const bool depthState, const float size,
            const QVector3D color, const QVector3D pos, const float lableSize = 0, const QVector3D dir = QVector3D(0, 0, 0))
        {

            string text = txt.toStdString();
            // �����ı������е��ַ�
            std::string::const_iterator c;
            int posx = 0;
            int posy = 0;
            for (c = text.begin(); c != text.end(); c++)
            {
                Character ch = this->_fontTexture->_characters[*c];
                //��������
                GLfloat xpos = posx + ch.Bearing.x();
                GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
                //���������������ԭ��
                GLfloat xpos_texture = ch.Positon.x();
                GLfloat ypos_texture = ch.Positon.y();
                //�����С
                GLfloat w = ch.Size.x();
                GLfloat h = ch.Size.y();
                //�������ַ��������괫�������ַ�������������
                //��һ�鶥��
                //���ͨ�õ�����
                _vertex_HighlightState.push_back(highlightState);
                _vertex_ShowState.push_back(showState);
                _vertex_DepthState.push_back(depthState);
                _vertex_Size.push_back(size);
                _vertex_Color.push_back(color.x());
                _vertex_Color.push_back(color.y());
                _vertex_Color.push_back(color.z());
                _vertex_Vertex.push_back(xpos);
                _vertex_Vertex.push_back(ypos + h);
                _vertex_Vertex.push_back(0);
                _vertex_TexCoord.push_back(xpos_texture);		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
                _vertex_TexCoord.push_back(ypos_texture);
                _vertex_Pos.push_back(pos.x());
                _vertex_Pos.push_back(pos.y());
                _vertex_Pos.push_back(pos.z());
				if (_fontType == FontType_Arrow)
                {
                    _vertex_Dir.push_back(dir.x());
                    _vertex_Dir.push_back(dir.y());
                    _vertex_Dir.push_back(dir.z());
                    _vertex_LableSize.push_back(lableSize);
                }
				else if (_fontType == FontType_FixShape)
				{
					_vertex_LableSize.push_back(lableSize);
				}

                //�ڶ��鶥��
                _vertex_HighlightState.push_back(highlightState);
                _vertex_ShowState.push_back(showState);
                _vertex_DepthState.push_back(depthState);
                _vertex_Size.push_back(size);
                _vertex_Color.push_back(color.x());
                _vertex_Color.push_back(color.y());
                _vertex_Color.push_back(color.z());
                _vertex_Vertex.push_back(xpos);
                _vertex_Vertex.push_back(ypos);
                _vertex_Vertex.push_back(0);
                _vertex_TexCoord.push_back(xpos_texture);		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
                _vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
                _vertex_Pos.push_back(pos.x());
                _vertex_Pos.push_back(pos.y());
                _vertex_Pos.push_back(pos.z());
				if (_fontType == FontType_Arrow)
				{
					_vertex_Dir.push_back(dir.x());
					_vertex_Dir.push_back(dir.y());
					_vertex_Dir.push_back(dir.z());
					_vertex_LableSize.push_back(lableSize);
				}
				else if (_fontType == FontType_FixShape)
				{
					_vertex_LableSize.push_back(lableSize);
				}
                //�����鶥��
                _vertex_HighlightState.push_back(highlightState);
                _vertex_ShowState.push_back(showState);
                _vertex_DepthState.push_back(depthState);
                _vertex_Size.push_back(size);
                _vertex_Color.push_back(color.x());
                _vertex_Color.push_back(color.y());
                _vertex_Color.push_back(color.z());
                _vertex_Vertex.push_back(xpos + w);
                _vertex_Vertex.push_back(ypos);
                _vertex_Vertex.push_back(0);
                _vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
                _vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
                _vertex_Pos.push_back(pos.x());
                _vertex_Pos.push_back(pos.y());
                _vertex_Pos.push_back(pos.z());
				if (_fontType == FontType_Arrow)
				{
					_vertex_Dir.push_back(dir.x());
					_vertex_Dir.push_back(dir.y());
					_vertex_Dir.push_back(dir.z());
					_vertex_LableSize.push_back(lableSize);
				}
				else if (_fontType == FontType_FixShape)
				{
					_vertex_LableSize.push_back(lableSize);
				}
                //�����鶥��
                _vertex_HighlightState.push_back(highlightState);
                _vertex_ShowState.push_back(showState);
                _vertex_DepthState.push_back(depthState);
                _vertex_Size.push_back(size);
                _vertex_Color.push_back(color.x());
                _vertex_Color.push_back(color.y());
                _vertex_Color.push_back(color.z());
                _vertex_Vertex.push_back(xpos + w);
                _vertex_Vertex.push_back(ypos + h);
                _vertex_Vertex.push_back(0);
                _vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
                _vertex_TexCoord.push_back(ypos_texture);
                _vertex_Pos.push_back(pos.x());
                _vertex_Pos.push_back(pos.y());
                _vertex_Pos.push_back(pos.z());
				if (_fontType == FontType_Arrow)
				{
					_vertex_Dir.push_back(dir.x());
					_vertex_Dir.push_back(dir.y());
					_vertex_Dir.push_back(dir.z());
					_vertex_LableSize.push_back(lableSize);
				}
				else if (_fontType == FontType_FixShape)
				{
					_vertex_LableSize.push_back(lableSize);
				}
                // ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
                posx += (ch.Advance >> 6); // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
            }

        }


    private:
        //��Ⱦ����
        const QHash<QString, T> &_data;
        //����������Ϣ��
        const shared_ptr<mFontTexture> _fontTexture;
        //��������
		FontType _fontType;
        //��������
        vector<GLushort> _vertex_TexCoord;//��������
        vector<float> _vertex_Pos;  //��Ҫ�ƶ�������
		vector<float> _vertex_LableSize;	//ͼ��Ĵ�С������Ҫ��ʾ�ڼ�ͷĩ�� ���� �̶�ͼ�����ϽǴ�)����һ�����ڣ�
        vector<float> _vertex_Dir;	//�����ͷ���򣨲�һ�����ڣ�
       
		template<class T1>
		void appendFontVertex(const QString & name, const QVector<T1>& data, const bool highlightState, const bool showState, const bool depthState, const float size, const QVector3D color);

};
	
	//ֻ�������֣�ͨ�����֡��̶����֣�
	template<class T>
	class mLableRendFont2 :public mLableRendBase<mDrawTexture>
	{
	public:
		//ͨ������/�̶����� VBOnum = 8 
		explicit mLableRendFont2(const QHash<QString, T> &data, const shared_ptr<mFontTexture> fontTextrue):
			mLableRendBase<mDrawTexture>(8, GL_QUADS), _data(data), _fontTexture(fontTextrue)
		{
			//������
			_rendObject->_texture = fontTextrue->_texture;
		};

		virtual ~mLableRendFont2() override {};
		/*
		* ����ͼ�����
		*/
		virtual void updateSingleRend(LableOperateMode mode, const QString &name) override
		{
			auto tmp = this->_data[name];
			long long ID = tmp.fontData.getLableID();
			bool showState = tmp.fontData.getLableShowState();
			bool highlightState = tmp.fontData.getLableHighlightState();
			bool depthState = tmp.fontData.getLableDepthState();
			float size = tmp.fontData.getLableSize();
			QVector3D color = tmp.fontData.getLableColor();
			//�غ���ͼ��
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//������������Ҫ������
				int vertexBegin = _rendObject->_vertexNum;
				//��Ӷ�������
				auto data = tmp.fontData.getParameter();
				if (data.isEmpty()) return;
				appendFontVertex(data, highlightState, showState, depthState, size, color);
				//��������
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				//���¶�������
				_rendObject->_vertexNum += _vertex_HighlightState.size();
				//����buffer
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end())
				{
					return;
				}
				int deleteVertexBegin = _vertexIndex[ID].first;
				int deleteVertexSize = _vertexIndex[ID].second;
				//���¶�������
				_rendObject->_vertexNum -= deleteVertexSize;


				//������Ⱦ			
				initialBuffer_delete<GLushort, float>(2, 3, deleteVertexBegin, deleteVertexSize);
				_vertexIndex.remove(ID);

				//��������
				for (auto it = _vertexIndex.begin(); it != _vertexIndex.end(); ++it)
				{
					if (it.value().first > deleteVertexBegin)
					{
						_vertexIndex[it.key()] = { it.value().first - deleteVertexSize, it.value().second };
					}
				}
			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight || mode == SetColor || mode == SetSize)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end())
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
						tmpVertex.push_back(color.x());
						tmpVertex.push_back(color.y());
						tmpVertex.push_back(color.z());
					}
					initialBuffer_update<GLubyte>(mode, 3, tmpVertex, updateVertexBegin, updateVertexSize);
				}
				else if (mode == SetSize)
				{
					//�������ִ�С
					vector<float> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
					initialBuffer_update<float>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);
				}

			};
		}

		/*
		* ����ͼ�����
		*/
		virtual void updateAllRend(LableOperateMode mode)override
		{
			//�غ���ͼ��
			if (mode == Append)
			{
				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					int vertexTmpBegin = _rendObject->_vertexNum;
					auto tmp = it.value();
					long long ID = tmp.fontData.getLableID();
					bool showState = tmp.fontData.getLableShowState();
					bool highlightState = tmp.fontData.getLableHighlightState();
					bool depthState = tmp.fontData.getLableDepthState();
					float size = tmp.fontData.getLableSize();
					QVector3D color = tmp.fontData.getLableColor();

					//��Ӷ�������
					auto data = tmp.fontData.getParameter();
					if (data.isEmpty()) continue;
					appendFontVertex(data, highlightState, showState, depthState, size, color);
					//��������
					_vertexIndex[ID] = { vertexTmpBegin,(int)_vertex_HighlightState.size()- vertexTmpBegin };
					//���¶�������
					_rendObject->_vertexNum += (int)_vertex_HighlightState.size()- vertexTmpBegin;
				}
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				//���¶�������
				_rendObject->_vertexNum = 0;

				//������Ⱦ				
				initialBuffer_delete<GLushort, float>(2, 3, 0, deleteVertexSize);
				_vertexIndex.clear();
			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight)
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
			}
			else if (mode == SetColor)
			{
				int updateVertexSize = _rendObject->_vertexNum;
				vector<GLubyte> tmpVertex;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					int ID = tmp.fontData.getLableID();
					QVector3D color = tmp.fontData.getLableColor();
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
					int ID = tmp.fontData.getLableID();
					float size = tmp.fontData.getLableSize();
					int vertexSize = _vertexIndex[ID].second;
					for (int i = 0; i < vertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
				}
				initialBuffer_update<float>(mode, 1, tmpVertex, 0, updateVertexSize);
			}
		};

		/*
		* ɾ�����еĵĶ�������
		*/
		inline void deleteSpecificVertex()
		{
			vector<GLushort>().swap(_vertex_TexCoord);
			vector<float>().swap(_vertex_Pos);
		};

		/*
		* ��ʼ����������еĶ�������
		*/

		//�̶�ͼ������ ���� ͨ������
		void appendFontVertex(const QVector<POS_TXT>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
			const QVector3D color)
		{
			for (auto val : data)
			{
				QVector3D pos = val.first;
				QString txt = val.second;
				beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos);
			}

		};
		//��ʼ���
		void beginAppendFontVertex(const QString &txt, const bool highlightState, const bool showState, const bool depthState, const float size,
			const QVector3D color, const QVector3D pos)
		{

			string text = txt.toStdString();
			// �����ı������е��ַ�
			std::string::const_iterator c;
			int posx = 0;
			int posy = 0;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = this->_fontTexture->_characters[*c];
				//��������
				GLfloat xpos = posx + ch.Bearing.x();
				GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
				//���������������ԭ��
				GLfloat xpos_texture = ch.Positon.x();
				GLfloat ypos_texture = ch.Positon.y();
				//�����С
				GLfloat w = ch.Size.x();
				GLfloat h = ch.Size.y();
				//�������ַ��������괫�������ַ�������������
				//��һ�鶥��
				//���ͨ�õ�����
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos);
				_vertex_Vertex.push_back(ypos + h);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture);		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				

				//�ڶ��鶥��
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos);
				_vertex_Vertex.push_back(ypos);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture);		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				
				//�����鶥��
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos + w);
				_vertex_Vertex.push_back(ypos);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				
				//�����鶥��
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos + w);
				_vertex_Vertex.push_back(ypos + h);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				
				// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
				posx += (ch.Advance >> 6); // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
			}

		}


	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
		//����������Ϣ��
		const shared_ptr<mFontTexture> _fontTexture;
		//��������
		vector<GLushort> _vertex_TexCoord;//��������
		vector<float> _vertex_Pos;  //��Ҫ�ƶ�������

	};

	//ֻ�������֣�ͨ�����֡��̶����֣�  ���� ֡��
	template<class T>
	class mLableRendFont3 :public mLableRendBase<mDrawTexture>
	{
	public:
		//ͨ������/�̶����� VBOnum = 8 
		explicit mLableRendFont3(const QHash<int, QHash<QString, T>> &data, const shared_ptr<mFontTexture> fontTextrue) :
			mLableRendBase<mDrawTexture>(8, GL_QUADS), _data(data), _fontTexture(fontTextrue)
		{
			//������
			_rendObject->_texture = fontTextrue->_texture;
		};

		virtual ~mLableRendFont3() override {};
		/*
		* ĳһ֡����ͼ�����
		*/
		virtual void updateSingleRend_OneFrame(const int frameID, LableOperateMode mode, const QString &name) override
		{
			auto tmp = this->_data[frameID].value(name);
			long long ID = tmp.fontData.getLableID();
			bool showState = tmp.fontData.getLableShowState();
			bool highlightState = tmp.fontData.getLableHighlightState();
			bool depthState = tmp.fontData.getLableDepthState();
			float size = tmp.fontData.getLableSize();
			QVector3D color = tmp.fontData.getLableColor();
			//�غ���ͼ��
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend_OneFrame(frameID, Delete, name);
				}
				//������������Ҫ������
				int vertexBegin = _rendObject->_vertexNum;
				//��Ӷ�������
				auto data = tmp.fontData.getParameter();
				if (data.isEmpty()) return;
				appendFontVertex(data, highlightState, showState, depthState, size, color);
				//��������
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				//���¶�������
				_rendObject->_vertexNum += _vertex_HighlightState.size();
				//����buffer
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end())
				{
					return;
				}
				int deleteVertexBegin = _vertexIndex[ID].first;
				int deleteVertexSize = _vertexIndex[ID].second;
				//���¶�������
				_rendObject->_vertexNum -= deleteVertexSize;


				//������Ⱦ			
				initialBuffer_delete<GLushort, float>(2, 3, deleteVertexBegin, deleteVertexSize);
				_vertexIndex.remove(ID);

				//��������
				for (auto it = _vertexIndex.begin(); it != _vertexIndex.end(); ++it)
				{
					if (it.value().first > deleteVertexBegin)
					{
						_vertexIndex[it.key()] = { it.value().first - deleteVertexSize, it.value().second };
					}
				}
			}
		}

		/*
		* ĳһ֡����ͼ�����
		*/
		virtual void updateAllRend_OneFrame(const int frameID, LableOperateMode mode)override
		{
			//�غ���ͼ��
			if (mode == Append)
			{
				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;
				for (auto it = _data[frameID].begin(); it != _data[frameID].end(); ++it)
				{
					int vertexTmpBegin = _rendObject->_vertexNum;
					auto tmp = it.value();
					long long ID = tmp.fontData.getLableID();
					bool showState = tmp.fontData.getLableShowState();
					bool highlightState = tmp.fontData.getLableHighlightState();
					bool depthState = tmp.fontData.getLableDepthState();
					float size = tmp.fontData.getLableSize();
					QVector3D color = tmp.fontData.getLableColor();

					//��Ӷ�������
					auto data = tmp.fontData.getParameter();
					if (data.isEmpty()) continue;
					appendFontVertex(data, highlightState, showState, depthState, size, color);
					//��������
					_vertexIndex[ID] = { vertexTmpBegin,(int)_vertex_HighlightState.size() - vertexTmpBegin };
					//���¶�������
					_rendObject->_vertexNum += (int)_vertex_HighlightState.size() - vertexTmpBegin;
				}
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				for (auto it = _data[frameID].begin(); it != _data[frameID].end(); ++it)
				{
					updateSingleRend_OneFrame(frameID, Delete, it.key());
				}
			}
			
		};
		/*
		* ɾ������֡ͼ��
		*/
		virtual void updateAllRend(LableOperateMode mode)override
		{			
			if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				//���¶�������
				_rendObject->_vertexNum = 0;

				//������Ⱦ				
				initialBuffer_delete<GLushort, float>(2, 3, 0, deleteVertexSize);
				_vertexIndex.clear();
			}

		};
		/*
		* ɾ�����еĵĶ�������
		*/
		inline void deleteSpecificVertex()
		{
			vector<GLushort>().swap(_vertex_TexCoord);
			vector<float>().swap(_vertex_Pos);
		};

		/*
		* ��ʼ����������еĶ�������
		*/

		//�̶�ͼ������ ���� ͨ������
		void appendFontVertex(const QVector<POS_TXT>& data, const bool highlightState, const bool showState, const bool depthState, const float size,
			const QVector3D color)
		{
			for (auto val : data)
			{
				QVector3D pos = val.first;
				QString txt = val.second;
				beginAppendFontVertex(txt, highlightState, showState, depthState, size, color, pos);
			}

		};
		//��ʼ���
		void beginAppendFontVertex(const QString &txt, const bool highlightState, const bool showState, const bool depthState, const float size,
			const QVector3D color, const QVector3D pos)
		{

			string text = txt.toStdString();
			// �����ı������е��ַ�
			std::string::const_iterator c;
			int posx = 0;
			int posy = 0;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = this->_fontTexture->_characters[*c];
				//��������
				GLfloat xpos = posx + ch.Bearing.x();
				GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
				//���������������ԭ��
				GLfloat xpos_texture = ch.Positon.x();
				GLfloat ypos_texture = ch.Positon.y();
				//�����С
				GLfloat w = ch.Size.x();
				GLfloat h = ch.Size.y();
				//�������ַ��������괫�������ַ�������������
				//��һ�鶥��
				//���ͨ�õ�����
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos);
				_vertex_Vertex.push_back(ypos + h);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture);		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());


				//�ڶ��鶥��
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos);
				_vertex_Vertex.push_back(ypos);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture);		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());

				//�����鶥��
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos + w);
				_vertex_Vertex.push_back(ypos);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());

				//�����鶥��
				_vertex_HighlightState.push_back(highlightState);
				_vertex_ShowState.push_back(showState);
				_vertex_DepthState.push_back(depthState);
				_vertex_Size.push_back(size);
				_vertex_Color.push_back(color.x());
				_vertex_Color.push_back(color.y());
				_vertex_Color.push_back(color.z());
				_vertex_Vertex.push_back(xpos + w);
				_vertex_Vertex.push_back(ypos + h);
				_vertex_Vertex.push_back(0);
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//��Ӧ�ó���1024 Ϊ�˽�ʡ�ռ�����ɫ���г���1024 �����Ϊglshort����
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());

				// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
				posx += (ch.Advance >> 6); // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
			}

		}


	private:
		//��Ⱦ����
		const const QHash<int, QHash<QString, T>> &_data;
		//����������Ϣ��
		const shared_ptr<mFontTexture> _fontTexture;
		//��������
		vector<GLushort> _vertex_TexCoord;//��������
		vector<float> _vertex_Pos;  //��Ҫ�ƶ�������

	};

}
