/**
@file Screamer.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Screamer".
 
@see Logic::CScreamer
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

#include "Screamer.h"
#include "PhysicDynamicEntity.h"
#include "Graphics.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"

namespace Logic {

	IMP_FACTORY(CScreamer);

	//__________________________________________________________________

	CScreamer::CScreamer() : CPlayerClass("screamer"),
							 _primarySkillIsActive(false),
							 _screamerShield(NULL) {
		
		// Nada que hacer
	}

	//__________________________________________________________________

	CScreamer::~CScreamer() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CScreamer::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;
	} // spawn

	//__________________________________________________________________

	void CScreamer::tick(unsigned int msecs) {
		CPlayerClass::tick(msecs);

		// Si la habilidad primaria está siendo usada
		if(_primarySkillIsActive) {
			refreshShieldPosition();
		}
	}

	//__________________________________________________________________

	void CScreamer::primarySkill() {
		std::cout << "Primary Skill - Screamer" << std::endl;

		_primarySkillIsActive = true;

		// Me he visto obligado a realizar esta comprobacion aqui porque de momento no podemos crear entidades dinamicas
		// en el spawn (por la forma de asignar nombres).
		if(_screamerShield == NULL) {
			// Obtenemos la informacion asociada al arquetipo de la granada
			Map::CEntity* screamerShieldInfo = CEntityFactory::getSingletonPtr()->getInfo("ScreamerShield");
			// Creamos la entidad y la activamos
			_screamerShield = CEntityFactory::getSingletonPtr()->createEntity( screamerShieldInfo, Logic::CServer::getSingletonPtr()->getMap() );
			assert(_screamerShield != NULL);
		}

		// Activamos el escudo
		_screamerShield->activate();
		refreshShieldPosition();
	}

	//__________________________________________________________________

	void CScreamer::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Screamer" << std::endl;
	}

	//__________________________________________________________________

	void CScreamer::stopPrimarySkill() {
		_primarySkillIsActive = false;
		
		// Desactivamos los gráficos y la entidad
		CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setVisible(false);
		_screamerShield->deactivate();
	}

	//__________________________________________________________________

	void CScreamer::refreshShieldPosition() {
		// Spawneamos la granada justo delante del jugador y a la altura de disparo que corresponda
		Vector3 entityPosition = _entity->getPosition();
		Vector3 myPosition = entityPosition + ( Math::getDirection( _entity->getOrientation() ) * _capsuleRadius );
		myPosition.y = entityPosition.y + _heightShoot;

		// Posicionamos el centro del escudo justo en el punto de mira
		CPhysicDynamicEntity* physicDynamic = _screamerShield->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");
		assert(physicDynamic && "Error la entidad ScreamerShield no tiene un componente fisico");
		physicDynamic->setPhysicPosition(myPosition, false);

		// Activamos los gráficos del escudo
		CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setVisible(true);
	}

} // namespace Logic

