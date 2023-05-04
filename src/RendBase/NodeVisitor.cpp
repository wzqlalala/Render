#include "NodeVisitor.h"
#include "StateSet.h"

namespace mxr
{
	GLuint glgetTypeSize(GLenum type)
	{
		switch (type)
		{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		case GL_INT:
		case GL_UNSIGNED_INT:
		case GL_FLOAT:
			return 4;
		default:
			break;
		}
		return 0;
	}



	//ͳ��ÿ��drawable�Ĺ���ÿ������Ĵ�С������������
	std::map<int, DrawableData> DrawableAttribute::census()
	{
		std::map<int, DrawableData> _mapdatas;
		Drawable::ArrayList _lists = drawable->getVertexAttribArrayList();
		UIntArray* index = drawable->getIndexAttribute();
		if (index)
		{
			DrawElementsIndirectCommand &command = drawable->getDrawElemnetIndirect();
			command.indexCount = index->size();
			command.baseVertex = 0;
			command.firstIndex = 0;
		}
		else
		{
			DrawArraysIndirectCommand &command = drawable->getDrawArraysIndirect();
			command.firstVertex = 0;
			command.vertexCount = drawable->getVertexCount();
			
		}
	
		for (int k = 0; k < _lists.size(); k++)
		{
			if (_lists[k])
			{
				DrawableData type
				{
					0,
					_lists[k]->size()*glgetTypeSize(_lists[k]->getDataType()),
					_lists[k]->getDataSize(),
					_lists[k]->getDataType(),
					_lists[k]->data()

				};
				_mapdatas[k] = type;			
			}
		}
		return _mapdatas;
	}

	//������ͬ��ʽ��һ��
	void VaoDrawArrayAttribute::compile(std::vector<DrawableAttribute> &_drawableattribute)
	{
		std::vector<DrawArraysIndirectCommand> arraycommands;
		std::vector<DrawElementsIndirectCommand> elementcommands;

		int firstVertex = 0;
		int firstIndex = 0;
		int baseVertex = 0;
		/*for (int i = 0; i < _drawableattribute.size(); i++)
		{
			std::map<int, DrawableData> _drawabledata = _drawableattribute[i].census();
			if (!_drawableattribute[i].isCompile)
			{
				isSetVAOFormat = false;
				if (HaveIBO)
				{
					asset_ref<IBO> ibo;
					BufferAttribute att;
					if (_ibo)
					{
						int isize = _drawableattribute[i].drawable->getIndexAttribute()->size() * sizeof(GLuint);
						const void*idata = _drawableattribute[i].drawable->getIndexAttribute()->data();
						int _size = _ibo->Size() + isize;
						ibo = MakeAsset<IBO>(_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
						ibo->Copy(_ibo->ID(), ibo->ID(), 0, 0, _ibo->Size());
						ibo->SetData(_ibo->Size(), isize, idata);
						att = { (int)_ibo->Size() , _size };
					}
					else
					{
						int isize = _drawableattribute[i].drawable->getIndexAttribute()->size() * sizeof(GLuint);
						const void*idata = _drawableattribute[i].drawable->getIndexAttribute()->data();
						ibo = MakeAsset<IBO>(isize, idata, GL_DYNAMIC_STORAGE_BIT);
						att = { 0 , isize };
					}
					_ibo = ibo;
					_drawableattribute[i].drawable->getIndexAttribute()->setBufferAttribute(att);
				}

				for (auto &item : _drawabledata)
				{
					asset_ref<VBO> vbo;
					BufferAttribute att;
					if (vbos[item.first])
					{
						int _size = vbos[item.first]->Size() + item.second.size;
						vbo = MakeAsset<VBO>(_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
						vbo->Copy(vbos[item.first]->ID(), vbo->ID(), 0, 0, vbos[item.first]->Size());
						vbo->SetData(vbos[item.first]->Size(), item.second.size, item.second.data);
						att = { (int)vbos[item.first]->Size() , _size };
					}
					else
					{
						int _size = item.second.size;
						vbo = MakeAsset<VBO>(_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
						vbo->SetData(0, item.second.size, item.second.data);
						att = { 0 , item.second.size };
					}
					vbos[item.first] = vbo;
					_drawableattribute[i].drawable->getVertexAttribArray(item.first)->SetBuffer(vbo);
					_drawableattribute[i].drawable->getVertexAttribArray(item.first)->setBufferAttribute(att);
					vbos_datasize[item.first] = item.second.data_size;
					vbos_datatype[item.first] = item.second.type;
				}
				_drawableattribute[i].isCompile = true;
			}

			if (HaveIBO)
			{
				DrawElementsIndirectCommand &command = _drawableattribute[i].drawable->getDrawElemnetIndirect();
				command.firstIndex = firstIndex;
				command.baseVertex = baseVertex;
				_drawableattribute[i].drawable->SetVAO(vao);
				firstIndex += command.indexCount;
				baseVertex += _drawableattribute[i].drawable->getVertexCount();
				elementcommands.push_back(command);
				if (_drawableattribute[i].drawable->getNodeMask())
				{
					elementcommands[elementcommands.size() - 1].instanceCount = 0;
				}
			}
			else
			{
				DrawArraysIndirectCommand &command = _drawableattribute[i].drawable->getDrawArraysIndirect();
				command.firstVertex = firstVertex;
				_drawableattribute[i].drawable->SetVAO(vao);

				firstVertex += command.vertexCount;
				arraycommands.push_back(command);
				if (_drawableattribute[i].drawable->getNodeMask())
				{
					arraycommands[arraycommands.size() - 1].instanceCount = 0;
				}

			}


		}*/

		std::map<int, int> vbosize;//vbo�Ĵ�С
		for (auto vbo : vbos)
		{
			vbosize[vbo.first] = vbo.second->Size();
		}
		int ibosize = 0;//ibo�Ĵ�С
		if (HaveIBO && _ibo)
		{
			ibosize = _ibo->Size();
		}
		std::map<int, std::map<int, DrawableData>> _drawabledatas;
		for (int i = 0; i < _drawableattribute.size(); i++)
		{
			if (!_drawableattribute[i].isCompile)
			{
				std::map<int, DrawableData> _drawabledata = _drawableattribute[i].census(); _drawabledatas[i] = _drawabledata;
				isSetVAOFormat = false;
				if (HaveIBO)
				{
					asset_ref<IBO> ibo;
					BufferAttribute att;
					//if (_ibo)
					//{
					//	int isize = _drawableattribute[i].drawable->getIndexAttribute()->size() * sizeof(GLuint);
					//	int _size = ibosize + isize;
					//	att = { ibosize, _size };
					//	ibosize = _size;
					//}
					//else
					{
						int isize = _drawableattribute[i].drawable->getIndexAttribute()->size() * sizeof(GLuint) + ibosize;
						//const void*idata = _drawableattribute[i].drawable->getIndexAttribute()->data();
						//ibo = MakeAsset<IBO>(isize, idata, GL_DYNAMIC_STORAGE_BIT);
						att = { ibosize , isize };
						ibosize = isize;
					}
					_drawableattribute[i].drawable->getIndexAttribute()->setBufferAttribute(att);
				}

				for (auto &item : _drawabledata)
				{
					//asset_ref<VBO> vbo;
					BufferAttribute att;
					if (vbosize.find(item.first) != vbosize.end())
					{
						int _size = vbosize[item.first] + item.second.size;
						att = { (int)vbosize[item.first] , _size };
						vbosize[item.first] = _size;
					}
					else
					{
						int _size = item.second.size;
						att = { 0 , item.second.size };
						vbosize[item.first] = _size;
					}
					_drawableattribute[i].drawable->getVertexAttribArray(item.first)->setBufferAttribute(att);
					vbos_datasize[item.first] = item.second.data_size;
					vbos_datatype[item.first] = item.second.type;
				}
			}

			if (HaveIBO)
			{
				DrawElementsIndirectCommand &command = _drawableattribute[i].drawable->getDrawElemnetIndirect();
				command.firstIndex = firstIndex;
				command.baseVertex = baseVertex;
				_drawableattribute[i].drawable->SetVAO(vao);
				firstIndex += command.indexCount;
				baseVertex += _drawableattribute[i].drawable->getVertexCount();
				elementcommands.push_back(command);
				if (_drawableattribute[i].drawable->getNodeMask())
				{
					elementcommands[elementcommands.size() - 1].instanceCount = 0;
				}
			}
			else
			{
				DrawArraysIndirectCommand &command = _drawableattribute[i].drawable->getDrawArraysIndirect();
				command.firstVertex = firstVertex;
				_drawableattribute[i].drawable->SetVAO(vao);

				firstVertex += command.vertexCount;
				arraycommands.push_back(command);
				if (_drawableattribute[i].drawable->getNodeMask())
				{
					arraycommands[arraycommands.size() - 1].instanceCount = 0;
				}

			}


		}

		bool vbobool{ false };//�Ƿ���Ҫ��������VBO
		for (auto vsize : vbosize)
		{
			if (vbos[vsize.first])//����ԭʼvbo
			{
				if (vbos[vsize.first]->Size() != vsize.second)
				{
					asset_ref<VBO> newvbo = MakeAsset<VBO>(vsize.second, nullptr, GL_DYNAMIC_STORAGE_BIT);
					newvbo->Copy(vbos[vsize.first]->ID(), newvbo->ID(), 0, 0, vbos[vsize.first]->Size());
					vbos[vsize.first] = newvbo;
					vbobool = true;
				}
			}
			else
			{
				vbobool = true;
				vbos[vsize.first] = MakeAsset<VBO>(vsize.second, nullptr, GL_DYNAMIC_STORAGE_BIT);
			}
		}
		bool ibobool = false;//�Ƿ���Ҫ��������IBO
		if (HaveIBO)
		{
			if (_ibo)
			{
				if (_ibo->Size() != ibosize)
				{
					asset_ref<IBO> newvbo = MakeAsset<IBO>(ibosize, nullptr, GL_DYNAMIC_STORAGE_BIT);
					newvbo->Copy(_ibo->ID(), newvbo->ID(), 0, 0, _ibo->Size());
					_ibo = newvbo;
					ibobool = true;
				}
			}
			else
			{
				ibobool = true;
				_ibo = MakeAsset<IBO>(ibosize, nullptr, GL_DYNAMIC_STORAGE_BIT);
			}
		}

		if (vbobool || ibobool)
		{
			for (int i = 0; i < _drawableattribute.size(); i++)
			{
				//std::map<int, DrawableData> _drawabledata = _drawableattribute[i].census();
				std::map<int, DrawableData> _drawabledata = _drawableattribute[i].census();
				if (!_drawableattribute[i].isCompile)//û�б������
				{
					isSetVAOFormat = false;
					if (HaveIBO)
					{
						auto index = _drawableattribute[i].drawable->getIndexAttribute();
						BufferAttribute att = index->getBufferAttribute();
						_ibo->SetData(att._start, index->size() * sizeof(GLuint), index->data());
						_drawableattribute[i].drawable->getIndexAttribute()->SetBuffer(_ibo);					
					}

					for (auto &item : _drawabledata)
					{
						BufferAttribute att = _drawableattribute[i].drawable->getVertexAttribArray(item.first)->getBufferAttribute();
						auto vbo = vbos[item.first];
						vbo->SetData(att._start, item.second.size, item.second.data);
						_drawableattribute[i].drawable->getVertexAttribArray(item.first)->SetBuffer(vbos[item.first]);
						//_drawableattribute[i].drawable->getVertexAttribArray(item.first)->clear();
					}
					_drawableattribute[i].isCompile = true;
				}
				else//������ģ���Ҫ����֮ǰ��vbo����
				{
					isSetVAOFormat = false;
					if (HaveIBO)
					{
						_drawableattribute[i].drawable->getIndexAttribute()->SetBuffer(_ibo);
					}
					for (auto &item : _drawabledata)
					{
						_drawableattribute[i].drawable->getVertexAttribArray(item.first)->SetBuffer(vbos[item.first]);
					}
					_drawableattribute[i].isCompile = true;
				}
			}
		}

		if (!isSetVAOFormat)
		{
			int bind_id = 0;
			for (auto &item : vbos)
			{

				vao->SetFormat(bind_id, item.first, 0, vbos_datasize[item.first], vbos_datatype[item.first]);
				vao->SetVBO(vbos[item.first]->ID(), bind_id, 0, glgetTypeSize(vbos_datatype[item.first])*vbos_datasize[item.first]);
				bind_id++;
			}
			if (HaveIBO)
			{
				vao->SetIBO(_ibo->ID());
			}
			isSetVAOFormat = true;
		}
		_drawbuffers.clear();
		_arraystates.clear();
		_elementstates.clear();
		//�ȸ��ݲ�ͬ��stateSet����,���Ҽ�¼λ��
		std::map<asset_ref<StateSet>, std::vector<int> > locations;
		for (int i = 0; i < _drawableattribute.size(); i++)
		{
			Drawable *temp = _drawableattribute[i].drawable;
			locations[temp->getStateSet()].push_back(i);
		}

		//����drawbuffer	
		if (HaveIBO)
		{
			for (auto &item : locations)
			{
				for (int i = 0; i < item.second.size(); i++)
				{
					_elementstates[item.first].push_back(elementcommands[item.second[i]]);
				}
				int _size = _elementstates[item.first].size() * sizeof(DrawElementsIndirectCommand);
				asset_ref<DrawBuffer> _buffer = MakeAsset<DrawBuffer>(_size, _elementstates[item.first].data(), GL_DYNAMIC_STORAGE_BIT);
				_drawbuffers[item.first] = _buffer;
			}

		}
		else
		{

			for (auto &item : locations)
			{
				for (int i = 0; i < item.second.size(); i++)
				{
					_arraystates[item.first].push_back(arraycommands[item.second[i]]);
				}
				int _size = _arraystates[item.first].size() * sizeof(DrawArraysIndirectCommand);
				asset_ref<DrawBuffer> _buffer = MakeAsset<DrawBuffer>(_size, _arraystates[item.first].data(), GL_DYNAMIC_STORAGE_BIT);
				_drawbuffers[item.first] = _buffer;
			}
		}

	}



	void NodeVisitor::accept(Drawable * node)
	{
		for (auto item = drawableattributes.begin(); item != drawableattributes.end(); )
		{
			if (item->size() == 0)
			{
				item = drawableattributes.erase(item);
			}
			else
			{
				item++;
			}
		}

		if (!node)
		{
			return;
		}

		if (node->empty())
		{
			return;
		}

		bool addList = false;
		for (int i = 0; i < drawableattributes.size(); i++)
		{
			if (drawableattributes[i][0].drawable->CompareForm(node))
			{
				//������ڸ�drawable,�򷵻�
				for (int j = 0; j < drawableattributes[i].size(); j++)
				{
					if (drawableattributes[i][j].drawable == node)
					{
						return;
					}
				}
				DrawableAttribute node_temp{ node, false };
				drawableattributes[i].push_back(node_temp);
				addList = true;
			}
			
		}

		if (!addList)
		{
			std::vector<DrawableAttribute > ables;
			DrawableAttribute node_temp{ node, false };
			ables.push_back(node_temp);
			drawableattributes.push_back(ables);

			VaoDrawArrayAttribute _attribute;
			if (node->getIndexAttribute())
			{
				_attribute.HaveIBO = true;
			}
			asset_ref<VAO> vao = MakeAsset<VAO>();
			_attribute.vao = vao;
			_vaoattributes.push_back(_attribute);

		}

	}

	void NodeVisitor::compile()
	{
		if (_vaoattributes.size() == 0)
		{
			return;
		}
		if (!_changeDrawBuffer)
		{
			return;
		}
		for (int i = 0; i < drawableattributes.size(); i++)
		{
			_vaoattributes[i].compile(drawableattributes[i]);			
		}
		_changeDrawBuffer = false;
	}

	void NodeVisitor::clear()
	{
		for (int i = 0; i < drawableattributes.size(); i++)
		{
			for (auto &item : drawableattributes.at(i))
			{
				if (!item.isClear)
				{
					item.isClear = true;
					auto arraylist = item.drawable->getVertexAttribArrayList();
					for (auto array : arraylist)
					{
						if (array)
						{
							array->clear();
						}
					}
				}
			}
		}
	}

	void NodeVisitor::run()
	{
		//���ݲ�ͬ��RenderPass���ٷ�һ����.
		std::map<RenderPass*, std::vector<OnceDrawAttribute>> drawAttributes;
		std::vector<OnceDrawAttribute> nodrawAttributes;
		for (int i = 0; i < _vaoattributes.size(); i++)
		{
			for (auto &item : _vaoattributes[i]._drawbuffers)
			{
				int count;
				if (_vaoattributes[i].HaveIBO)
				{
					count = _vaoattributes[i]._elementstates[item.first].size();
				}
				else
				{
					count = _vaoattributes[i]._arraystates[item.first].size();
				}

				OnceDrawAttribute oncedraw = { _vaoattributes[i].vao, item.first, item.second, count,_vaoattributes[i].HaveIBO };
				if (item.first->getRenderPass())
				{
					drawAttributes[item.first->getRenderPass()].push_back(oncedraw);
				}
				else
				{
					nodrawAttributes.push_back(oncedraw);
				}
			}
		}

		
		//draw
		for (auto &item : drawAttributes)
		{

			int passCount = item.first->getpassSize();
			item.first->Bind(0);
			//������һ��pass,�õ���һ��pass�Ľ��
			for (int i = 0; i < item.second.size(); i++)
			{
				item.second[i]._state->Bind();
				item.second[i]._vao->SetDrawBuffer(item.second[i]._drawbuffer->ID());
				int _Count = item.second[i].drawCount;
				GLenum drawMode = item.second[i]._state->getDrawMode();
				if (item.second[i].HaveIBO)
				{
					item.second[i]._vao->DrawIndirectElement(drawMode, 0, _Count);
				}
				else
				{
					item.second[i]._vao->DrawIndirectArray(drawMode, 0, _Count);
				}
				item.second[i]._state->UnBind();
			}
			item.first->UnBind(0);

			//��������pass
			item.first->DrawQuad();

		}

		//����
		std::sort(nodrawAttributes.begin(), nodrawAttributes.end(), [](const OnceDrawAttribute &d1, const OnceDrawAttribute &d2)//offset�е�factor�Ӵ�С����
		{
			//if (d1._state->getModes(StateAttribute::POLYGON_OFFSET_FILL) && d2._state->getModes(StateAttribute::POLYGON_OFFSET_FILL))
			//{
			//	
			//}
			//else if ()
			//{

			//}
			if (d1._state != nullptr && d2._state !=nullptr)
			{
				return (d1._state->whichIsBeforeRend(d2._state));
			}
			return false;
		});

		//����
		std::vector<OnceDrawAttribute> noblendAttributes;
		std::vector<OnceDrawAttribute> blendAttributes;
		for (int i = 0; i < nodrawAttributes.size(); i++)
		{
			if (nodrawAttributes[i]._state->getModes(StateAttribute::BLENDFUNC))
			{
				blendAttributes.push_back(nodrawAttributes[i]);
			}
			else
			{
				noblendAttributes.push_back(nodrawAttributes[i]);
			}
		}

		
		//��û��renderpass�Ĳ���
		
		for (int i = 0; i < noblendAttributes.size(); i++)
		{
			noblendAttributes[i]._state->Bind();
			noblendAttributes[i]._vao->SetDrawBuffer(noblendAttributes[i]._drawbuffer->ID());
			int _Count = noblendAttributes[i].drawCount;
			GLenum drawMode = noblendAttributes[i]._state->getDrawMode();
			if (noblendAttributes[i].HaveIBO)
			{
				noblendAttributes[i]._vao->DrawIndirectElement(drawMode, 0, _Count);
			}
			else
			{
				noblendAttributes[i]._vao->DrawIndirectArray(drawMode, 0, _Count);
			}
			
			noblendAttributes[i]._state->UnBind();
		}

		for (int i = 0; i < blendAttributes.size(); i++)
		{
			blendAttributes[i]._state->Bind();
			blendAttributes[i]._vao->SetDrawBuffer(blendAttributes[i]._drawbuffer->ID());
			int _Count = blendAttributes[i].drawCount;
			GLenum drawMode = blendAttributes[i]._state->getDrawMode();
			if (blendAttributes[i].HaveIBO)
			{
				blendAttributes[i]._vao->DrawIndirectElement(drawMode, 0, _Count);
			}
			else
			{
				blendAttributes[i]._vao->DrawIndirectArray(drawMode, 0, _Count);
			}

			blendAttributes[i]._state->UnBind();
		}
		
		/*
		for (int i = 0; i < _vaoattributes.size(); i++)
		{
			for (auto item : _vaoattributes[i]._arraystates)
			{
				item.first->Bind();

				_vaoattributes[i].vao->DrawArrays(item.second, item.first->getDrawMode());
				item.first->UnBind();
			}
		}
		*/
	}

	void NodeVisitor::RemoveDrawableAttribute(Drawable * node)
	{
		if (_vaoattributes.size() == 0)
		{
			return;
		}
		for (auto item = drawableattributes.begin(); item != drawableattributes.end(); )
		{
			if (item->size() == 0)
			{
				item = drawableattributes.erase(item);
			}
			else
			{
				item++;
			}
		}

		for (int i = 0; i < drawableattributes.size(); i++)
		{
			if (drawableattributes[i][0].drawable->CompareForm(node))
			{
				//������ڸ�drawable,�򷵻�
				for (auto item = drawableattributes[i].begin(); item != drawableattributes[i].end(); item++)
				{
					if (item->drawable == node)
					{				
						RemoveVaoAttribute(node, i);
						drawableattributes[i].erase(item);
						if (drawableattributes[i].size() == 0)
						{
							drawableattributes.erase(drawableattributes.begin() + i);
						}
						return;
					}
				}			
			}
		}

		
	}

	void NodeVisitor::RemoveAllData()
	{
		for (auto &iter : _vaoattributes)
		{
			iter.vao.reset();
			iter.vbos.clear();
			iter._arraystates.clear();
			iter._drawbuffers.clear();
			iter._elementstates.clear();
		}
		std::vector<VaoDrawArrayAttribute>().swap(_vaoattributes);
		//_vaoattributes.clear();
		//for (auto &iter : drawableattributes)
		//{
		//	for (int i = 0; i < iter.size(); i++)
		//	{
		//		delete iter[i].drawable;
		//	}
		//}
		std::vector<std::vector<DrawableAttribute>>().swap(drawableattributes);
		//drawableattributes.clear();
	}

	void NodeVisitor::RemoveVaoAttribute(Drawable * node, int index)
	{
		if (_vaoattributes.size() == 0)
		{
			return;
		}
		Drawable::ArrayList arraylist = node->getVertexAttribArrayList();
		std::map<int, BufferAttribute> _bufferattribute;
		BufferAttribute _indexattribute;
		for (int i = 0; i < arraylist.size(); i++)
		{
			if (_vaoattributes[index].vbos.find(i) != _vaoattributes[index].vbos.end())
			{
				int _size = arraylist[i]->size()*glgetTypeSize(arraylist[i]->getDataType());
				if (_vaoattributes[index].vbos[i]->Size() - _size == 0)
				{
					//ȫ��ɾ��
					_vaoattributes.erase(_vaoattributes.begin() + index);
					return;
				}
				else
				{
					BufferAttribute att = arraylist[i]->getBufferAttribute();
					//ɾ��һ��������
					_vaoattributes[index].vbos[i]->DeleteData(att._start, att._end - att._start);
					_bufferattribute[i] = att;
				}
				
			}
		}	
		//ɾ��index����
		if (_vaoattributes[index].HaveIBO)
		{
			BufferAttribute att = node->getIndexAttribute()->getBufferAttribute();
			_vaoattributes[index]._ibo->DeleteData(att._start, att._end - att._start);
			_indexattribute = att;
		}

		//��������vao
		_vaoattributes[index].isSetVAOFormat = false;
		//_vaoattributes[index].vao = MakeAsset<VAO>();

		//�������øø�ʽ������drawable������
		for (auto item = drawableattributes[index].begin(); item != drawableattributes[index].end(); item++)
		{
			//���drawable���������
			if (item->drawable == node)
			{
				continue;
			}	
			Drawable::ArrayList arraylist = item->drawable->getVertexAttribArrayList();			
			for (int i = 0; i < arraylist.size(); i++)
			{
				if (arraylist[i])
				{
					BufferAttribute att = arraylist[i]->getBufferAttribute();
					if (att._start < _bufferattribute[i]._start)
					{
						continue;
					}
					else
					{
						att._start -= (_bufferattribute[i]._end - _bufferattribute[i]._start);
						att._end -= (_bufferattribute[i]._end - _bufferattribute[i]._start);
						arraylist[i]->setBufferAttribute(att);
					}
				}
			}	

			Array *indexarray = item->drawable->getIndexAttribute();
			if (indexarray)
			{
				BufferAttribute att = indexarray->getBufferAttribute();
				if (att._start < _indexattribute._start)
				{
					continue;
				}
				else
				{
					att._start -= (_indexattribute._end - _indexattribute._start);
					att._end -= (_indexattribute._end - _indexattribute._start);
					item->drawable->getIndexAttribute()->setBufferAttribute(att);
				}
				item->drawable->SetVAO(_vaoattributes[index].vao);
			}
		}


		QOpenGLContext *context = QOpenGLContext::currentContext();
		GLenum error = context->functions()->glGetError();
		qDebug() << error;
	}

	
}