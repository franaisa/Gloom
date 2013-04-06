//---------------------------------------------------------------------------
// GameNetMsgManager.cpp
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.cpp

Contiene la implementación del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author David Llansó
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

		Net::NetMessageType msgType = Net::NetMessageType::DESTROY_ENTITY;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
		//serializamos toda la información que se necesita para la creación de la entidad
		serialMsg.write(&msgType, sizeof(msgType));
		serialMsg.serialize(destID);

		//enviamos el mensaje
		Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize());
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

		//le decimos al mapa que elimine la entidad
		CEntity * entity = CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entity);
	}

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendCreateEntity(TEntityID destID){
		//cogemos la entidad que hemos creado para enviar la información por la red
		CEntity * destEntity = CServer::getSingletonPtr()->getMap()->getEntityByID(destID);

		Net::NetMessageType msgType = Net::NetMessageType::CREATE_ENTITY;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
		//serializamos toda la información que se necesita para la creación de la entidad
		serialMsg.write(&msgType, sizeof(msgType));
		serialMsg.serialize(destID);
		serialMsg.serialize(destEntity->getType(), false);
		serialMsg.serialize(destEntity->getName(), false);
		serialMsg.serialize(destEntity->getTransform());

		//enviamos el mensaje
		Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize());
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


		std::string type;
		serialMsg.deserialize(type);

		std::string name;
		serialMsg.deserialize(name);

		Matrix4 transform;
		serialMsg.deserialize(transform);

		//creamos la entidad
		Map::CEntity * info = Logic::CEntityFactory::getSingletonPtr()->getInfo(type);
		info->setName(name);
		CEntity * newEntity = Logic::CEntityFactory::getSingletonPtr()->createEntity(info, CServer::getSingletonPtr()->getMap(), destID);
		newEntity->activate();
		//ahora le seteamos la posición
		// TIENE QUE TRATARSE DE UN COMPONENTE FISICO DINAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMICO SI NO EXPLOTA
		// (tenemos entidades fisicas dinamicas y estaticas y controllers).
		newEntity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity")->setPhysicPosition(transform.getTrans(), false);

		//seteamos la orientación
		Matrix3 orientation;
		transform.extract3x3Matrix(orientation);
		newEntity->setOrientation(orientation);
	}


	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendEntityMessage(CMessage* txMsg, TEntityID destID)
	{

		Net::CBuffer* bufferAux = txMsg->serialize();

		Net::NetMessageType msgType = Net::NetMessageType::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			serialMsg.write(bufferAux->getbuffer(), bufferAux->getSize()); //Guardamos el mensaje en el buffer
			
		Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize());
		//std::cout << "Enviado mensaje tipo " << txMsg->getMessageType() << " para la entidad " << destID << " de tamaño " << serialMsg.getSize() << std::endl;
		//LOG("TX ENTITY_MSG " << txMsg._type << " to EntityID " << destID);
	} // sendEntityMessage

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendMessageToOne(CMessage* txMsg, TEntityID destID, TEntityID player)
	{

		Net::CBuffer* bufferAux = txMsg->serialize();

		Net::NetMessageType msgType = Net::NetMessageType::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
		Net::CBuffer serialMsg;
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			serialMsg.write(bufferAux->getbuffer(), bufferAux->getSize()); //Guardamos el mensaje en el buffer
			
		Net::NetID idMsg = Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayerByEntityId(player).getNetId();

		Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize(), idMsg);
	} // sendMessageToOne

	//---------------------------------------------------------
		
	void CGameNetMsgManager::processEntityMessage(Net::CPaquete* packet)
	{
		// TODO Método que debe de ser invocado desde el método que
		// recibe todos los paquetes de red cuando el tipo de mensaje
		// de red es Net::ENTITY_MSG. Se debe sacar el ID de la entidad,
		// recuperarla, deserializar el mensaje y enviárselo
		
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
		CMessage * messageReceived = Logic::CMessageFactory::getSingletonPtr()->create(typeMessage);
			messageReceived->deserialize(serialMsg);

		// Me saco la entidad a la que va destinado el mensaje y se lo envío
		CEntity* destEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(destID);
		if(destEntity != 0)
			destEntity->emitMessage(messageReceived);

		if(Net::CManager::getSingletonPtr()->imServer() && messageReceived->getMessageType()==Logic::TMessageType::CONTROL){
			Net::NetMessageType msgType = Net::NetMessageType::ENTITY_MSG;// Escribimos el tipo de mensaje de red a enviar
			Net::CBuffer serialMsg;
			Net::CBuffer *buffer = messageReceived->serialize();
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID)); // Escribimos el id de la entidad destino
			serialMsg.write(buffer->getbuffer(), buffer->getSize());
			Net::CManager::getSingletonPtr()->sendAllExcept(serialMsg.getbuffer(),serialMsg.getSize(),packet->getConexion()->getId());
		}

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

		case Net::NetMessageType::DESTROY_ENTITY:	
			processDestroyEntity(packet);
			break;	
		case Net::NetMessageType::ENTITY_MSG:	
			processEntityMessage(packet);
			break;	

		case Net::NetMessageType::CREATE_ENTITY:	
			processCreateEntity(packet);
			break;	

		case Net::NetMessageType::END_GAME:	

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
