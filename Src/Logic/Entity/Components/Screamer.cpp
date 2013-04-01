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
#include "ScreamerShieldDamageNotifier.h"
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
							 _screamerShieldDamageTimer(0),
							 _screamerShieldRecoveryTimer(0),
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

		assert( entityInfo->hasAttribute("screamerShieldThreshold") );
		assert( entityInfo->hasAttribute("screamerShieldDamageTimeStep") );
		assert( entityInfo->hasAttribute("screamerShieldDamageOverTime") );
		assert( entityInfo->hasAttribute("screamerShieldRecoveryTimeStep") );
		assert( entityInfo->hasAttribute("screamerShieldRecoveryOverTime") );

		_currentScreamerShield = _screamerShieldThreshold = entityInfo->getFloatAttribute("screamerShieldThreshold");
		_screamerShieldDamageTimeStep = entityInfo->getFloatAttribute("screamerShieldDamageTimeStep");
		_screamerShieldDamageOverTime = entityInfo->getFloatAttribute("screamerShieldDamageOverTime");
		_screamerShieldRecoveryTimeStep = entityInfo->getFloatAttribute("screamerShieldRecoveryTimeStep");
		_screamerShieldRecoveryOverTime = entityInfo->getFloatAttribute("screamerShieldRecoveryOverTime");
	} // spawn

	//__________________________________________________________________

	void CScreamer::tick(unsigned int msecs) {
		CPlayerClass::tick(msecs);

		// Si la habilidad primaria está siendo usada
		if(_primarySkillIsActive) {
			// Deberia funcionar este bucle como el de la fisica? restando
			// daños hasta cumplir los ciclos acumulados?
			if(_screamerShieldDamageTimer < _screamerShieldDamageTimeStep) {
				_screamerShieldDamageTimer += msecs;
			}
			else {
				_currentScreamerShield -= _screamerShieldDamageOverTime;
				_screamerShieldDamageTimer = 0;

				if(_currentScreamerShield > 0) {
					// Colocamos el escudo delante nuestra
					//refreshShieldPosition();
				}
				else {
					// El screamer explota
					std::cout << "Exploto" << std::endl;

					// Destruir o desactivar el escudo
				}
			}
		}
		else if(_currentScreamerShield != _screamerShieldThreshold) {
			if(_screamerShieldRecoveryTimer < _screamerShieldRecoveryTimeStep) {
				_screamerShieldRecoveryTimer += msecs;
			}
			else {
				_currentScreamerShield += _screamerShieldRecoveryOverTime;
				_screamerShieldRecoveryTimer = 0;

				if(_currentScreamerShield > _screamerShieldThreshold) {
					_currentScreamerShield = _screamerShieldThreshold;
				}
			}
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

			// Fijamos a nuestra entidad como dueña de la entidad creada en el componente
			// que recibe las notificaciones de daño.
			CScreamerShieldDamageNotifier* shieldDmgNotifier = _screamerShield->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier");
			assert(shieldDmgNotifier && "Error: La entidad ScreamerShield no tiene ningun componente llamado CScreamerShieldDamageNotifier");
			shieldDmgNotifier->setOwner(_entity);
		}

		// Activamos el escudo
		_screamerShield->activate();

		// deprecated
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
		/*CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setVisible(false);
		_screamerShield->deactivate();*/
	}

	//__________________________________________________________________

	void CScreamer::absorbDamage(float damage) {
		_currentScreamerShield -= damage;

		if(_currentScreamerShield <= 0) {
			std::cout << "Exploto" << std::endl;
		}
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

