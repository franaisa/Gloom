/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "HudWeapons.h"

#include "WeaponType.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


#include "Graphics/Camera.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Graphics/Overlay.h"

#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"

#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>

namespace Logic 
{
	IMP_FACTORY(CHudWeapons);
	
	//---------------------------------------------------------

	CHudWeapons::~CHudWeapons() 
	{
		if(_graphicsEntities)
		{
			for(int i = 0; i < _numWeapons; ++i)
			{
				//_scene->removeChild(_graphicsEntities[i]._graphicsEntity);
				delete _graphicsEntities[i].graphicsEntity;
				_graphicsEntities[i].graphicsEntity = NULL;

				delete _overlayWeapon3D[i];
			}
			delete _graphicsEntities;
		}

	} // ~CGraphics
	//---------------------------------------------------------
	
	void CHudWeapons::onActivate()
	{
		//Cuando activamos el componente solo tendremos visible el arma 0( arma melee)
		_graphicsEntities[0].graphicsEntity->setVisible(true);

	} // activate
	//---------------------------------------------------------

	void CHudWeapons::onDeactivate()
	{
		//Cuando desactivamos el componente, desactivaremos el arma actual
		_graphicsEntities[_currentWeapon].graphicsEntity->setVisible(false);

	} // deactivate
	//---------------------------------------------------------

	bool CHudWeapons::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();
		
		if(entityInfo->hasAttribute("numWeapons")){
			int numWeapons = entityInfo->getIntAttribute("numWeapons");
			
		_graphicsEntities = new TGraphicsWeapon[numWeapons];
		
		// Por ahora leo a mano cada una de las armas que tiene el usuario
	
		for(int i = 0; i < numWeapons; ++i){
				
			WeaponType::Enum current = (WeaponType::Enum)i;
			std::string currentOnText = WeaponType::toString(current);
		
			std::stringstream aux;
			aux << "weapon" << currentOnText;
			std::string weapon = aux.str();
				
			//_graphicsEntities[i]._graphicsEntity = createGraphicsEntity(weapon, entityInfo->getStringAttribute(weapon+"Model"));
				
			if(entityInfo->hasAttribute(weapon+"ModelYaw"))
				_graphicsEntities[current].yaw = entityInfo->getFloatAttribute(weapon+"ModelYaw");
			if(entityInfo->hasAttribute(weapon+"ModelPitch"))
				_graphicsEntities[current].pitch = entityInfo->getFloatAttribute(weapon+"ModelPitch");
			if(entityInfo->hasAttribute(weapon+"ModelRoll"))
				_graphicsEntities[current].pitch = entityInfo->getFloatAttribute(weapon+"ModelRoll");
				
			//Esto puede petar si no esta, pero creo q es obligatorio
			if(!entityInfo->hasAttribute(weapon+"Offset"))
				assert("seguro que no tiene offset?");

			_graphicsEntities[current].offset = entityInfo->getVector3Attribute(weapon+"Offset");
				
			
			// Ahora voy a crear los overlays por cada arma en 3D

			Graphics::CServer *server = Graphics::CServer::getSingletonPtr();

			_overlayWeapon3D[current] = server->createOverlay( "_overlay3D"+currentOnText, _scene );
			std::string modelWeapon = entityInfo->getStringAttribute("weapon"+currentOnText+"Model");			
			
			_graphicsEntities[current].graphicsEntity = _overlayWeapon3D[current]->add3D(currentOnText, modelWeapon);
			assert(_graphicsEntities[current].graphicsEntity != 0 && "error al cargar la entidad grafica");
			//_weaponsEntities[current] = _overlayWeapon3D[current]->add3D(currentOnText, modelWeapon, &offsetPositionWeapon);

			_graphicsEntities[current].graphicsEntity->setPosition(_graphicsEntities[current].offset);
			Matrix4 transformModificado = _graphicsEntities[current].graphicsEntity->getTransform();
			Math::pitchYawRoll(_graphicsEntities[current].pitch, _graphicsEntities[current].yaw, _graphicsEntities[current].roll, transformModificado);

			_graphicsEntities[current].graphicsEntity->setTransform(transformModificado);
			

			_overlayWeapon3D[current]->setVisible(false);

			}
		}
		if(!_graphicsEntities)
			return false;
		
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudWeapons::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_TRANSFORM				||
			   msgType == Message::CHANGE_WEAPON_GRAPHICS;
	} // accept
	
	//---------------------------------------------------------

	void CHudWeapons::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON_GRAPHICS: {
				std::shared_ptr<CMessageChangeWeaponGraphics> chgWpnMsg = std::static_pointer_cast<CMessageChangeWeaponGraphics>(message);
				changeWeapon( chgWpnMsg->getWeapon() );
				break;
			}
		}

	} // process
	//---------------------------------------------------------

	void CHudWeapons::changeWeapon(int newWeapon){
		
		_graphicsEntities[_currentWeapon].graphicsEntity->setVisible(false);
		_graphicsEntities[newWeapon].graphicsEntity->setVisible(true);
		_currentWeapon = newWeapon;

	}
	//---------------------------------------------------------

	void CHudWeapons::onTick(unsigned int msecs){
		movement();
	}
	//---------------------------------------------------------
	void CHudWeapons::movement(){
		
	}
	//---------------------------------------------------------

} // namespace Logic

