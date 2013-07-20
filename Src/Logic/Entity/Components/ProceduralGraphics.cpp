/**
@file ProceduralGraphics.cpp

Contiene la implementación del componente que controla las animaciones
procedurales que se aplican a la entidad
 
@see Logic::ProceduralGraphics
@see Logic::IComponent

@author Rubén Mulero Guerrero
@date June, 2013
*/

#include "ProceduralGraphics.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"

namespace Logic 
{
	IMP_FACTORY(CProceduralGraphics);

	//---------------------------------------------------------

	CProceduralGraphics::CProceduralGraphics() : _insertAnimation(true),
												 _lowerPitchBound(-0.6f),
												 _upperPitchBound(0.4f),
												 _lowerYawBound(-0.3f),
												 _upperYawBound(0.3f),
												 _animatedEntity(NULL) { 
	
		// Nada que hacer
	}

	//---------------------------------------------------------

	CProceduralGraphics::~CProceduralGraphics() {
		// Nada que hacer
	}
	
	//---------------------------------------------------------
	
	bool CProceduralGraphics::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("aimingBoneName") );
		_masterBoneName = entityInfo->getStringAttribute("aimingBoneName");

		if( entityInfo->hasAttribute("upperPitchBound") )
			_upperPitchBound = entityInfo->getFloatAttribute("upperPitchBound");
		if( entityInfo->hasAttribute("lowerPitchBound") )
			_lowerPitchBound = entityInfo->getFloatAttribute("lowerPitchBound");
		if( entityInfo->hasAttribute("upperYawBound") )
			_upperYawBound = entityInfo->getFloatAttribute("upperYawBound");
		if( entityInfo->hasAttribute("lowerYawBound") )
			_lowerYawBound = entityInfo->getFloatAttribute("lowerYawBound");

		return true;
	}
	
	//---------------------------------------------------------

	void CProceduralGraphics::onActivate() {
		_animatedEntity = _entity->getComponent<CAnimatedGraphics>("CAnimatedGraphics")->getAnimatedEntity();
		_animatedEntity->addObserver(this);
		_animatedEntity->freeBoneOrientation(_masterBoneName);
	}
	//---------------------------------------------------------

	void CProceduralGraphics::onDeactivate() {
		_animatedEntity->removeObserver(this);
	}
	
	//---------------------------------------------------------

	bool CProceduralGraphics::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_ANIMATION;

	} // accept
	
	//---------------------------------------------------------

	void CProceduralGraphics::process(const std::shared_ptr<CMessage>& message) {

		switch( message->getMessageType() ) {
			case Message::SET_ANIMATION: {
				std::shared_ptr<CMessageSetAnimation> setAnimMsg = std::static_pointer_cast<CMessageSetAnimation>(message);

				/*if(_insertAnimation && setAnimMsg->getExclude()){
					_insertAnimation = false;
					_animatedEntity->lockBoneOrientation("weapon_bone_2");
				}*/
				
			}
		}

	} // process

	//---------------------------------------------------------

	void CProceduralGraphics::onTick(unsigned int msecs) {
		//movemos el hueso en función de la orientación de la entidad
		if(!_insertAnimation)
			return;
		
		float pitch = _entity->getPitch().getPitch().valueRadians();

		if(pitch > _upperPitchBound) {
			pitch = _upperPitchBound;
		}
		else if(pitch < _lowerPitchBound) {
			pitch = _lowerPitchBound;
		}

		_animatedEntity->moveBone(_masterBoneName, -pitch);
	}
	
	//---------------------------------------------------------
	//onTick
	
	void CProceduralGraphics::animationFinished(const std::string &animation) {
		if(!_insertAnimation) {
			_insertAnimation = true;
			_animatedEntity->freeBoneOrientation(_masterBoneName);
		}
	}

} // namespace Logic