/**
@file ScreamerClient.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Screamer" (Cliente).
 
@see Logic::CScreamerClient
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

// Componentes
#include "ScreamerClient.h"
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

// Física
#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"

// Gráficos
#include "Graphics/Server.h"
#include "Graphics/Particle.h"
#include "Graphics/Scene.h"

#include "Graphics/Camera.h"

namespace Logic {

	IMP_FACTORY(CScreamerClient);

	//__________________________________________________________________

	CScreamerClient::CScreamerClient() : CPlayerClass("screamer"),
										_shieldIsActive(false) {

		// Creamos la entidad escudo que en el cliente solo será un gráfico
											
		// Obtenemos la informacion asociada al arquetipo de la granada
		Map::CEntity* screamerShieldInfo = CEntityFactory::getSingletonPtr()->getInfo("ScreamerShield"); // deberia ser screamerShieldClient
		// Creamos la entidad y la activamos
		_screamerShield = CEntityFactory::getSingletonPtr()->createEntity( screamerShieldInfo, Logic::CServer::getSingletonPtr()->getMap() );
		assert(_screamerShield != NULL);

		// @deprecated El servidor no va a tener graficos
		CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setVisible(false);
		_screamerShield->deactivate();
	}

	//__________________________________________________________________

	CScreamerClient::~CScreamerClient() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CScreamerClient::accept(CMessage* message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ACTIVATE_SCREAMER_SHIELD || 
			   msgType == Message::DEACTIVATE_SCREAMER_SHIELD;
	}

	//__________________________________________________________________

	void CScreamerClient::process(CMessage* message) {
		switch( message->getMessageType() ) {
			case Message::ACTIVATE_SCREAMER_SHIELD: {
				activateScreamerShield();
			}
			case Message::DEACTIVATE_SCREAMER_SHIELD: {
				deactivateScreamerShield();
			}
		}
	}

	//__________________________________________________________________

	void CScreamerClient::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Si la habilidad primaria está siendo usada
		if(_shieldIsActive) {
			refreshShieldGraphicsPos();
		}
	}

	//__________________________________________________________________

	void CScreamerClient::primarySkill() {
		// En el cliente este metodo no importa
		// Ya que de esto se encarga el servidor.
	}

	//__________________________________________________________________

	void CScreamerClient::secondarySkill() {
		// En el cliente este metodo no importa
		// Ya que de esto se encarga el servidor.
	}

	//__________________________________________________________________

	void CScreamerClient::refreshShieldGraphicsPos() {
		// Sacamos la posicion del escudo (que debe estar situada a la altura de disparo)
		Vector3 shootPosition = _entity->getPosition() + ( Math::getDirection( _entity->getOrientation() ) * _capsuleRadius*10 );
		shootPosition.y += _heightShoot;
		
		// Sacamos el transform de la entidad y luego le seteamos la posicion de disparo
		Matrix4 shootTransform = _entity->getTransform();
		shootTransform.setTrans(shootPosition);

		// Posicionamos el centro del escudo justo en el punto de mira		
		CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setTransform(shootTransform);
	}

	//__________________________________________________________________

	void CScreamerClient::activateScreamerShield() {
		std::cout << "Activo escudo" << std::endl;

		_shieldIsActive = true;

		// Activamos el escudo
		_screamerShield->activate();

		// Activamos los gráficos del escudo
		CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setVisible(true);
	}

	//__________________________________________________________________

	void CScreamerClient::deactivateScreamerShield() {
		std::cout << "Desactivo escudo" << std::endl;

		_shieldIsActive = false;

		CGraphics* shieldGraphics = _screamerShield->getComponent<CGraphics>("CGraphics");
		assert(shieldGraphics && "Error: La entidad ScreamerShield no tiene un componente CGraphics");
		shieldGraphics->setVisible(false);
		_screamerShield->deactivate();
	}

} // namespace Logic

