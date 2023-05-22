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
    //字形数据存储结构体
    struct Character
    {
        //大纹理上字形的像素原点和大小
        QVector2D  Positon;		//该字体原点（左下角）在文字大纹理上的像素坐标
        QVector2D  Size;       // 字形大小(像素大小 Width Height)

        //用来设置绘制字形的顶点数据（保持间隔）
        QVector2D  Bearing;    // 从基准线到字形左部/顶部的偏移值
        GLuint     Advance;    // 原点距下一个字形原点的距离

    };
    //文字纹理生成类
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
        * 友元模板
        */
        template<class T>
        friend class mLableRendFont;
		template<class T>
		friend class mLableRendFont2;
		template<class T>
		friend class mLableRendFont3;
        /*
        * 加载文字纹理（把所有文字加载到一张纹理上）
        */
		void LoadFontTexture();
    
    private:
        //纹理
        GLuint _texture;
        //字形数据库
        QMap<GLchar, Character> _characters;
    };

    //文字类型图标绘制(图标+文字同时存在) 跟图标绑定在一起
    template<class T>
    class mLableRendFont :public mLableRendBase<mDrawTexture>
    {
    public:
        //固定图标文字VBOnum = 9  箭头末端的文字 VBOnum = 10 mFontTexture 文字大纹理信息类
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
            //绑定纹理
            _rendObject->_texture = fontTextrue->_texture;
        };

        virtual ~mLableRendFont() override {};
        /*
        * 单个图标更新
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
            //载荷类图标
            if (mode == Append)
            {
                if (_vertexIndex.contains(ID))
                {
                    updateSingleRend(Delete, name);
                }
                //保存索引所需要的数据
                int vertexBegin = _rendObject->_vertexNum;
                //添加顶点数据
                auto data = tmp.fontData.getParameter();
				if (data.isEmpty()) return;
                appendFontVertex(name, data, highlightState, showState, depthState, size, color);
                //更新索引
                _vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
                //更新顶点数量
                _rendObject->_vertexNum += _vertex_HighlightState.size();
                //更新buffer
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
                //清空数据
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
                //更新顶点数量
                _rendObject->_vertexNum -= deleteVertexSize;


                //更新渲染
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

                //更新索引
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
					//更新文字大小
					vector<float> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
					initialBuffer_update<float>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);

					//更新文字位置 箭头末端文字 和 固定类型图标文字才有
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
        * 所有图标更新
        */
        virtual void updateAllRend(LableOperateMode mode) override
        {
            //载荷类图标
            if (mode == Append)
            {
                //顶点更新起点位置
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

                    //添加顶点数据
                    auto data = tmp.fontData.getParameter();
					if (data.isEmpty()) continue;
                    appendFontVertex(it.key(), data, highlightState, showState, depthState, size, color);
                    //更新索引
                    _vertexIndex[ID] = { vertexTmpBegin,(int)_vertex_HighlightState.size()- vertexTmpBegin };
                    //更新顶点数量
                    _rendObject->_vertexNum += (int)_vertex_HighlightState.size()- vertexTmpBegin;
                }
                //更新buffer
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
                //清空数据
                deleteSpecificVertex();
                deleteCommonVerrtex();
            }
            else if (mode == Delete)
            {
                int deleteVertexSize = _rendObject->_vertexNum;
                //更新顶点数量
                _rendObject->_vertexNum = 0;

                //更新渲染
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

				//更新文字位置 箭头末端文字 和 固定类型图标文字才有
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
        * 删除特有的的顶点数据
        */
        inline void deleteSpecificVertex()
        {
            vector<GLushort>().swap(_vertex_TexCoord);
            vector<float>().swap(_vertex_Pos);
			vector<float>().swap(_vertex_LableSize);
            vector<float>().swap(_vertex_Dir);
            
        };

        /*
        * 开始添加文字所有的顶点数据
        */
		//箭头文字
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
		//固定图标文字 或者 通用文字
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
        //开始添加
        void beginAppendFontVertex(const QString &txt, const bool highlightState, const bool showState, const bool depthState, const float size,
            const QVector3D color, const QVector3D pos, const float lableSize = 0, const QVector3D dir = QVector3D(0, 0, 0))
        {

            string text = txt.toStdString();
            // 遍历文本中所有的字符
            std::string::const_iterator c;
            int posx = 0;
            int posy = 0;
            for (c = text.begin(); c != text.end(); c++)
            {
                Character ch = this->_fontTexture->_characters[*c];
                //顶点坐标
                GLfloat xpos = posx + ch.Bearing.x();
                GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
                //该字体的纹理坐标原点
                GLfloat xpos_texture = ch.Positon.x();
                GLfloat ypos_texture = ch.Positon.y();
                //纹理大小
                GLfloat w = ch.Size.x();
                GLfloat h = ch.Size.y();
                //将文字字符顶点坐标传入所有字符顶点坐标容器
                //第一组顶点
                //添加通用的数据
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
                _vertex_TexCoord.push_back(xpos_texture);		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
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

                //第二组顶点
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
                _vertex_TexCoord.push_back(xpos_texture);		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
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
                //第三组顶点
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
                _vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
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
                //第四组顶点
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
                _vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
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
                // 更新位置到下一个字形的原点，注意单位是1/64像素
                posx += (ch.Advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
            }

        }


    private:
        //渲染数据
        const QHash<QString, T> &_data;
        //文字纹理信息类
        const shared_ptr<mFontTexture> _fontTexture;
        //文字类型
		FontType _fontType;
        //顶点数据
        vector<GLushort> _vertex_TexCoord;//纹理坐标
        vector<float> _vertex_Pos;  //需要移动的坐标
		vector<float> _vertex_LableSize;	//图标的大小（文字要显示在箭头末端 或者 固定图标右上角处)（不一定存在）
        vector<float> _vertex_Dir;	//跟随箭头朝向（不一定存在）
       
		template<class T1>
		void appendFontVertex(const QString & name, const QVector<T1>& data, const bool highlightState, const bool showState, const bool depthState, const float size, const QVector3D color);

};
	
	//只存在文字（通用文字、固定文字）
	template<class T>
	class mLableRendFont2 :public mLableRendBase<mDrawTexture>
	{
	public:
		//通用文字/固定文字 VBOnum = 8 
		explicit mLableRendFont2(const QHash<QString, T> &data, const shared_ptr<mFontTexture> fontTextrue):
			mLableRendBase<mDrawTexture>(8, GL_QUADS), _data(data), _fontTexture(fontTextrue)
		{
			//绑定纹理
			_rendObject->_texture = fontTextrue->_texture;
		};

		virtual ~mLableRendFont2() override {};
		/*
		* 单个图标更新
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
			//载荷类图标
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//保存索引所需要的数据
				int vertexBegin = _rendObject->_vertexNum;
				//添加顶点数据
				auto data = tmp.fontData.getParameter();
				if (data.isEmpty()) return;
				appendFontVertex(data, highlightState, showState, depthState, size, color);
				//更新索引
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				//更新顶点数量
				_rendObject->_vertexNum += _vertex_HighlightState.size();
				//更新buffer
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//清空数据
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
				//更新顶点数量
				_rendObject->_vertexNum -= deleteVertexSize;


				//更新渲染			
				initialBuffer_delete<GLushort, float>(2, 3, deleteVertexBegin, deleteVertexSize);
				_vertexIndex.remove(ID);

				//更新索引
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
					//更新文字大小
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
		* 所有图标更新
		*/
		virtual void updateAllRend(LableOperateMode mode)override
		{
			//载荷类图标
			if (mode == Append)
			{
				//顶点更新起点位置
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

					//添加顶点数据
					auto data = tmp.fontData.getParameter();
					if (data.isEmpty()) continue;
					appendFontVertex(data, highlightState, showState, depthState, size, color);
					//更新索引
					_vertexIndex[ID] = { vertexTmpBegin,(int)_vertex_HighlightState.size()- vertexTmpBegin };
					//更新顶点数量
					_rendObject->_vertexNum += (int)_vertex_HighlightState.size()- vertexTmpBegin;
				}
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//清空数据
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				//更新顶点数量
				_rendObject->_vertexNum = 0;

				//更新渲染				
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
		* 删除特有的的顶点数据
		*/
		inline void deleteSpecificVertex()
		{
			vector<GLushort>().swap(_vertex_TexCoord);
			vector<float>().swap(_vertex_Pos);
		};

		/*
		* 开始添加文字所有的顶点数据
		*/

		//固定图标文字 或者 通用文字
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
		//开始添加
		void beginAppendFontVertex(const QString &txt, const bool highlightState, const bool showState, const bool depthState, const float size,
			const QVector3D color, const QVector3D pos)
		{

			string text = txt.toStdString();
			// 遍历文本中所有的字符
			std::string::const_iterator c;
			int posx = 0;
			int posy = 0;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = this->_fontTexture->_characters[*c];
				//顶点坐标
				GLfloat xpos = posx + ch.Bearing.x();
				GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
				//该字体的纹理坐标原点
				GLfloat xpos_texture = ch.Positon.x();
				GLfloat ypos_texture = ch.Positon.y();
				//纹理大小
				GLfloat w = ch.Size.x();
				GLfloat h = ch.Size.y();
				//将文字字符顶点坐标传入所有字符顶点坐标容器
				//第一组顶点
				//添加通用的数据
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
				_vertex_TexCoord.push_back(xpos_texture);		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				

				//第二组顶点
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
				_vertex_TexCoord.push_back(xpos_texture);		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				
				//第三组顶点
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
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				
				//第四组顶点
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
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				
				// 更新位置到下一个字形的原点，注意单位是1/64像素
				posx += (ch.Advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
			}

		}


	private:
		//渲染数据
		const QHash<QString, T> &_data;
		//文字纹理信息类
		const shared_ptr<mFontTexture> _fontTexture;
		//顶点数据
		vector<GLushort> _vertex_TexCoord;//纹理坐标
		vector<float> _vertex_Pos;  //需要移动的坐标

	};

	//只存在文字（通用文字、固定文字）  后处理 帧绑定
	template<class T>
	class mLableRendFont3 :public mLableRendBase<mDrawTexture>
	{
	public:
		//通用文字/固定文字 VBOnum = 8 
		explicit mLableRendFont3(const QHash<int, QHash<QString, T>> &data, const shared_ptr<mFontTexture> fontTextrue) :
			mLableRendBase<mDrawTexture>(8, GL_QUADS), _data(data), _fontTexture(fontTextrue)
		{
			//绑定纹理
			_rendObject->_texture = fontTextrue->_texture;
		};

		virtual ~mLableRendFont3() override {};
		/*
		* 某一帧单个图标更新
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
			//载荷类图标
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend_OneFrame(frameID, Delete, name);
				}
				//保存索引所需要的数据
				int vertexBegin = _rendObject->_vertexNum;
				//添加顶点数据
				auto data = tmp.fontData.getParameter();
				if (data.isEmpty()) return;
				appendFontVertex(data, highlightState, showState, depthState, size, color);
				//更新索引
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				//更新顶点数量
				_rendObject->_vertexNum += _vertex_HighlightState.size();
				//更新buffer
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//清空数据
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
				//更新顶点数量
				_rendObject->_vertexNum -= deleteVertexSize;


				//更新渲染			
				initialBuffer_delete<GLushort, float>(2, 3, deleteVertexBegin, deleteVertexSize);
				_vertexIndex.remove(ID);

				//更新索引
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
		* 某一帧所有图标更新
		*/
		virtual void updateAllRend_OneFrame(const int frameID, LableOperateMode mode)override
		{
			//载荷类图标
			if (mode == Append)
			{
				//顶点更新起点位置
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

					//添加顶点数据
					auto data = tmp.fontData.getParameter();
					if (data.isEmpty()) continue;
					appendFontVertex(data, highlightState, showState, depthState, size, color);
					//更新索引
					_vertexIndex[ID] = { vertexTmpBegin,(int)_vertex_HighlightState.size() - vertexTmpBegin };
					//更新顶点数量
					_rendObject->_vertexNum += (int)_vertex_HighlightState.size() - vertexTmpBegin;
				}
				initialBuffer_append <GLushort, float>(2, 3, _vertex_TexCoord, _vertex_Pos, vertexBegin);
				//清空数据
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
		* 删除所有帧图标
		*/
		virtual void updateAllRend(LableOperateMode mode)override
		{			
			if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				//更新顶点数量
				_rendObject->_vertexNum = 0;

				//更新渲染				
				initialBuffer_delete<GLushort, float>(2, 3, 0, deleteVertexSize);
				_vertexIndex.clear();
			}

		};
		/*
		* 删除特有的的顶点数据
		*/
		inline void deleteSpecificVertex()
		{
			vector<GLushort>().swap(_vertex_TexCoord);
			vector<float>().swap(_vertex_Pos);
		};

		/*
		* 开始添加文字所有的顶点数据
		*/

		//固定图标文字 或者 通用文字
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
		//开始添加
		void beginAppendFontVertex(const QString &txt, const bool highlightState, const bool showState, const bool depthState, const float size,
			const QVector3D color, const QVector3D pos)
		{

			string text = txt.toStdString();
			// 遍历文本中所有的字符
			std::string::const_iterator c;
			int posx = 0;
			int posy = 0;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = this->_fontTexture->_characters[*c];
				//顶点坐标
				GLfloat xpos = posx + ch.Bearing.x();
				GLfloat ypos = posy - (ch.Size.y() - ch.Bearing.y());
				//该字体的纹理坐标原点
				GLfloat xpos_texture = ch.Positon.x();
				GLfloat ypos_texture = ch.Positon.y();
				//纹理大小
				GLfloat w = ch.Size.x();
				GLfloat h = ch.Size.y();
				//将文字字符顶点坐标传入所有字符顶点坐标容器
				//第一组顶点
				//添加通用的数据
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
				_vertex_TexCoord.push_back(xpos_texture);		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());


				//第二组顶点
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
				_vertex_TexCoord.push_back(xpos_texture);		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());

				//第三组顶点
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
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture + ch.Size.y());
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());

				//第四组顶点
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
				_vertex_TexCoord.push_back(xpos_texture + ch.Size.x());		//本应该除以1024 为了节省空间在着色器中除以1024 这里变为glshort类型
				_vertex_TexCoord.push_back(ypos_texture);
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());

				// 更新位置到下一个字形的原点，注意单位是1/64像素
				posx += (ch.Advance >> 6); // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
			}

		}


	private:
		//渲染数据
		const const QHash<int, QHash<QString, T>> &_data;
		//文字纹理信息类
		const shared_ptr<mFontTexture> _fontTexture;
		//顶点数据
		vector<GLushort> _vertex_TexCoord;//纹理坐标
		vector<float> _vertex_Pos;  //需要移动的坐标

	};

}
