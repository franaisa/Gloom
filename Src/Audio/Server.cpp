//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementación de la clase principal de audio, llevará el control de los sonidos en el juego.

@see Audio::CServer

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "Server.h"

#include <cassert>
#include <iostream>

namespace Audio
{
	CServer *CServer::_instance = 0;

	CServer::CServer()
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		FMOD_RESULT result;
		//Creamos
		result = System_Create(&_system);
		ERRCHECK(result);
			
		//Iniciamos
		result = _system->init(100, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);

		playLoopSound("media/audio/themeGloom.wav", "theme");

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		_system->release();

	} // close

	//--------------------------------------------------------

	void CServer::tick(unsigned int secs) 
	{
		_system->update();
	} // tick
	//--------------------------------------------------------

	// función para dar salida de error y terminar aplicación
	void CServer::ERRCHECK(FMOD_RESULT result){
		if (result != FMOD_OK){
		std::cout << "FMOD error! " << result << std::endl <<FMOD_ErrorString(result);
		exit(-1);
		}
	}//ERRCHECK
	//--------------------------------------------------------

	void CServer::playSound(char* rutaSonido, std::string id){
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createSound(
		rutaSonido, // path del archivo de sonido
		FMOD_DEFAULT, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);

		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		float volume=0.7; // valor entre 0 y 1
		result = canal->setVolume(volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;

		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;

	}//playSound
	//--------------------------------------------------------

	void CServer::playLoopSound(char* rutaSonido, std::string id){
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result = _system->createSound(
		rutaSonido, // path del archivo de sonido
		FMOD_DEFAULT | FMOD_LOOP_NORMAL, // flags
		0, // información adicional (nada en este caso)
		& sound); // devolución del handle al buffer
		ERRCHECK(result);
		//sound->setLoopCount(-1); // sonido a loop asi le iba a david
			
		//Reproducción en canal
		Channel *canal;
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		float volume=0.7; // valor entre 0 y 1
		result = canal->setVolume(volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		std::cout << "el numero de canal ocupado es " << can << std::endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		std::cout << "El numero de canales usados al cargar el sonido es " << numcanales << std::endl;


		//Guardamos la asociacion nombreSonido/Canal
		_soundChannel[id]=canal;

	}//playLoopSound
	//--------------------------------------------------------

	void CServer::stopSound(std::string id){
		SoundChannelMap::const_iterator itMap = _soundChannel.begin();
		SoundChannelMap::const_iterator itErase;
		bool mapErase = false;
		for(; itMap != _soundChannel.end() && !mapErase; ++itMap) {
			if(itMap->first.compare(id)==0) {
				itMap->second->stop();
				itErase=itMap;
				mapErase=true;
			}
		}
		_soundChannel.erase(itErase);
			
	}//stopSound
	//--------------------------------------------------------


	void CServer::stopAllSounds(){
		SoundChannelMap::const_iterator itMap = _soundChannel.begin();
		bool mapErase = false;
		for(; itMap != _soundChannel.end(); ++itMap) {
				itMap->second->stop();
		}
		_soundChannel.clear();
	}//stopSound
	//--------------------------------------------------------

} // namespace Audio