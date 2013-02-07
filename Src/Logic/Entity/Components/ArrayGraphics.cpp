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
				_scene->removeEntity(_graphicsEntities[i]._graphicsEntity);
				delete _graphicsEntities[i]._graphicsEntity;
				_graphicsEntities[i]._graphicsEntity = NULL;
			}
			delete _graphicsEntities;
		}

	} // ~CGraphics
	//---------------------------------------------------------
	
	void CArrayGraphics::activate()
	{
		IComponent::activate();

		_graphicsEntities[_actualWeapon]._graphicsEntity->setVisible(true);

	} // activate
	//---------------------------------------------------------

	void CArrayGraphics::deactivate()
	{
		IComponent::deactivate();

		_graphicsEntities[_actualWeapon]._graphicsEntity->setVisible(false);

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

			std::string armas[] = {"Hammer","MiniGun","ShotGun"};

			
			for(int i = 0; i < numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << armas[i];
				std::string weapon = aux.str();
				
				printf("%s", weapon.c_str());
				
				_graphicsEntities[i]._graphicsEntity = createGraphicsEntity(weapon, entityInfo->getStringAttribute(weapon+"Model"));
				
				_graphicsEntities[i].yaw = entityInfo->getFloatAttribute(weapon+"ModelYaw");
				_graphicsEntities[i].pitch = entityInfo->getFloatAttribute(weapon+"ModelPitch");
				
				//Esto perta y creo q es necesario.
				_graphicsEntities[i].offset = new Vector3(entityInfo->getVector3Attribute(weapon+"Offset"));
				
				if(i!=0) _graphicsEntities[i]._graphicsEntity->setVisible(false);
				
			}
		}
		if(!_graphicsEntities)
			return false;

		setTransform(_entity->getTransform());

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

	bool CArrayGraphics::accept(CMessage *message)
	{
		return (message->getMessageType() == Message::SET_TRANSFORM)
			|| (message->getMessageType() == Message::CHANGE_WEAPON_GRAPHICS);
	} // accept
	
	//---------------------------------------------------------

	void CArrayGraphics::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::SET_TRANSFORM:
			setTransform(((CMessageTransform*)message)->getTransform());
			break;
		case Message::CHANGE_WEAPON_GRAPHICS:
			changeWeapon( ((CMessageChangeWeaponGraphics*)message)->getWeapon() );
			break;
		}

	} // process

	void CArrayGraphics::changeWeapon(int newWeapon){
		
		_graphicsEntities[newWeapon]._graphicsEntity->setVisible(true);
		_graphicsEntities[_actualWeapon]._graphicsEntity->setVisible(false);
		_actualWeapon = newWeapon;
		
		
		setTransform(_entity->getTransform());
		
	}

	void CArrayGraphics::setTransform(const Matrix4 &transform){
		
		//en un futuro simplemente se orientara la entidad en el 3ds o similar. 

		// Obtengo la camara para posicionarla en esta posicion pero algo modificada
		Graphics::CCamera* camera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
		Vector3 direction = camera->getTargetCameraPosition() - camera->getCameraPosition();
		
		direction.normalise();
		Vector3 posicionModificada = camera->getCameraPosition()- Vector3(0,2.5,0) + ((8.0f) * direction);
		
		//Aqui establezco la rotacion (En un futuro se rotara el modelo)
		Matrix4 transformModificado = transform;
		Math::yaw(_graphicsEntities[_actualWeapon].yaw, transformModificado);
		Math::pitch(_graphicsEntities[_actualWeapon].pitch, transformModificado);
		_graphicsEntities[_actualWeapon]._graphicsEntity->setTransform(transformModificado);
		_graphicsEntities[_actualWeapon]._graphicsEntity->setPosition(posicionModificada);


	}

} // namespace Logic

