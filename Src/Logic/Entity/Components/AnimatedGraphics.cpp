/**
@file AnimatedGraphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de una entidad estática.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author Rubén Mulero Guerrero
@author Antonio Jesús Narváez Corrales

@date March, 2013
*/

#include "AnimatedGraphics.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "WeaponType.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);

	CAnimatedGraphics::~CAnimatedGraphics(){
		if(_weapons != NULL) delete _weapons;
	}
	
	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
		if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;
		
		_animatedGraphicsEntity->setTransform(_entity->getPosition(),_entity->getOrientation());
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
		}

		//cargamos los modelos de las armas para poder ponerselas en la mano conforme los jugadores cambien de arma

			_weapons = new  Graphics::CEntity*[WeaponType::eSIZE];	

			_scene = _entity->getMap()->getScene();
			WeaponType::Enum current;
			for(int i = WeaponType::eSOUL_REAPER; i < WeaponType::eSIZE; ++i){
				
				current = (WeaponType::Enum)i;
				std::stringstream aux;
				aux << "weapon" << WeaponType::toString(current);
				std::string weapon = aux.str();

				aux << "_" << _entity->getEntityID();

				std::string nameWeapon = (aux.str());
				//creamos la entidad gráfica del arma para poder atacharla al monigote
				//HACK MEGA BASTISIMO LOOOOOOL
				//Explicacion del hack, si no atacho todas las armas al bone, tiene pinta de que
				//ogre coge y me las borra para optimizar, por lo que al atacharlas luego no existen T-T
				Graphics::CEntity* entity = new Graphics::CEntity(nameWeapon,entityInfo->getStringAttribute(weapon+"Model"));

				_animatedGraphicsEntity->attachWeapon(*entity, _entity->getEntityID());
				_weapons[i] =  entity;
			}

		if(!_weapons)
			return NULL;
		
		
		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	void CAnimatedGraphics::onActivate()
	{
		CGraphics::onActivate();

		_animatedGraphicsEntity->setAnimation( _defaultAnimation, true );
		_animatedGraphicsEntity->attachWeapon(*_weapons[0], _entity->getEntityID());
		_insertAnimation = true;
		_animatedGraphicsEntity->addObserver(this);
	}
	//---------------------------------------------------------

	void CAnimatedGraphics::onDeactivate()
	{
		CGraphics::onDeactivate();
		_animatedGraphicsEntity->removeObserver(this);
		_insertAnimation = false;
		
	}
	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return CGraphics::accept(message)					||
			   msgType == Message::SET_ANIMATION			||
			   msgType == Message::STOP_ANIMATION			||
			   msgType == Message::CHANGE_WEAPON_GRAPHICS	;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(const std::shared_ptr<CMessage>& message) {
		CGraphics::process(message);
		switch( message->getMessageType() ) {
			/*case Message::CHANGE_MATERIAL: {
				std::shared_ptr<CMessageChangeMaterial> chgMatMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);
				changeMaterial( chgMatMsg->getMaterialName() );
				break;
			}*/
			case Message::SET_ANIMATION: {
				std::shared_ptr<CMessageSetAnimation> setAnimMsg = std::static_pointer_cast<CMessageSetAnimation>(message);

				// Paramos todas las animaciones antes de poner una nueva.
				// Por debajo, el animated Entity nos va a interpolar las animaciones
				// para que todo quede mejor
				if(_insertAnimation){

					_animatedGraphicsEntity->stopAllAnimations();
					_animatedGraphicsEntity->setAnimation( setAnimMsg->getAnimation(), setAnimMsg->getLoop() );
					_insertAnimation = setAnimMsg->getExclude();

				}else if(!_insertAnimation && setAnimMsg->getExclude()){
					
					_animatedGraphicsEntity->stopAllAnimations();
					_animatedGraphicsEntity->setAnimation( setAnimMsg->getAnimation(), setAnimMsg->getLoop() );
					
				}else{

					nextAnim.animation = setAnimMsg->getAnimation();
					nextAnim.loop = setAnimMsg->getLoop();
					nextAnim.exclude = setAnimMsg->getExclude();

				}
				break;
			}
			case Message::STOP_ANIMATION: {
				std::shared_ptr<CMessageStopAnimation> stopAnimMsg = std::static_pointer_cast<CMessageStopAnimation>(message);
				_animatedGraphicsEntity->stopAnimation( stopAnimMsg->getString() );
				break;
			}
			case Message::CHANGE_WEAPON_GRAPHICS: {
				std::shared_ptr<CMessageChangeWeaponGraphics> chgWeaponGraphMsg = std::static_pointer_cast<CMessageChangeWeaponGraphics>(message);
				changeWeapon( chgWeaponGraphMsg->getWeapon() );
				break;
			}
		}

	} // process
	
	//---------------------------------------------------------

	void CAnimatedGraphics::onTick(unsigned int msecs){
		Matrix4 transform(_entity->getOrientation());
		Matrix3 rot;
		transform.setTrans(_entity->getPosition());
		Math::setYaw( Math::getYaw( transform ) + Math::PI,transform );
		transform.extract3x3Matrix(rot);
		_graphicsEntity->setTransform( transform.getTrans(),  rot);
	}//---------------------------------------------------------
	//onTick
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		if(!_insertAnimation && nextAnim.animation.size()>0){

			_insertAnimation = true;
			_animatedGraphicsEntity->setAnimation( nextAnim.animation,nextAnim.loop );
			_insertAnimation = nextAnim.exclude;
			_entity->getYaw();
			nextAnim.animation="";
		}
	}

	//---------------------------------------------------------
	void CAnimatedGraphics::changeWeapon(int newWeapon){
		if(newWeapon != _currentWeapon)
			_currentWeapon = newWeapon;
			_animatedGraphicsEntity->attachWeapon( *_weapons[_currentWeapon] , _entity->getEntityID() );

			//comprobamos si el material que tenia el arma anterior no era el original
			// y si no lo era se lo tenemos que cambiar

			_originalMaterialWeapon = _animatedGraphicsEntity->getWeaponMaterial();

			if(_currentMaterialWeapon != "original")
				_animatedGraphicsEntity->changeMaterialToWeapon( _currentMaterialWeapon );
			//changeMaterial(_currentMaterialWeapon);
	}

	//---------------------------------------------------------
	void CAnimatedGraphics::changeMaterial(const std::string& materialName) {
		if(materialName != _currentMaterialWeapon){
			_currentMaterialWeapon = materialName;
			CGraphics::changeMaterial(_currentMaterialWeapon);
			if(_currentMaterialWeapon != "original"){
				_originalMaterialWeapon = _animatedGraphicsEntity->getWeaponMaterial();
				//_animatedGraphicsEntity->changeMaterialToWeapon(materialName);
			}else{
				//_animatedGraphicsEntity->changeMaterialToWeapon(_originalMaterialWeapon);
			}
		}
	}
} // namespace Logic
