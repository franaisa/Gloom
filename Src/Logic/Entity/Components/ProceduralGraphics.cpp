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

	
	//---------------------------------------------------------

	void CProceduralGraphics::onActivate()
	{
		_animatedEntity = _entity->getComponent<CAnimatedGraphics>("CAnimatedGraphics")->getAnimatedEntity();
		_animatedEntity->addObserver(this);
		_animatedEntity->freeBoneOrientation("bip_spine_3");
	}
	//---------------------------------------------------------

	void CProceduralGraphics::onDeactivate()
	{
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

	void CProceduralGraphics::onTick(unsigned int msecs){

		//movemos el hueso en función de la orientación de la entidad
		if(!_insertAnimation)
			return;
		
		float pitch = _entity->getPitch().getPitch().valueRadians();

		if(pitch>0.4){
			pitch = 0.4;
		}else if(pitch < -0.6){
			pitch = -0.6;
		}

		_animatedEntity->moveBone("bip_spine_3", pitch*-1);

	}//---------------------------------------------------------
	//onTick
	
	void CProceduralGraphics::animationFinished(const std::string &animation)
	{
		if(!_insertAnimation){
			_insertAnimation = true;
			_animatedEntity->freeBoneOrientation("bip_spine_3");
		}
	}
} // namespace Logic