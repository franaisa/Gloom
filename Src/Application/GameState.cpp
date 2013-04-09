//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementación del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llansó
@date Agosto, 2010
*/

#include "GameState.h"
#include "OgreClock.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "Input/Server.h"
#include "Input\PlayerController.h"

#include "Physics/Server.h"
#include "Audio\Server.h"

//#include <CEGUISystem.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>

namespace Application {

	bool CGameState::init() 
	{
		// Ejecutamos la inicialización de la clase padre
		// En este caso no hace nada, solo retorna true
		CApplicationState::init();

		// INICIALIZACIÓN DE LA FÍSICA
		// ---------------------------

		// TODO: desactivar colisiones entre los grupos 0 y 1
		//Physics::CServer::getSingletonPtr()->setGroupCollisions(0, 1, false);

		// TODO: Crear la escena física usando el servidor de física
		//Physics::CServer::getSingletonPtr()->createScene();

		// INICIALIZACIÓN DE LA LÓGICA
		// ---------------------------

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		/*if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
			return false;*/

		// INICIALIZACIÓN DEL GUI
		// ----------------------

		// Ahora mismo la implementación está totalmente acoplada a CEGUI
		// Hay que desacoplarlo utilizando un nuevo paquete donde se abstraiga
		// el subsistema utilizado

		// Cargamos la ventana que muestra el tiempo de juego transcurrido.
		//CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Time.layout");
		//_timeWindow = CEGUI::WindowManager::getSingleton().getWindow("Time");
		//LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Time");
		
		//LAYOUT MIRA
		//GUI::CServer::getSingletonPtr()->addLayoutToState(this, "Mira");
		_time = 0;
		_timelogic=0;
		_timephysics=0;
		return true;
	} // init

	//--------------------------------------------------------

	void CGameState::release() 
	{
		// Liberar los recursos reservados para la escena y las
		// clases construidas a partir de los blueprints
		Logic::CServer::getSingletonPtr()->unLoadLevel();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();
		// Liberar la escena física usando el motor de física
		Physics::CServer::getSingletonPtr()->destroyScene();

		// Llamar al método padre por si acaso tiene que hacer algo
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameState::activate() 
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		// Queremos que el GUI maneje al jugador.
		Input::CServer::getSingletonPtr()->getPlayerController()->activate();
		Logic::CEntityFactory::getSingletonPtr()->dynamicCreation(true);

		//La picadura no te escapas
		Audio::CServer::getSingletonPtr()->stopSound("theme");
		//Audio::CServer::getSingletonPtr()->playLoopSound("media/audio/picadura.mp3","picadura");

		// Activamos la ventana que nos muestra el tiempo transcurrido.
		//CEGUI::System::getSingletonPtr()->setGUISheet(_timeWindow);
		//_timeWindow->setVisible(true);
		//_timeWindow->activate();

		//LAYOUT TIME
		//GUI::CServer::getSingletonPtr()->activateGUI(this, "Time");
	
		//LAYOUT MIRA
		//GUI::CServer::getSingletonPtr()->activateGUI(this, "Mira");
	} // activate

	//--------------------------------------------------------

	void CGameState::deactivate() 
	{
		std::cout << "tiempo de procesado fisico: " << _timephysics << std::endl;
		std::cout << "tiempo de procesado logico: " << _timelogic << std::endl;
		std::cout << "tiempo total jugado: " << _time << std::endl;
		// Desactivamos la clase que procesa eventos de entrada para 
		// controlar al jugador.
		Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();

		Logic::CServer::getSingletonPtr()->unLoadLevel();
		
		CApplicationState::deactivate();

		Logic::CEntityFactory::getSingletonPtr()->dynamicCreation(false);
	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		unsigned int time = clock();
		// TODO: realizar la simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);
		//tiempo que ha tardado la fisica
		time = clock()-time;
		_timephysics+=time;
		
		time = clock();

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);
		//tiempo que ha tardado la logica
		time = clock()-time;
		_timelogic+=time;
		_time += msecs;

	} // tick

	//--------------------------------------------------------

	bool CGameState::keyPressed(Input::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameState::keyReleased(Input::TKey key)
	{
		switch(key.keyId)
		{
		case Input::Key::ESCAPE:
			
			//Logic::CServer::getSingletonPtr()->unLoadLevel();
/*
			Logic::CServer::getSingletonPtr()->unLoadLevel();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();
		// Liberar la escena física usando el motor de física
		Physics::CServer::getSingletonPtr()->destroyScene();

		// Llamar al método padre por si acaso tiene que hacer algo
		CApplicationState::release();

		CApplicationState::init();
		*/
			_app->setState("menu");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
