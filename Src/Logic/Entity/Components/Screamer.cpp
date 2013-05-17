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

// Componentes
#include "Screamer.h"
#include "ScreamerShieldDamageNotifier.h"
#include "PhysicDynamicEntity.h"
#include "Graphics.h"
#include "Life.h"

// Logica
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageActivateScreamerShield.h"
#include "Logic/Messages/MessageDeactivateScreamerShield.h"
#include "Logic/Messages/MessageSetRelatedEntity.h"

// Física
#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"

// Gráficos
#include "Graphics/Server.h"
#include "Graphics/Particle.h"
#include "Graphics/Scene.h"

#include "Graphics/Camera.h"

namespace Logic {

	IMP_FACTORY(CScreamer);

	//__________________________________________________________________

	CScreamer::CScreamer() : CPlayerClass("screamer"),
							 _primarySkillIsActive(false),
							 _screamerShieldDamageTimer(0),
							 _screamerShieldRecoveryTimer(0),
							 _screamerShield(NULL) {


	}

	//__________________________________________________________________

	CScreamer::~CScreamer() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CScreamer::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;

		if(entityInfo->hasAttribute("materialName"))
			_materialName = entityInfo->getStringAttribute("materialName");

		// Nos aseguramos de que todos los atributos necesarios para jugar esten
		// correctamente asignados
		assert( entityInfo->hasAttribute("screamerShieldThreshold") );
		assert( entityInfo->hasAttribute("screamerShieldDamageTimeStep") );
		assert( entityInfo->hasAttribute("screamerShieldDamageOverTime") );
		assert( entityInfo->hasAttribute("screamerShieldRecoveryTimeStep") );
		assert( entityInfo->hasAttribute("screamerShieldRecoveryOverTime") );
		assert( entityInfo->hasAttribute("screamerExplotionDamage") );
		assert( entityInfo->hasAttribute("screamerExplotionRadius") );

		// Asignamos los atributos correspondientes.
		_currentScreamerShield = _screamerShieldThreshold = entityInfo->getFloatAttribute("screamerShieldThreshold");
		_screamerShieldDamageTimeStep = entityInfo->getIntAttribute("screamerShieldDamageTimeStep");
		_screamerShieldDamageOverTime = entityInfo->getFloatAttribute("screamerShieldDamageOverTime");
		_screamerShieldRecoveryTimeStep = entityInfo->getIntAttribute("screamerShieldRecoveryTimeStep");
		_screamerShieldRecoveryOverTime = entityInfo->getFloatAttribute("screamerShieldRecoveryOverTime");
		_screamerExplotionDamage = entityInfo->getFloatAttribute("screamerExplotionDamage");
		_screamerExplotionRadius = entityInfo->getFloatAttribute("screamerExplotionRadius");
		return true;
	} // spawn

	//__________________________________________________________________

	void CScreamer::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		// Si la habilidad primaria está siendo usada
		if(_primarySkillIsActive) {
			// Comprobamos si tenemos que explotar, ya que podrían habernos
			// disparado al escudo
			if(_currentScreamerShield < 0) {
				// El screamer explota
				blowUp();
			}
			else {
				// Reducir la energia si es posible
				if(_currentScreamerShield > 1) {
					// Decrementar iterativamente hasta llegar a 0
					// o no poder decrementar mas
					_screamerShieldDamageTimer += msecs;
					unsigned int steps = _screamerShieldDamageTimer / _screamerShieldDamageTimeStep;
					_screamerShieldDamageTimer = _screamerShieldDamageTimer % _screamerShieldDamageTimeStep;
					
					for(int i = 0; i < steps; ++i) {
						_currentScreamerShield -= _screamerShieldDamageTimeStep;
						if(_currentScreamerShield <= 1) {
							_currentScreamerShield = 1;
							break;
						}
					}
				}
			}
		}
		else if(_currentScreamerShield < _screamerShieldThreshold) {
			// Incremento el timer de recuperación hasta llegar al máximo
			_screamerShieldRecoveryTimer += msecs;
			unsigned int steps = _screamerShieldRecoveryTimer / _screamerShieldRecoveryTimeStep;
			_screamerShieldRecoveryTimer = _screamerShieldRecoveryTimer % _screamerShieldRecoveryTimeStep;

			for(int i = 0; i < steps; ++i) {
				_currentScreamerShield += _screamerShieldDamageTimeStep;
				if(_currentScreamerShield >= _screamerShieldThreshold) {
					_currentScreamerShield = _screamerShieldThreshold;
					break;
				}
			}
		}
	}

	//________________________________________________________________________

	void CScreamer::blowUp() {
		// Elimino la entidad escudo
		if(_screamerShield != NULL) {
			// Destruimos la entidad e indicamos a los clientes que tambien la destruyan
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_screamerShield, true);
			_screamerShield = NULL;
		}

		// Creo la explosión. La propia explosión me mata ya que estoy en el centro
		// y se emite un mensaje de instakill.
		createExplotion();
	}

	//________________________________________________________________________

	void CScreamer::onActivate() {
		CPlayerClass::onActivate();

		// @deprectaed De momento se hace esto en el activate para darle
		// una textura distinta a cada clase.
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName(_materialName);
		_entity->emitMessage(materialMsg);

		_primarySkillIsActive = false;
		_currentScreamerShield = _screamerShieldThreshold;
		_screamerShieldDamageTimer = _screamerShieldRecoveryTimer = 0;
	}

	//__________________________________________________________________

	void CScreamer::primarySkill() {
		_primarySkillIsActive = true;

		// Creamos una entidad ScreamerShield
		// Obtenemos la informacion asociada al arquetipo del escudo del screamer
		Map::CEntity* screamerShieldInfo = CEntityFactory::getSingletonPtr()->getInfo("ScreamerShield");

		// Creamos la entidad y la activamos
		_screamerShield = CEntityFactory::getSingletonPtr()->createEntity( screamerShieldInfo, Logic::CServer::getSingletonPtr()->getMap() );
		assert(_screamerShield != NULL);

		// Fijamos a nuestra entidad como dueña de la entidad creada en el componente
		// que recibe las notificaciones de daño.
		CScreamerShieldDamageNotifier* shieldDmgNotifier = _screamerShield->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier");
		assert(shieldDmgNotifier && "Error: La entidad ScreamerShield no tiene ningun componente llamado CScreamerShieldDamageNotifier");
		shieldDmgNotifier->setOwner(_entity);

		// Activamos la entidad creada
		_screamerShield->activate();
		_screamerShield->start();
	}

	//__________________________________________________________________

	void CScreamer::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Screamer" << std::endl;
	}

	//__________________________________________________________________

	void CScreamer::stopPrimarySkill() {
		_primarySkillIsActive = false;

		//assert(_screamerShield != NULL && "Error: No se ha creado el escudo");
		if(_screamerShield != NULL) {
			// Destruimos la entidad e indicamos a los clientes que tambien la destruyan
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_screamerShield, true);
			_screamerShield = NULL;
		}
	}

	//__________________________________________________________________

	void CScreamer::absorbDamage(float damage) {
		_currentScreamerShield -= damage;
	}

	//__________________________________________________________________

	void CScreamer::createExplotion() {
		// EntitiesHit sera el buffer que contendra la lista de entidades que ha colisionado
		// con el overlap
		std::vector<CEntity*> entitiesHit;

		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_screamerExplotionRadius);
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, _entity->getPosition(), entitiesHit);

		int nbHits = entitiesHit.size();
		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// Si la entidad golpeada es valida y es un player
			if( entitiesHit[i] != NULL && entitiesHit[i]->isPlayer() ) {
				// Emitimos el mensaje de instakill
				// @deprecated Calcular bien el daño
				std::shared_ptr<CMessageDamaged> dmgMsg = std::make_shared<CMessageDamaged>();
				dmgMsg->setDamage(_screamerExplotionDamage);
				dmgMsg->setEnemy(_entity);
				entitiesHit[i]->emitMessage(dmgMsg);
			}
		}

		Graphics::CParticle *particle = Graphics::CServer::getSingletonPtr()->
			getActiveScene()->createParticle(_entity->getName(),"ExplosionParticle", _entity->getPosition());
	}

} // namespace Logic

