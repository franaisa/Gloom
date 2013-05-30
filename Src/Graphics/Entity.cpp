//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <Graphics/Server.h>
#include <Graphics/Camera.h>

namespace Graphics 
{
	CEntity::CEntity(const std::string &name, const std::string &mesh)
		: _entity(0), _entityNode(0), _scene(0), _loaded(false)
	{
		_name = name;
		_mesh = mesh;

	} // CEntity

	//--------------------------------------------------------

	CEntity::~CEntity() 
	{
		//assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		if(_scene){
			unload();
			_scene = 0;
		}
		
	} // ~CEntity
	
	//--------------------------------------------------------
		
	bool CEntity::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La entidad debe asociarse a una escena!!");
		// Si la entidad está cargada por otro gestor de escena.
		if(_loaded && (_scene != scene))
			return false;

		// Si no está cargada forzamos su carga.
		if (!_loaded)
		{
			_scene = scene;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	} // attachToScene
	
	//--------------------------------------------------------

	bool CEntity::deattachFromScene()
	{
		// Si la entidad no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;
		// Si la entidad está cargada forzamos su descarga.
		else
		{
			assert(_scene && "¡¡La entidad debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}
		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CEntity::load()
	{
		try
		{
			_entity = _scene->getSceneMgr()->createEntity(_mesh);
			//_entity->setCastShadows(true);
		}
		catch(std::exception e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_node");
		_entityNode->attachObject(_entity);

		/*size_t vertexCount, indexCount;
		Ogre::Vector3* vertexBuffer;
		unsigned* indexBuffer;

		getMeshInformation(_entity->getMesh(), vertexCount, vertexBuffer, indexCount, indexBuffer);*/
			
		return _loaded = true;

	} // load
	
	//--------------------------------------------------------
		
	void CEntity::unload()
	{
		if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			if(_scene->getSceneMgr()->hasSceneNode(_entityNode->getName()))
				_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}

	} // load

	//--------------------------------------------------------
		
	void CEntity::tick(float secs) {
		//_text->update();
	} // tick
	
	//--------------------------------------------------------
		
	void CEntity::setTransform(const Matrix4 &transform)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
		{
			_entityNode->setPosition(transform.getTrans());
			_entityNode->setOrientation(transform.extractQuaternion());
		}

	} // setTransform
	
	//--------------------------------------------------------
		
	Matrix4 CEntity::getTransform() {
			Matrix4 result;
			result.makeTransform(_entityNode->getPosition(),_entityNode->getScale(),_entityNode->getOrientation());
			return result;
	}// getTransform

	//--------------------------------------------------------

	void CEntity::setOrientation(const Matrix3 &orientation)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setOrientation(orientation);

	} // setOrientation
	
	//--------------------------------------------------------
		
	void CEntity::setVisible(bool visible)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
		
	const bool CEntity::getVisible()
	{
		if(_entityNode)
			return _entity->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	//--------------------------------------------------------
		
	void CEntity::setPosition(const Vector3 &position)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setPosition(position);

	} // setPosition
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const Vector3 &scale)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setScale(scale);

	} // setScale
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const float scale)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
		{
			Vector3 scaleVector(scale,scale,scale);
			_entityNode->setScale(scaleVector);
		}

	} // setScale

	void CEntity::setOgreEntity(Ogre::Entity* entity){
		_entity=entity;
	}

	//--------------------------------------------------------

	void CEntity::changeMaterial(const std::string& materialName) {
		_entity->setMaterialName(materialName);
	}

	//--------------------------------------------------------

	void CEntity::getMeshInformation(const Ogre::MeshPtr& mesh,size_t &vertex_count, Ogre::Vector3* &vertices,
									 size_t &index_count, unsigned* &indices,
									 const Ogre::Vector3 &position,
									 const Ogre::Quaternion &orient,
									 const Ogre::Vector3 &scale) {

		vertex_count = index_count = 0;
 
		bool added_shared = false;
		size_t current_offset = vertex_count;
		size_t shared_offset = vertex_count;
		size_t next_offset = vertex_count;
		size_t index_offset = index_count;
		size_t prev_vert = vertex_count;
		size_t prev_ind = index_count;
 
		// Calculate how many vertices and indices we're going to need
		for(int i = 0;i < mesh.get()->getNumSubMeshes();i++)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
			// We only need to add the shared vertices once
			if(submesh->useSharedVertices)
			{
				if(!added_shared)
				{
					Ogre::VertexData* vertex_data = mesh->sharedVertexData;
					vertex_count += vertex_data->vertexCount;
					added_shared = true;
				}
			}
			else
			{
				Ogre::VertexData* vertex_data = submesh->vertexData;
				vertex_count += vertex_data->vertexCount;
			}
 
			// Add the indices
			Ogre::IndexData* index_data = submesh->indexData;
			index_count += index_data->indexCount;
		}
 
		// Allocate space for the vertices and indices
		vertices = new Vector3[vertex_count];
		indices = new unsigned[index_count];
 
		added_shared = false;
 
		// Run through the submeshes again, adding the data into the arrays
		for(int i = 0;i < mesh->getNumSubMeshes();i++)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
			Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
			if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
			{
				if(submesh->useSharedVertices)
				{
					added_shared = true;
					shared_offset = current_offset;
				}
 
				const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
				Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
				unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				Ogre::Real* pReal;
 
				for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
				{
					posElem->baseVertexPointerToElement(vertex, &pReal);
 
					Vector3 pt;
 
					pt.x = (*pReal++);
					pt.y = (*pReal++);
					pt.z = (*pReal++);
 
					pt = (orient * (pt * scale)) + position;
 
					vertices[current_offset + j].x = pt.x;
					vertices[current_offset + j].y = pt.y;
					vertices[current_offset + j].z = pt.z;
				}
				vbuf->unlock();
				next_offset += vertex_data->vertexCount;
			}
 
			Ogre::IndexData* index_data = submesh->indexData;
 
			size_t numTris = index_data->indexCount / 3;
			unsigned short* pShort;
			unsigned int* pInt;
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
			bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
			if (use32bitindexes) pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			else pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
			for(size_t k = 0; k < numTris; ++k)
			{
				size_t offset = (submesh->useSharedVertices)?shared_offset:current_offset;
 
				unsigned int vindex = use32bitindexes? *pInt++ : *pShort++;
				indices[index_offset + 0] = vindex + offset;
				vindex = use32bitindexes? *pInt++ : *pShort++;
				indices[index_offset + 1] = vindex + offset;
				vindex = use32bitindexes? *pInt++ : *pShort++;
				indices[index_offset + 2] = vindex + offset;
 
				index_offset += 3;
			}
			ibuf->unlock();
			current_offset = next_offset;
		}
	}

} // namespace Graphics
