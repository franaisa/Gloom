//---------------------------------------------------------------------------
// GameNetMsgManager.cpp
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.cpp

Contiene la implementación del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author Rubén Mulero
@author Francisco Aisa
@date Febrero, 2011
*/

#include "GameNetMsgManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/PhysicDynamicEntity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Net/paquete.h"
#include "Net/buffer.h"
#include "Entity\MessageFactory.h"
#include "Logic/Messages/Message.h"
#include "Input/Server.h"
#include "Input/PlayerController.h"
#include "Logic/Entity/Components/Interpolation.h"
#include "Map/MapEntity.h"

#include "Logic/GameNetPlayersManager.h"
#include "Logic/PlayerInfo.h"

#include "Application/BaseApplication.h"

#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) cout << "LOGIC::NET_MANAGER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	CGameNetMsgManager* CGameNetMsgManager::_instance = 0;

	//--------------------------------------------------------

	CGameNetMsgManager::CGameNetMsgManager()
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CGameNetMsgManager::~CGameNetMsgManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CGameNetMsgManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CGameNetMsgManager no permitida!");

		new CGameNetMsgManager();

		return true;

	} // Init

	//--------------------------------------------------------

	void CGameNetMsgManager::Release()
	{
		assert(_instance && "Logic::CGameNetMsgManager no está inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	//--------------------------------------------------------

	void CGameNetMsgManager::activate() 
	{
		// TODO Escuchamos los mensajes de red. Engancharnos a Net::CManager
		Net::CManager::getSingletonPtr()->addObserver(this);

	} // activate

	//--------------------------------------------------------

	void CGameNetMsgManager::deactivate() 
	{	
		// TODO Dejamos de escuchar los mensajes de red. 
		// Desengancharnos de Net::CManager
		Net::CManager::getSingletonPtr()->removeObserver(this);
	} // deactivate

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendDestroyEntity(TEntityID destID){
		//cogemos la entidad que hemos creado para enviar la información por la red
		CEntity * destEntity = CServer::getSingletonPtr()->getMap()->getEntityByID(destID);

		Net::NetMessageType msgType = Net::DESTROY_ENTITY;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
		//serializamos toda la información que se necesita para la creación de la entidad
		serialMsg.write(&msgType, sizeof(msgType));
		serialMsg.serialize(destID);

		//enviamos el mensaje
		Net::CManager::getSingletonPtr()->broadcast(serialMsg.getbuffer(), serialMsg.getSize());
	}

	//---------------------------------------------------------

	void CGameNetMsgManager::processDestroyEntity(Net::CPaquete* packet){
		Net::CBuffer serialMsg;
			serialMsg.write(packet->getData(),packet->getDataLength());
			serialMsg.reset(); // Reiniciamos el puntero de lectura a la posición 0

		//deserializamos toda la información que se necesita para la creación de la entidad
		Net::NetMessageType msgType;
			serialMsg.read(&msgType,sizeof(msgType));
		
		//cargamos todos los datos que se nos envían por mensaje
		TEntityID destID; 
			serialMsg.read(&destID, sizeof(destID));

		//le decimos al mapa que elimine la entidad si no fue ya eliminada
		CEntity * entity = CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
		//Comprobamos por si acaso para hacerlo bien
		if(entity!=NULL)
			CEntityFactory::getSingletonPtr()->deleteEntity(entity);
	}

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendCreateEntity(TEntityID destID, Map::CEntity* customInfoForClient) {
		//cogemos la entidad que hemos creado para enviar la información por la red
		CEntity* destEntity = CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
		Net::NetMessageType msgType = customInfoForClient == NULL ? Net::CREATE_ENTITY : Net::CREATE_CUSTOM_ENTITY;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
		//serializamos toda la información que se necesita para la creación de la entidad
		serialMsg.write(&msgType, sizeof(msgType));
		serialMsg.serialize(destID);
		serialMsg.serialize(destEntity->getType(), false);
		serialMsg.serialize(destEntity->getName(), false);
		serialMsg.serialize(destEntity->getPosition());
		serialMsg.serialize(destEntity->getOrientation());

		if(customInfoForClient != NULL) {
			// Serializamos la información de la entidad
			serialMsg.serialize(customInfoForClient);
		}

		//enviamos el mensaje
		Net::CManager::getSingletonPtr()->broadcast(serialMsg.getbuffer(), serialMsg.getSize());
	}

	//---------------------------------------------------------
		
	void CGameNetMsgManager::processCreateEntity(Net::CPaquete* packet){

		Net::CBuffer serialMsg;
			serialMsg.write(packet->getData(),packet->getDataLength());
			serialMsg.reset(); // Reiniciamos el puntero de lectura a la posición 0

		//deserializamos toda la información que se necesita para la creación de la entidad
		Net::NetMessageType msgType;
			serialMsg.read(&msgType,sizeof(msgType));
		
		//cargamos todos los datos que se nos envían por mensaje
		TEntityID destID; 
			serialMsg.read(&destID, sizeof(destID));
		
		//Si el ID no lo tenemos ya en nuestro mapa proseguimos con la creacion
		if(CServer::getSingletonPtr()->getMap()->getEntityByID(destID)!=0){//si lo encontramos terminamos
			return;
		}
		std::string type;
		serialMsg.deserialize(type);

		std::string name;
		serialMsg.deserialize(name);

		Vector3 position;
		serialMsg.deserialize(position);

		Quaternion orientation;
		serialMsg.deserialize(orientation);

		//creamos la entidad con nombre..
		Map::CEntity * info = Logic::CEntityFactory::getSingletonPtr()->getInfo(type);
		info->setName(name);

		if(msgType == Net::CREATE_CUSTOM_ENTITY) {
			// Deserializamos la información de los atributos customizados
			// para el cliente
			unsigned int nbCustomAttributes;
			serialMsg.deserialize(nbCustomAttributes);

			std::string attributeName, value;
			for(int i = 0; i < nbCustomAttributes; ++i) {
				serialMsg.deserialize(attributeName);
				serialMsg.deserialize(value);

				info->setAttribute(attributeName, value);
			}
		}

		// Creamos la entidad
		CEntity* newEntity = Logic::CEntityFactory::getSingletonPtr()->createEntityById(info, CServer::getSingletonPtr()->getMap(), destID, position, orientation);

		// La inicializamos
		newEntity->activate();
		newEntity->start();
	}


	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendEntityMessage(const std::shared_ptr<CMessage>& txMsg, TEntityID destID) {

		Net::CBuffer bufferAux = txMsg->serialize();

		Net::NetMessageType msgType = Net::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			serialMsg.write(bufferAux.getbuffer(), bufferAux.getSize()); //Guardamos el mensaje en el buffer
			
		Net::CManager::getSingletonPtr()->broadcast(serialMsg.getbuffer(), serialMsg.getSize());
		//std::cout << "Enviado mensaje tipo " << txMsg->getMessageType() << " para la entidad " << destID << " de tamaño " << serialMsg.getSize() << std::endl;
		//LOG("TX ENTITY_MSG " << txMsg._type << " to EntityID " << destID);
	} // sendEntityMessage

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendMessageToOne(const std::shared_ptr<CMessage>& txMsg, TEntityID destID, TEntityID player)
	{

		Net::CBuffer bufferAux = txMsg->serialize();

		Net::NetMessageType msgType = Net::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			serialMsg.write(bufferAux.getbuffer(), bufferAux.getSize()); //Guardamos el mensaje en el buffer
			
		Net::NetID idMsg = Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayerByEntityId(player).getNetId();

		Net::CManager::getSingletonPtr()->sendTo(idMsg, serialMsg.getbuffer(), serialMsg.getSize());
	} // sendMessageToOne

	//---------------------------------------------------------
		
	void CGameNetMsgManager::processEntityMessage(Net::CPaquete* packet)
	{
		// Creamos un buffer con los datos para leer de manera más cómoda
		Net::CBuffer serialMsg;
			serialMsg.write(packet->getData(),packet->getDataLength());
			serialMsg.reset(); // Reiniciamos el puntero de lectura a la posición 0

		// Extraemos, pero ignoramos el tipo de mensaje de red. Ya lo hemos procesado		
		Net::NetMessageType msgType;
			serialMsg.read(&msgType,sizeof(msgType));
		
		// Escribimos el id de la entidad
		TEntityID destID; 
			serialMsg.read(&destID, sizeof(destID));
			
		//leemos el mensaje que se ha enviado por la red
		int typeMessage;
		serialMsg.read(&typeMessage, sizeof(int));
		//std::cout << "mensaje de tipo " << typeMessage << std::endl;
		std::shared_ptr<CMessage> messageReceived( Logic::CMessageFactory::getSingletonPtr()->create(typeMessage) );
			messageReceived->deserialize(serialMsg);

		// Me saco la entidad a la que va destinado el mensaje y se lo envío
		CEntity* destEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
		if(destEntity != 0)
			destEntity->emitMessage(messageReceived);

		// El servidor ya no debe forwardear las teclas a los remotePlayers. Para eso estan las snapshots, los
		// remotePlayers serán integramente interpolados.
		// El input debe ser predicho de alguna forma. El servidor deberia devolver las teclas o los movimientos
		// solo al player que las manda para que éste sepa si se está moviendo bien o no.
		/*if(Net::CManager::getSingletonPtr()->imServer() && messageReceived->getMessageType()==Message::CONTROL){
			Net::NetMessageType msgType = Net::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
			Net::CBuffer serialMsg;
			Net::CBuffer *buffer = messageReceived->serialize();
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			serialMsg.write(buffer->getbuffer(), buffer->getSize());
			Net::CManager::getSingletonPtr()->broadcastIgnoring(packet->getConexion()->getId(), serialMsg.getbuffer(), serialMsg.getSize());
		}*/

		//LOG("RX ENTITY_MSG " << rxMsg._type << " from EntityID " << destID);
	} // processEntityMessage


	/*******************
		NET: IObserver
	*********************/
	// Aquí es donde debemos recibir los mensajes de red
	void CGameNetMsgManager::dataPacketReceived(Net::CPaquete* packet)
	{
		Net::CBuffer rxSerialMsg; // Packet: "NetMessageType | extraData"
			rxSerialMsg.write(packet->getData(),packet->getDataLength());
			rxSerialMsg.reset();

		Net::NetMessageType rxMsgType;
			rxSerialMsg.read( &rxMsgType, sizeof(rxMsgType) );			
		switch (rxMsgType)
		{

		case Net::DESTROY_ENTITY:	
			processDestroyEntity(packet);
			break;	
		case Net::ENTITY_MSG:	
			processEntityMessage(packet);
			break;	

		case Net::CREATE_CUSTOM_ENTITY:
		case Net::CREATE_ENTITY:	
			processCreateEntity(packet);
			break;	

		case Net::END_GAME:	

			TEntityID entityID; 
				rxSerialMsg.read(&entityID, sizeof(entityID) );  //	Packet: "NetMessageType | extraData(NetID)"	
			
			CEntity* player = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(entityID);
				if( player->isPlayer() )						// GameOver si el END_GAME es para nuestro player
					Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
				else											// Si no, eliminamos ese player del mapa
					player->deactivate();
				//CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entity);

			break;
		}
		
	} // dataPacketReceived

	//--------------------------------------------------------

} // namespace Logic
