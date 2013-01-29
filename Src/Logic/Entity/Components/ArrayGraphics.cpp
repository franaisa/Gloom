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

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

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

	bool CArrayGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();

		if(entityInfo->hasAttribute("numWeapons")){
			int numWeapons = entityInfo->getIntAttribute("numWeapons");
			
			_graphicsEntities = new TGraphicsWeapon[numWeapons];

			for(int i = 0; i < numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << i+1;
				std::string weapon = aux.str();
				
				_graphicsEntities[i]._graphicsEntity = createGraphicsEntity(weapon, entityInfo->getStringAttribute(weapon+"Model"));
				
				_graphicsEntities[i].yaw = entityInfo->getIntAttribute(weapon+"ModelYaw");
				_graphicsEntities[i].pitch = entityInfo->getIntAttribute(weapon+"ModelPitch");
				
				//Esto perta y creo q es necesario.
				//_graphicsEntities[i].offset = new Vector3(entityInfo->getVector3Attribute(weapon+"ModelOffset"));
				
				if(i!=0) _graphicsEntities[i]._graphicsEntity->setVisible(false);
			}
		}
		if(!_graphicsEntities)
			return false;

		setTransform(&_entity->getTransform());

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CArrayGraphics::createGraphicsEntity(std::string nombreEntidad, std::string modelo)
	{
		Graphics::CEntity *_ge = new Graphics::CEntity(nombreEntidad,modelo);
		if(!_scene->addEntity(_ge))
			return 0;
		

				
		
		return _ge;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	bool CArrayGraphics::accept(CMessage *message)
	{
		return (message->getMessageType() == Message::SET_TRANSFORM) || 
			(message->getMessageType() == Logic::Message::CONTROL);
	} // accept
	
	//---------------------------------------------------------

	void CArrayGraphics::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::SET_TRANSFORM: setTransform( &((CMessageTransform*)message)->getTransform() );
			break;
		case Message::CONTROL: 
			//if(message->getMessageType() == Control::CHANGE_WEAPON)
				//changeWeapon((CMessageChangeWeapon*)message);
			break;
		}

	} // process

	void CArrayGraphics::changeWeapon(CMessageChangeWeapon *msg){

		_graphicsEntities[msg->getWeapon()]._graphicsEntity->setVisible(true);
		_graphicsEntities[_actualWeapon]._graphicsEntity->setVisible(false);
		
		
		setTransform(&_entity->getTransform());
	}

	void CArrayGraphics::setTransform(const Matrix4 *transform){
		
		Matrix4 transformModificado = *transform;
		
		Math::yaw(_graphicsEntities[_actualWeapon].yaw, transformModificado);
		Vector3 vector;
		transformModificado.getTrans().swap(vector);
		transformModificado.setTrans (vector + (Vector3(0.0,2.0,0.0)));
		
		_graphicsEntities[_actualWeapon]._graphicsEntity->setTransform(transformModificado);
	}

} // namespace Logic

