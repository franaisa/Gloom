/**
@file Archangel.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Archangel".
 
@see Logic::CArchangel
@see Logic::IComponent

@author Francisco Aisa García.
@author Jaime Chapinal Cervantes
@date Marzo, 2013f
*/

#include "Archangel.h"
#include "Graphics.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/WorldState.h"

#include "Logic/Messages/MessageSetReducedDamage.h"
#include "Logic/Messages/MessageChangeMaterial.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"

#include <assert.h>

namespace Logic {

	IMP_FACTORY(CArchangel);

	//__________________________________________________________________

	CArchangel::CArchangel() : CPlayerClass("archangel") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CArchangel::~CArchangel() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CArchangel::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) 
			return false;

		// Leer atributos propios
		_inmunityTimer = 0;

		if(entityInfo->hasAttribute("materialName"))
			_materialName = entityInfo->getStringAttribute("materialName");

		// Leer el tiempo que dura la invisibilidad
		assert( entityInfo->hasAttribute("inmunityDuration") );
		// Pasamos el tiempo a msecs
		_inmunityDuration = entityInfo->getFloatAttribute("inmunityDuration") * 1000;
		
		return true; //Para quitar el warning?
	} // spawn

	//__________________________________________________________________

	void CArchangel::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		// Si la habilidad primaria esta en uso, controlar el tiempo
		// efectivo siendo inmune. Cuando se cumpla el tiempo,
		// deshabilitamos la reducción de daño
		if(_doingPrimarySkill){
			if(_inmunityTimer > 0) {
				_inmunityTimer -= msecs;
			}else{
				// Seteamos la reducción de daño a 0 de manera que recibimos los daños normales
 				std::shared_ptr<CMessageSetReducedDamage> pReducedDmgMsg = std::make_shared<CMessageSetReducedDamage>();
				pReducedDmgMsg->setReducedDamage(0);
				_entity->emitMessage(pReducedDmgMsg);
					
	
				// Desactivamos el shader de inmunidad
				// Desactivamos el shader de invisibilidad
				std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
				materialMsg->setMaterialName("original");
				_entity->emitMessage(materialMsg);
				Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);
				_doingPrimarySkill = false;
			}
		}
	}

	//________________________________________________________________________

	void CArchangel::onActivate() {
		CPlayerClass::onActivate();

		_inmunityTimer = _inmunityDuration;
	}

	//__________________________________________________________________

	void CArchangel::primarySkill() {

		// Arrancamos el cronometro
		_inmunityTimer = _inmunityDuration;

		// Seteamos la reducción de daño al máximo de manera que no se reciban daños
		std::shared_ptr<CMessageSetReducedDamage> pReducedDmgMsg = std::make_shared<CMessageSetReducedDamage>();
		pReducedDmgMsg->setReducedDamage(1);
		_entity->emitMessage(pReducedDmgMsg);

		// Activamos el shader de inmunidad
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName("ArchangelDiamond3"); // En el futuro debe ser el material del archangel
		_entity->emitMessage(materialMsg);
		Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);

		_doingPrimarySkill = true;
	}

	//__________________________________________________________________

	void CArchangel::secondarySkill() {
		Vector3 position = _entity->getPosition();
		position.y += _heightShoot;

		Matrix4 transform = Matrix4::IDENTITY;
		transform.setTrans(position);

		CEntity *projectileEntity= CEntityFactory::getSingletonPtr()->createEntity(
			//CEntityFactory::getSingletonPtr()->getInfo("MagneticBullet"),
			CEntityFactory::getSingletonPtr()->getInfo("LifeDome"),			
			Logic::CServer::getSingletonPtr()->getMap(),
			transform
		);
	}

	//__________________________________________________________________


} // namespace Logic

