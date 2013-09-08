/**
@file Shadow.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Shadow".
 
@see Logic::CShadow
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

#include "Shadow.h"
#include "Graphics.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/WorldState.h"
#include "Logic/Entity/Entity.h"
#include "Physics/Server.h"
#include "Physics/RaycastHit.h"

#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageChangeMaterialHudWeapon.h"
#include "Logic/Messages/MessageFlash.h"

#include <assert.h>



namespace Logic {

	IMP_FACTORY(CShadow);

	//__________________________________________________________________

	CShadow::CShadow() : CPlayerClass("shadow"), _playingSound(false), _playingShadowOut(false) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CShadow::~CShadow() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CShadow::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;

		_invisibilityTimer = 0;

		if(entityInfo->hasAttribute("materialName"))
			_materialName = entityInfo->getStringAttribute("materialName");

		// Leer el tiempo que dura la invisibilidad
		assert( entityInfo->hasAttribute("invisibilityDuration") && "no tienes invisibilityduration mendrugo" );
		// Pasamos el tiempo a msecs
		_invisibilityDuration = entityInfo->getFloatAttribute("invisibilityDuration") * 1000;

		_flashRadius= 150;

		return true;
	} // spawn

	//__________________________________________________________________

	void CShadow::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		// Si la habilidad primaria esta en uso, controlar el tiempo
		// efectivo de la invisibilidad. Cuando se cumpla el tiempo,
		// deshabilitamos el shader de transparencia.
		if(_doingPrimarySkill){
			if(_invisibilityTimer > 0) {
				_invisibilityTimer -= msecs;
				if(_invisibilityTimer < 1600 && !_playingShadowOut) {
 					emitSound("character/shadowOut.ogg", false, false, false, false, true);
					_playingShadowOut = true;
				}
			}else{
				// Desactivamos el "shader" de invisibilidad (shader por llamarle algo)
				std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
				materialMsg->setMaterialName("original");
				_entity->emitMessage(materialMsg);
				Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);
				/*
				std::shared_ptr<CMessageChangeMaterialHudWeapon> materialMsgHud = std::make_shared<CMessageChangeMaterialHudWeapon>();
				materialMsgHud->setMaterialName("original");
				_entity->emitMessage(materialMsgHud);
				*/
				_invisibilityTimer = _invisibilityDuration;
				_doingPrimarySkill = false;
				
				// Dejamos de reproducir el sonido chunger
				emitSound("character/shadow.ogg", false, false, false, true, true);
				_playingSound = false;
				_playingShadowOut = false;
			}
		}
	}

	//________________________________________________________________________

	void CShadow::onActivate() {
		CPlayerClass::onActivate();
	}

	//________________________________________________________________________

	void CShadow::onDeactivate() {
		if(_playingSound) {
			emitSound("character/shadow.ogg", false, false, false, true, true);
			_playingSound = _playingShadowOut = false;
		}
	}

	//__________________________________________________________________

	void CShadow::primarySkill() {

		// Arrancamos el cronometro
		_invisibilityTimer = _invisibilityDuration;
		
		// Activamos el shader de invisibilidad
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName("shadowInvisibility");
		_entity->emitMessage(materialMsg);
		Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);
		_doingPrimarySkill = true;

		// Creamos el sonido de acojonamiento indicando que
		// el shadow esta entre las sombras malotas
		emitSound("character/shadow.ogg", true, false, false, false, true);
		emitSound("character/shadowIn.ogg", false, false, false, false, true);
		_playingSound = true;
	}

	//__________________________________________________________________

	void CShadow::secondarySkill() {
		// Habilidad por definir
		std::vector<CEntity*> entitiesHit;
		

		//primer filtro
		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_flashRadius);
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, _entity->getPosition(), entitiesHit, Physics::CollisionGroup::ePLAYER);
		int nbHits = entitiesHit.size();
		if(nbHits==0){
			return;
		}

		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// una vez tenemos las entidades en el radio de ceguera, hacemos el segundo filtro
			//que es coger solo a los players

			if(entitiesHit[i] != NULL && entitiesHit[i] != _entity ) 
			{//begin if
				flashEntity(entitiesHit[i]);
			}//end if
		}//end for

		emitSound("character/shadowFlash.ogg", false, true, false, false, false);
	}


	void CShadow::flashEntity(CEntity* entity){

		if(entity == _entity)
			return;

		//primero trazamos un raycast para filtrar si hay algo entre la entidad y yo
		Vector3 direction = _entity->getPosition() - entity->getPosition();
		Ogre::Ray ray( entity->getPosition()+Vector3(0,8,0), direction.normalisedCopy() );
		
		std::vector<Physics::CRaycastHit> hitBuffer;

		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, direction.length(), hitBuffer);
		int bufferSize = hitBuffer.size();

		//ifs de eficiencia
		if(hitBuffer.size()!=2){
			return;
		}

		//Ahora comprobamos el angulo entre la visión directa y la orientación del player a cegar
		float angle = direction.normalisedCopy().angleBetween((entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z).normalisedCopy()).valueDegrees();

		//si no lo esta mirando nada no lo cegamos
		if(angle > 90)
			return;


		//std::cout << "angulo " << angle << std::endl;

		float flashFactor;

		if(angle>1.0)
			flashFactor = 50 * (90/angle);
		else
			flashFactor = 50;


		//mandamos un mensaje de flashazo
		std::shared_ptr<CMessageFlash> flashMsg = std::make_shared<CMessageFlash>();
		flashMsg->setFlashFactor(flashFactor);
		entity->emitMessage(flashMsg);
	}


} // namespace Logic

