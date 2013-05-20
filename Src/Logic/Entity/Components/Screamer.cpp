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
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"

// Mensajes
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageCreateParticle.h"
#include "AvatarController.h"

// Física
#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"

// Gráficos
#include "Graphics/Server.h"
#include "Graphics/Particle.h"
#include "Graphics/Scene.h"

#include "Graphics/Camera.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CScreamer);

	//__________________________________________________________________

	CScreamer::CScreamer() : CPlayerClass("screamer"),
							 _secondarySkillIsActive(false),
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
		assert( entityInfo->hasAttribute("screamerScreamForce") );
		

		// Asignamos los atributos correspondientes.
		_currentScreamerShield = _screamerShieldThreshold = entityInfo->getFloatAttribute("screamerShieldThreshold");
		_screamerShieldDamageTimeStep = entityInfo->getIntAttribute("screamerShieldDamageTimeStep");
		_screamerShieldDamageOverTime = entityInfo->getFloatAttribute("screamerShieldDamageOverTime");
		_screamerShieldRecoveryTimeStep = entityInfo->getIntAttribute("screamerShieldRecoveryTimeStep");
		_screamerShieldRecoveryOverTime = entityInfo->getFloatAttribute("screamerShieldRecoveryOverTime");
		_screamerExplotionDamage = entityInfo->getFloatAttribute("screamerExplotionDamage");
		_screamerExplotionRadius = entityInfo->getFloatAttribute("screamerExplotionRadius");

		_screamerScreamForce = entityInfo->getFloatAttribute("screamerScreamForce");
		_screamerReboundForce= entityInfo->getFloatAttribute("screamerReboundForce");
		
		_rebound = 0;
		_maxNumberRebounds = 1;

		return true;
	} // spawn

	//__________________________________________________________________

	void CScreamer::checkSecondarySkill(unsigned int msecs) {
		// Si la habilidad primaria está siendo usada
		if(_secondarySkillIsActive) {
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

		_secondarySkillIsActive = false;
		_currentScreamerShield = _screamerShieldThreshold;
		_screamerShieldDamageTimer = _screamerShieldRecoveryTimer = 0;
	}
	//__________________________________________________________________

	void CScreamer::primarySkill() {
		// Habilidad por definir
		
		// 3.5 mirao a ojo a traves del visual debbuger 
		Physics::SphereGeometry sphere  = Physics::CGeometryFactory::getSingletonPtr()->createSphere(3.5);

		// Si no he hecho ningun rebote, he de coger la del player, si estoy en algun rebote, el rebote se encarga de decirme en q posicion.
		if(_rebound == 0)
			_directionShoot = Math::getDirection(_entity->getOrientation());

		std::vector<Physics::CSweepHit> hitSpots;

		Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),_directionShoot,999,hitSpots);

		hitConsequences(hitSpots);		

		//std::cout << std::endl << "Primary Skill - Screamer" << std::endl;
	} // primarySkill
	//__________________________________________________________________

	void CScreamer::stopPrimarySkill() {
		
	}

	void CScreamer::hitConsequences(std::vector<Physics::CSweepHit> &hits){
		
		for(auto it = hits.begin(); it < hits.end(); ++it){

			std::string typeEntity = (*it).entity->getType().c_str();
			printf("\nImpacto con: %s a distancia: %f \n\tEn punto: %f %f %f \n", (*it).entity->getName().c_str(), (*it).distance,  (*it).impact.x, (*it).impact.y, (*it).impact.z);
			if(typeEntity == "World" )
			{
				if ((*it).distance < 10){
					printf("\n effecto martillo");
					_entity->getComponent<CAvatarController>("CAvatarController")->addForce(-_directionShoot * _screamerReboundForce);
				}else{
					if(_rebound <= _maxNumberRebounds){
						++_rebound;
						_directionShoot = _directionShoot.reflect(-(*it).normal);
						primarySkill();
					}else{
						_rebound = 0;
					}
				}
			}
			if(typeEntity == "Screamer" || typeEntity == "Hound" || typeEntity == "Archangel" || typeEntity == "Shadow" || typeEntity == "RemotePlayer"){
				printf("\n he dao a un player");
				(*it).entity->getComponent<CAvatarController>("CAvatarController")->addForce(-(*it).normal * _screamerScreamForce);
			}
		}
	}

	void CScreamer::secondarySkill() {
		_secondarySkillIsActive = true;

		// Creamos una entidad ScreamerShield
		// Obtenemos la informacion asociada al arquetipo del escudo del screamer
		Matrix4 playerTransform = _entity->getTransform();
		Vector3 shootPosition = playerTransform.getTrans() + ( Math::getDirection( _entity->getOrientation() ) * _capsuleRadius );
		shootPosition.y += _heightShoot;

		// Creamos la entidad y la activamos
		_screamerShield = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation( 
			CEntityFactory::getSingletonPtr()->getInfo("ScreamerShield"),
			Logic::CServer::getSingletonPtr()->getMap(),
			shootPosition,
			Math::getYaw(playerTransform),
			Math::getPitch(playerTransform)
		);
		
		// Fijamos a nuestra entidad como dueña de la entidad creada en el componente
		// que recibe las notificaciones de daño.
		CScreamerShieldDamageNotifier* shieldDmgNotifier = _screamerShield->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier");
		assert(shieldDmgNotifier && "Error: La entidad ScreamerShield no tiene ningun componente llamado CScreamerShieldDamageNotifier");
		shieldDmgNotifier->setOwner(_entity);
		shieldDmgNotifier->setOffset(_capsuleRadius, _heightShoot);

		// Activamos la entidad creada
		_screamerShield->activate();
		_screamerShield->start();
	} // secondarySkill
	//__________________________________________________________________

	void CScreamer::stopSecondarySkill() {
		_secondarySkillIsActive = false;

		if(_screamerShield != NULL) {
			// Destruimos la entidad e indicamos a los clientes que tambien la destruyan
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_screamerShield, true);
			_screamerShield = NULL;
		}
	} // stopSecondarySkill
	//__________________________________________________________________

	void CScreamer::absorbDamage(float damage) {
		_currentScreamerShield -= damage;
	} // absorbDamage
	//__________________________________________________________________

	void CScreamer::createExplotion() {
		// EntitiesHit sera el buffer que contendra la lista de entidades que ha colisionado
		// con el overlap
		vector<CEntity*> entitiesHit;

		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_screamerExplotionRadius);
		Vector3 explotionPos = _entity->getPosition();
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, explotionPos, entitiesHit);

		int nbHits = entitiesHit.size();
		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// Si la entidad golpeada es valida y es un player
			if( entitiesHit[i] != NULL && entitiesHit[i]->isPlayer() ) {
				// Emitimos el mensaje de instakill
				// @todo mandar un mensaje de instakill en vez de un mensaje de daño
				shared_ptr<CMessageDamaged> dmgMsg = make_shared<CMessageDamaged>();
				dmgMsg->setDamage(_screamerExplotionDamage);
				dmgMsg->setEnemy(_entity);
				entitiesHit[i]->emitMessage(dmgMsg);
			}
		}

		shared_ptr<CMessageCreateParticle> particleMsg = make_shared<CMessageCreateParticle>();
        particleMsg->setParticle("explotionParticles");
        particleMsg->setPosition(explotionPos);
        _entity->emitMessage(particleMsg);
	} // createExplotion
	//________________________________________________________________________

	void CScreamer::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		checkSecondarySkill(msecs);
	} // onTick
	//________________________________________________________________________

} // namespace Logic

