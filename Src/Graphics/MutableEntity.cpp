//---------------------------------------------------------------------------
// MutableEntity.cpp
//---------------------------------------------------------------------------

/**
@file MutableEntity.cpp

@see Graphics::CMutableEntity

@author Francisco Aisa García
@date Abril, 2013
*/

#include "MutableEntity.h"

#include "OgreMesh.h"
#include "OgreSubMesh.h"

namespace Graphics {

	CMutableEntity::CMutableEntity(const std::string &name, const std::string &mesh)
		: CEntity(name, mesh) {


	} // CEntity

	//--------------------------------------------------------

	CMutableEntity::~CMutableEntity() {
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
	} // ~CEntity

	//--------------------------------------------------------

	void CMutableEntity::getMeshInformation(const Ogre::MeshPtr& mesh,size_t &vertex_count, Ogre::Vector3* &vertices,
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
		vertices = new Ogre::Vector3[vertex_count];
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
 
					Ogre::Vector3 pt;
 
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
