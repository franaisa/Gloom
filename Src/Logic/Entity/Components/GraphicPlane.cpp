/**
@file GraphicPlane.cpp


@see Logic::CGraphicPlane
@see Logic::IComponent

*/

#include "GraphicPlane.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Plane.h"

#include "Map/MapEntity.h"

#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreEntity.h>
#include <Ogre/OgreMeshManager.h>

namespace Logic {

	IMP_FACTORY(CGraphicPlane);

	//________________________________________________________________________

	CGraphicPlane::CGraphicPlane() : _plane(NULL),
									 _castShadows(false) {
		// Nada que hacer
	}
	
	//________________________________________________________________________

	CGraphicPlane::~CGraphicPlane() {
		if(_plane != NULL)
			delete _plane;
	} // ~CGraphics
	
	//________________________________________________________________________

	bool CGraphicPlane::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute("plane_orientation") );
		assert( entityInfo->hasAttribute("plane_width") );
		assert( entityInfo->hasAttribute("plane_height") );
		assert( entityInfo->hasAttribute("plane_xSegments") );
		assert( entityInfo->hasAttribute("plane_ySegments") );
		assert( entityInfo->hasAttribute("plane_uTiling") );
		assert( entityInfo->hasAttribute("plane_vTiling") );
		assert( entityInfo->hasAttribute("plane_upVector") );
		assert( entityInfo->hasAttribute("plane_material") );
		
		_orientation = entityInfo->getVector3Attribute("plane_orientation");
		_width = entityInfo->getFloatAttribute("plane_width");
		_height = entityInfo->getFloatAttribute("plane_height");
		_xSegments = entityInfo->getIntAttribute("plane_xSegments");
		_ySegments = entityInfo->getIntAttribute("plane_ySegments");
		_uTiling = entityInfo->getFloatAttribute("plane_uTiling");
		_vTiling = entityInfo->getFloatAttribute("plane_vTiling");
		_upVector = entityInfo->getVector3Attribute("plane_upVector");
		_materialName = entityInfo->getStringAttribute("plane_material");

		_lightMask = readLightMask(entityInfo);

		if( entityInfo->hasAttribute("plane_castShadows") )
			_castShadows = entityInfo->getBoolAttribute("plane_castShadows");

		return true;
	}

	//________________________________________________________________________

	void CGraphicPlane::onStart() {
		Graphics::CScene* activeScene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		_plane = new Graphics::CPlane( activeScene, "plane_" + _entity->getName(), _entity->getPosition(), 
									   _orientation, _width, _height, _xSegments, _ySegments, _uTiling, _vTiling, _upVector, 
									   _materialName, _lightMask, _castShadows );

	} // spawn

	//________________________________________________________________________

	unsigned int CGraphicPlane::readLightMask(const Map::CEntity *entityInfo) {
		std::vector<unsigned int> lightGroups;

		if( entityInfo->hasAttribute("lightMask") ) {
			std::istringstream groupListString( entityInfo->getStringAttribute("lightMask") );

			// Para cada cadena entre comas...
			do {
				std::string groupNumber;
				std::getline(groupListString, groupNumber, ',');	// linea entre delimitadores
				
				std::istringstream str(groupNumber);				// wrappeamos cadena como Input Stream
				do {												// Le quitamos los espacios...
					std::getline(str, groupNumber, ' ');			// linea entre espacios
				} while ( groupNumber.size() == 0 && !str.eof() );

				lightGroups.push_back( atoi(groupNumber.c_str()) );
			} while ( !groupListString.eof() );
		}

		unsigned int lightMask;
		if( !lightGroups.empty() ) {
			lightMask = 0;
			for(int i = 0; i < lightGroups.size(); ++i) {
				lightMask |= (1 << lightGroups[i]);
			}
		}
		else {
			lightMask = 0xFFFFFFFF;
		}

		return lightMask;
	}

} // namespace Logic

