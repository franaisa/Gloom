/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "ArrayGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


#include "Graphics/Camera.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"

#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>

namespace Logic 
{
	IMP_FACTORY(CArrayGraphics);
	
	//---------------------------------------------------------

	CArrayGraphics::~CArrayGraphics() 
	{
		if(_graphicsEntities)
		{
			for(int i = 0; i < _numWeapons; ++i)
			{
				//_scene->removeChild(_graphicsEntities[i]._graphicsEntity);
				delete _graphicsEntities[i]._graphicsEntity;
				_graphicsEntities[i]._graphicsEntity = NULL;
			}
			delete _graphicsEntities;
		}

	} // ~CGraphics
	//---------------------------------------------------------
	
	void CArrayGraphics::onActivate()
	{
		//Cuando activamos el componente solo tendremos visible el arma 0( arma melee)
		_graphicsEntities[0]._graphicsEntity->setVisible(true);

	} // activate
	//---------------------------------------------------------

	void CArrayGraphics::onDeactivate()
	{
		//Cuando desactivamos el componente, desactivaremos el arma actual
		_graphicsEntities[_currentWeapon]._graphicsEntity->setVisible(false);

	} // deactivate
	//---------------------------------------------------------

	bool CArrayGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();
		
		if(entityInfo->hasAttribute("numWeapons")){
			int numWeapons = entityInfo->getIntAttribute("numWeapons");
			
			_graphicsEntities = new TGraphicsWeapon[numWeapons];



			// Por ahora leo a mano cada una de las armas que tiene el usuario

			std::string armas[] = {"hammer","sniper","shotGun","miniGun", "grenadeLauncher", "rocketLauncher"};

			
			for(int i = 0; i < numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << armas[i];
				std::string weapon = aux.str();
				
				printf("%s", weapon.c_str());
				
				_graphicsEntities[i]._graphicsEntity = createGraphicsEntity(weapon, entityInfo->getStringAttribute(weapon+"Model"));
				assert(_graphicsEntities[i]._graphicsEntity != 0 && "error al cargar la entidad grafica");
				if(entityInfo->hasAttribute(weapon+"ModelYaw"))
					_graphicsEntities[i].yaw = entityInfo->getFloatAttribute(weapon+"ModelYaw");
				if(entityInfo->hasAttribute(weapon+"ModelPitch"))
					_graphicsEntities[i].pitch = entityInfo->getFloatAttribute(weapon+"ModelPitch");
				if(entityInfo->hasAttribute(weapon+"ModelRoll"))
					_graphicsEntities[i].pitch = entityInfo->getFloatAttribute(weapon+"ModelRoll");
				
				//Esto perta y creo q es necesario.
				_graphicsEntities[i].offset = new Vector3(entityInfo->getVector3Attribute(weapon+"Offset"));
				
				if(i!=0) _graphicsEntities[i]._graphicsEntity->setVisible(false);
			}
		}
		if(!_graphicsEntities)
			return false;
		
		setTransform();

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CArrayGraphics::createGraphicsEntity(std::string nombreEntidad, std::string modelo)
	{
		Graphics::CEntity *graphicsEntity = new Graphics::CEntity(nombreEntidad,modelo);
		if(!_scene->addEntity(graphicsEntity))
			return 0;

		return graphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	bool CArrayGraphics::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_TRANSFORM				||
			   msgType == Message::CHANGE_WEAPON_GRAPHICS;
	} // accept
	
	//---------------------------------------------------------

	void CArrayGraphics::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON_GRAPHICS: {
				std::shared_ptr<CMessageChangeWeaponGraphics> chgWpnMsg = std::static_pointer_cast<CMessageChangeWeaponGraphics>(message);
				changeWeapon( chgWpnMsg->getWeapon() );
				break;
			}
		}

	} // process

	//---------------------------------------------------------

	void CArrayGraphics::changeWeapon(int newWeapon){
		
		_graphicsEntities[_currentWeapon]._graphicsEntity->setVisible(false);
		_graphicsEntities[newWeapon]._graphicsEntity->setVisible(true);
		_currentWeapon = newWeapon;
		
		
		setTransform();
		
	}

	//---------------------------------------------------------

	void CArrayGraphics::setTransform(){
		
		
		//en un futuro simplemente se orientara la entidad en el 3ds o similar. 

		// Obtengo la camara para posicionarla en esta posicion pero algo modificada
		Graphics::CCamera* camera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
		Vector3 direction = /*camera->getTargetCameraPosition() -*/ camera->getCameraPosition();
		
		direction.normalise();
		//Vector3 posicionModificada = camera->getCameraPosition() - Vector3(0,2.5,0) + ((8.0f) * direction);
		Vector3 posicionModificada = camera->getCameraPosition() + ((8.0f) * direction);
		
		//Aqui establezco la rotacion (En un futuro se rotara el modelo)
		Matrix4 transformModificado = _entity->getTransform();
		//Math::setPitchYawRoll(_graphicsEntities[_currentWeapon].pitch, _graphicsEntities[_currentWeapon].yaw, _graphicsEntities[_currentWeapon].roll, transformModificado);
		_graphicsEntities[_currentWeapon]._graphicsEntity->setTransform(transformModificado);
		_graphicsEntities[_currentWeapon]._graphicsEntity->setPosition(posicionModificada);
	}// setTransform

	//---------------------------------------------------------

	void CArrayGraphics::onTick(unsigned int msecs){
		setTransform();
	}

} // namespace Logic

