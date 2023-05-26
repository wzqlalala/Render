#include "mPreMeshPickData1.h"

namespace MDataPre
{
	void mPreMeshPickData1::setMeshPickFunction(int pickfunction)
	{
		_pickFunction = pickfunction;
	}

	mPreMeshPickData1::mPreMeshPickData1()
	{

	}

	mPreMeshPickData1::~mPreMeshPickData1()
	{

	}

	void mPreMeshPickData1::setSoloPickNodeData(MXMeshVertex* nodeid, float depth)
	{
		if (nodeid != 0)
		{
			if (depth < _nodeBuffer.depth)
			{
				_nodeBuffer.depth = depth;
				_nodeBuffer.vertex = nodeid;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshData(MXMeshElement* meshid, float depth)
	{
		if (meshid != 0)
		{
			if (depth < _meshBuffer.depth)
			{
				_meshBuffer.depth = depth;
				_meshBuffer.element = meshid;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshLineData(int meshlineid, float depth)
	{
		if (meshlineid != 0)
		{
			if (depth < _meshLineBuffer.depth)
			{
				_meshLineBuffer.depth = depth;
				_meshLineBuffer.id = meshlineid;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshFaceData(MFace* meshfaceid, float depth)
	{
		if (meshfaceid != 0)
		{
			if (depth < _meshFaceBuffer.depth)
			{
				_meshFaceBuffer.depth = depth;
				_meshFaceBuffer.meshface = meshfaceid;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshPartData(QString partName, float depth)
	{
		if (partName.size()!=0)
		{
			if (depth < _partBuffer.depth)
			{
				_partBuffer.depth = depth;
				_partBuffer.name = partName;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickNodeByPartData(set<MXMeshVertex*> nodeids, float depth)
	{
		if (nodeids.size() != 0)
		{
			if (depth < _nodeBuffers.depth)
			{
				_nodeBuffers.depth = depth;
				_nodeBuffers.vertexs = nodeids;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshByPartData(set<MXMeshElement*> meshids, float depth)
	{
		if (meshids.size() != 0)
		{
			if (depth < _meshBuffers.depth)
			{
				_meshBuffers.depth = depth;
				_meshBuffers.elements = meshids;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshLineByPartData(set<int> meshLineids, float depth)
	{
		if (meshLineids.size() != 0)
		{
			if (depth < _meshLineBuffers.depth)
			{
				_meshLineBuffers.depth = depth;
				_meshLineBuffers.ids = meshLineids;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshFaceByPartData(set<MFace*> meshFaceids, float depth)
	{
		if (meshFaceids.size() != 0)
		{
			if (depth < _meshFaceBuffers.depth)
			{
				_meshFaceBuffers.depth = depth;
				_meshFaceBuffers.meshfaces = meshFaceids;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshDataByAngle(MXMeshElement* meshid, QString partName, float depth)
	{
		if (meshid != 0)
		{
			if (depth < _meshPartNameBuffers.depth)
			{
				//_meshPartNameBuffers.depth = depth;
				//_meshPartNameBuffers.partName = partName;
				//_meshPartNameBuffers. = meshid;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshLineDataByAngle(int meshlineid, QString partName, float depth)
	{
		if (meshlineid != 0)
		{
			//if (depth < _meshLinePartNameBuffers.depth)
			//{
			//	_meshLinePartNameBuffers.depth = depth;
			//	_meshLinePartNameBuffers.partName = partName;
			//	_meshLinePartNameBuffers.ptr = meshlineid;
			//}
		}
	}

	void mPreMeshPickData1::setSoloPickMeshFaceDataByAngle(void * meshfaceid, QString partName, float depth)
	{
		if (meshfaceid != 0)
		{
			if (depth < _meshFacePartNameBuffers.depth)
			{
				_meshFacePartNameBuffers.depth = depth;
				_meshFacePartNameBuffers.partName = partName;
				_meshFacePartNameBuffers.ptr = meshfaceid;
			}
		}
	}

	void mPreMeshPickData1::setSoloPickData()
	{
		if (_nodeBuffer.vertex != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickNodes.insert(_nodeBuffer.vertex); break;
			case 1:
				_pickNodes.erase(_nodeBuffer.vertex); break;
			}

			_nodeBuffer.initial();
		}
		if (_meshBuffer.element != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickMeshs.insert(_meshBuffer.element); break;
			case 1:
				_pickMeshs.erase(_meshBuffer.element); break;
			}

			_meshBuffer.initial();
		}
		if (_meshLineBuffer.id != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickMeshLines.insert(_meshLineBuffer.id); break;
			case 1:
				_pickMeshLines.erase(_meshLineBuffer.id); break;
			}
			_meshLineBuffer.initial();
		}
		if (_meshFaceBuffer.meshface != 0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickMeshFaces.insert(_meshFaceBuffer.meshface); break;
			case 1:
				_pickMeshFaces.erase(_meshFaceBuffer.meshface); break;
			}
			_meshFaceBuffer.initial();
		}
		if (_partBuffer.name.size()!=0)
		{
			switch (_pickFunction)
			{
			case 0:
				_pickParts.insert(_partBuffer.name); break;
			case 1:
				_pickParts.erase(_partBuffer.name); break;
			}
			_partBuffer.initial();
		}
		if (_nodeBuffers.vertexs.size() != 0)
		{
			setMultiplyPickNodeData(_nodeBuffers.vertexs);
			_nodeBuffers.initial();
		}
		if (_meshBuffers.elements.size() != 0)
		{
			setMultiplyPickMeshData(_meshBuffers.elements);
			_meshBuffers.initial();
		}
		if (_meshLineBuffers.ids.size() != 0)
		{
			setMultiplyPickMeshLineData(_meshLineBuffers.ids);
			_meshLineBuffers.initial();
		}
		if (_meshFaceBuffers.meshfaces.size() != 0)
		{
			setMultiplyPickMeshFaceData(_meshFaceBuffers.meshfaces);
			_meshFaceBuffers.initial();
		}
	}

	void mPreMeshPickData1::setSoloOrderPickData()
	{
		if (_nodeBuffer.vertex != 0)
		{
			_pickNodesOrder.removeOne(_nodeBuffer.vertex);
			switch (_pickFunction)
			{
			case 0:
				_pickNodesOrder.append(_nodeBuffer.vertex); break;
			case 1:
				break;
			}
			_nodeBuffer.initial();
		}
	}

	void mPreMeshPickData1::setMultiplyPickNodeData(std::set<MXMeshVertex*> nodeids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
			{
				_pickNodes.insert(*iter);
			}
			// set_union(_picknodes.begin(), _picknodes.end(), nodeData.begin(), nodeData.end(), inserter(_picknodes, _picknodes.begin()));
		}
		else
		{
			for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
			{
				_pickNodes.erase(*iter);
			}
		}
	}

	void mPreMeshPickData1::setMultiplyPickNodeData(QVector<MXMeshVertex*> nodeids)
	{
		_pickNodesOrder.append(nodeids);
	}

	void mPreMeshPickData1::setMultiplyPickMeshData(std::set<MXMeshElement*> meshids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = meshids.begin(); iter != meshids.end(); ++iter)
			{
				_pickMeshs.insert(*iter);
			}
		}
		else
		{
			for (auto iter = meshids.begin(); iter != meshids.end(); ++iter)
			{
				_pickMeshs.erase(*iter);
			}
		}
	}

	void mPreMeshPickData1::setMultiplyPickMeshLineData(std::set<int> meshlineids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = meshlineids.begin(); iter != meshlineids.end(); ++iter)
			{
				_pickMeshLines.insert(*iter);
			}
		}
		else
		{
			for (auto iter = meshlineids.begin(); iter != meshlineids.end(); ++iter)
			{
				_pickMeshLines.erase(*iter);
			}
		}
	}

	void mPreMeshPickData1::setMultiplyPickMeshFaceData(std::set<MFace*> meshfaceids)
	{
		if (_pickFunction == 0)
		{
			for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
			{
				_pickMeshFaces.insert(*iter);
			}
		}
		else
		{
			for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
			{
				_pickMeshFaces.erase(*iter);
			}
		}
	}

	void mPreMeshPickData1::setMultiplyPickMeshPartData(QString partName)
	{
		if (_pickFunction == 0)
		{
			_pickParts.insert(partName);
		}
		else
		{
			_pickParts.erase(partName);			
		}
	}

	void mPreMeshPickData1::setAllPickNodeData(set<MXMeshVertex*> nodeids)
	{
		_pickNodes = nodeids;
	}

	void mPreMeshPickData1::setAllPickMeshData(set<MXMeshElement*> meshids)
	{
		_pickMeshs = meshids;
	}

	void mPreMeshPickData1::setAllPickMeshLineData(set<int> meshlineids)
	{
		_pickMeshLines = meshlineids;
	}

	void mPreMeshPickData1::setAllPickMeshFaceData(set<MFace*> meshfaceids)
	{
		_pickMeshFaces = meshfaceids;
	}

	void mPreMeshPickData1::setAllPickMeshPartData(set<QString> partNames)
	{
		_pickParts = partNames;
	}

	void mPreMeshPickData1::setAddPickNodeData(set<MXMeshVertex*> nodeids)
	{
		_pickNodes.insert(nodeids.begin(), nodeids.end());
	}

	void mPreMeshPickData1::setAddPickMeshData(set<MXMeshElement*> meshids)
	{
		_pickMeshs.insert(meshids.begin(), meshids.end());
	}

	void mPreMeshPickData1::setAddPickMeshLineData(set<int> meshlineids)
	{
		_pickMeshLines.insert(meshlineids.begin(), meshlineids.end());
	}

	void mPreMeshPickData1::setAddPickMeshFaceData(set<MFace*> meshfaceids)
	{
		_pickMeshFaces.insert(meshfaceids.begin(), meshfaceids.end());
	}

	void mPreMeshPickData1::setAddPickMeshPartData(set<QString> partNames)
	{
		_pickParts.insert(partNames.begin(), partNames.end());
	}

	void mPreMeshPickData1::setReducePickNodeData(set<MXMeshVertex*> nodeids)
	{
		for (auto iter = nodeids.begin(); iter != nodeids.end(); ++iter)
		{
			_pickNodes.erase(*iter);
		}
	}

	void mPreMeshPickData1::setReducePickMeshData(set<MXMeshElement*> meshids)
	{
		for (auto iter = meshids.begin(); iter != meshids.end(); ++iter)
		{
			_pickMeshs.erase(*iter);
		}
	}

	void mPreMeshPickData1::setReducePickMeshLineData(set<int> meshlineids)
	{
		for (auto iter = meshlineids.begin(); iter != meshlineids.end(); ++iter)
		{
			_pickMeshLines.erase(*iter);
		}
	}

	void mPreMeshPickData1::setReducePickMeshFaceData(set<MFace*> meshfaceids)
	{
		for (auto iter = meshfaceids.begin(); iter != meshfaceids.end(); ++iter)
		{
			_pickMeshFaces.erase(*iter);
		}
	}

	void mPreMeshPickData1::setReducePickMeshPartData(set<QString> partNames)
	{
		for (auto iter = partNames.begin(); iter != partNames.end(); ++iter)
		{
			_pickParts.erase(*iter);
		}
	}

	set<MXMeshVertex*> mPreMeshPickData1::getPickNodeIDs()
	{
		return _pickNodes;
	}

	QVector<MXMeshVertex*> mPreMeshPickData1::getPickNodeIDsOrder()
	{
		return _pickNodesOrder;
	}

	set<MXMeshElement*> mPreMeshPickData1::getPickMeshIDs()
	{
		return _pickMeshs;
	}

	set<int> mPreMeshPickData1::getPickMeshLineIDs()
	{
		return _pickMeshLines;
	}

	set<MFace*> mPreMeshPickData1::getPickMeshFaceIDs()
	{
		return _pickMeshFaces;
	}

	set<QString> mPreMeshPickData1::getPickMeshPartNames()
	{
		return _pickParts;
	}

	void mPreMeshPickData1::clearAllPickData()
	{
		_pickNodes.clear();
		_pickNodesOrder.clear();
		_pickMeshs.clear();
		_pickMeshLines.clear();
		_pickMeshFaces.clear();
		_pickParts.clear();
	}

	void mPreMeshPickData1::clearPickNodeData()
	{
		_pickNodes.clear();
		_pickNodesOrder.clear();
	}

	void mPreMeshPickData1::clearPickMeshData()
	{
		_pickMeshs.clear();
	}

	void mPreMeshPickData1::clearPickMeshLineData()
	{
		_pickMeshLines.clear();
	}

	void mPreMeshPickData1::clearPickMeshFaceData()
	{
		_pickMeshFaces.clear();
	}

	void mPreMeshPickData1::clearPickMeshPartData()
	{
		_pickParts.clear();
	}
	QPair<PickObjectType, QPair<QString, void*>> mPreMeshPickData1::getSoloPickNodeDataByLineAngle()
	{
		PickObjectType objectType = PickObjectType::Mesh1D;
		QString partName = _meshPartNameBuffers.partName;
		void *id = _meshPartNameBuffers.ptr;
		if (_meshLinePartNameBuffers.ptr != 0 && _meshPartNameBuffers.ptr != 0)
		{
			if (_meshLinePartNameBuffers.depth < _meshPartNameBuffers.depth)
			{
				objectType = PickObjectType::MeshEdge;
				partName = _meshLinePartNameBuffers.partName;
				id = _meshLinePartNameBuffers.ptr;
			}
		}
		else if (_meshLinePartNameBuffers.ptr != 0)
		{
			objectType = PickObjectType::MeshEdge;
			partName = _meshLinePartNameBuffers.partName;
			id = _meshLinePartNameBuffers.ptr;
		}
		_meshPartNameBuffers.initial();
		_meshLinePartNameBuffers.initial();
		return QPair<PickObjectType, QPair<QString, void*>>(objectType, { partName, id });
	}
	QPair<PickObjectType, QPair<QString, void*>> mPreMeshPickData1::getSoloPickNodeDataByFaceAngle()
	{
		PickObjectType objectType = PickObjectType::Mesh2D;
		QString partName = _meshPartNameBuffers.partName;
		void *id = _meshPartNameBuffers.ptr;
		if (_meshFacePartNameBuffers.ptr != 0 && _meshPartNameBuffers.ptr != 0)
		{
			if (_meshFacePartNameBuffers.depth < _meshPartNameBuffers.depth)
			{
				objectType = PickObjectType::MeshFace;
				partName = _meshFacePartNameBuffers.partName;
				id = _meshFacePartNameBuffers.ptr;
			}
		}
		else if (_meshFacePartNameBuffers.ptr != 0)
		{
			objectType = PickObjectType::MeshFace;
			partName = _meshFacePartNameBuffers.partName;
			id = _meshFacePartNameBuffers.ptr;
		}
		_meshPartNameBuffers.initial();
		_meshFacePartNameBuffers.initial();
		return QPair<PickObjectType, QPair<QString, void*>>(objectType, { partName, id});
	}
	QPair<QString, void*> mPreMeshPickData1::getSoloPickMeshDataByAngle()
	{
		QPair<QString, void*> pair(_meshPartNameBuffers.partName, _meshPartNameBuffers.ptr);
		_meshPartNameBuffers.initial();
		return pair;
	}
	QPair<QString, void*> mPreMeshPickData1::getSoloPickMeshLineDataByAngle()
	{
		QPair<QString, void*> pair(_meshLinePartNameBuffers.partName, _meshLinePartNameBuffers.ptr);
		_meshLinePartNameBuffers.initial();
		return pair;
	}
	QPair<QString, void*> mPreMeshPickData1::getSoloPickMeshFaceDataByAngle()
	{
		QPair<QString, void*> pair(_meshFacePartNameBuffers.partName, _meshFacePartNameBuffers.ptr);
		_meshFacePartNameBuffers.initial();
		return pair;
	}
}
