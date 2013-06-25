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

#include "Logic/Entity/Entity.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace Audio
{
	CServer *CServer::_instance = 0;

	CServer::CServer() : _audioResourcesPath("media/audio/")
	{
		assert(!_instance && "Segunda inicialización de Audio::CServer no permitida!");
		_volume=0.5f;
		_doppler=1.0f;
		_rolloff=1.0f;
		_soundAvatar=NULL;
		_playerHeight=8;
		_isMute=false;
		_instance = this;
		_minimumExecuteTime=100;
		_timeToExecute=0;
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
		assert(!_instance && "Segunda inicialización de Audio::CServer no permitida!");

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
		std::cout << "Liberamos el server de audio" << std::endl;
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
		result = _system->init(32, FMOD_INIT_3D_RIGHTHANDED, 0);
		ERRCHECK(result);

		//Configuración 3D, el parámetro central es el factor de distancia (FMOD trabaja en metros/segundos)
		_system->set3DSettings(_doppler, 1.0f, _rolloff);

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		stopAllSounds();
		_soundAvatar=NULL;
		_system->release();

	} // close

	//--------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		_timeToExecute+=msecs;
		//Si ha pasado el tiempo mínimo para actualizarnos lo hacemos
		if(_timeToExecute>=_minimumExecuteTime){
			//Reseteamos el tiempo
			_timeToExecute=0;
			//Si hay un player con el que actualizarnos, seteamos la nueva posición
			if(_soundAvatar){
				Vector3 positionAvatar=_soundAvatar->getPosition();
				Vector3 directionAvatar=_soundAvatar->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
				directionAvatar.normalise();

				FMOD_VECTOR
					listenerPos = {positionAvatar.x,positionAvatar.y+_playerHeight,positionAvatar.z}, // posición del listener
					listenerVel = {0.0f,0.0f,0.0f}, // velocidad
					up = {0.0f,1.0f,0.0f},          // vector up: hacia la “coronilla”
					at = {directionAvatar.x,directionAvatar.y,directionAvatar.z};          // vector at: hacia donde se mira

				// se coloca el listener
				_system->set3DListenerAttributes(0,&listenerPos, &listenerVel,
										 &at, &up);
			}
			//Actualizamos el sistema
			_system->update();
		}
	} // tick
	//--------------------------------------------------------

	// función para dar salida de error y terminar aplicación
	void CServer::ERRCHECK(FMOD_RESULT result){
		if (result != FMOD_OK){
			cerr << "FMOD error! " << result << endl << FMOD_ErrorString(result);
			exit(-1);
		}
	}//ERRCHECK
	//--------------------------------------------------------

	void CServer::playSound(const string& soundName, bool loopSound, bool streamSound){
		unsigned int soundMask = loopSound ? (FMOD_DEFAULT | FMOD_LOOP_NORMAL) : FMOD_DEFAULT;
		
		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result;
		if(streamSound) {
			result = _system->createStream(
				(_audioResourcesPath + soundName).c_str(),	// path del archivo de sonido
				soundMask,									// flags
				0,											// información adicional (nada en este caso)
				& sound	); 									// devolución del handle al buffer
										  
											
		}
		else {
			result = _system->createSound( 
				(_audioResourcesPath + soundName).c_str(),	// path del archivo de sonido
				soundMask,									// flags
				0,											// información adicional (nada en este caso)
				& sound	);									// devolución del handle al buffer 
		}
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
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		int can;
		canal->getIndex(&can);
		//cout << "el numero de canal ocupado es " << can << endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		//cout << "El numero de canales usados al cargar el sonido es " << numcanales << endl;

		//Guardamos la asociacion nombreSonido/Canal
		size_t charPosition = soundName.find('.');
		if(charPosition != std::string::npos) {
			_soundChannel[soundName.substr(0, charPosition)] = canal;
		}
		else {
			_soundChannel[soundName] = canal;
		}

	}//playSound
	//--------------------------------------------------------

	void CServer::playSound3D(const string& soundName, const Vector3& position, const Vector3& speed, bool loopSound, bool streamSound){
		unsigned int soundMask = loopSound ? (FMOD_3D | FMOD_LOOP_NORMAL) : FMOD_3D;

		//Carga del sonido
		Sound *sound;
		FMOD_RESULT result;
		if(streamSound) {
			result = _system->createStream(
				(_audioResourcesPath + soundName).c_str(),	// path del archivo de sonido
				soundMask,									// flags
				0,											// información adicional (nada en este caso)
				& sound	); 									// devolución del handle al buffer
										  
											
		}
		else {
			result = _system->createSound( 
				(_audioResourcesPath + soundName).c_str(),	// path del archivo de sonido
				soundMask,									// flags
				0,											// información adicional (nada en este caso)
				& sound	);									// devolución del handle al buffer 
		}
		ERRCHECK(result);

		
		Channel *canal;
		//Reproducción en canal
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese canal
		false, // arranca sin “pause” (se reproduce directamente)
		& canal); // devuelve el canal que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		result = canal->setVolume(_volume);
		ERRCHECK(result);

		FMOD_VECTOR
			pos={position.x,position.y,position.z}, // posición
			vel={speed.x, speed.y, speed.z};  // velocidad (para el doppler)
			canal->set3DAttributes(& pos, & vel);
			ERRCHECK(result);

		//Distancia a la que empieza a atenuarse y a la cual ya no se atenua mas respectivamente
		result = canal->set3DMinMaxDistance(50.0f,10000.0f);
		ERRCHECK(result);
	/*	int can;
		canal->getIndex(&can);
		cout << "el numero de canal ocupado es " << can << endl;
		int numcanales;
		_system->getChannelsPlaying(&numcanales);
		cout << "El numero de canales usados al cargar el sonido es " << numcanales << endl;*/

		//Guardamos la asociacion nombreSonido/Canal
		size_t charPosition = soundName.find('.');
		if(charPosition != std::string::npos) {
			_soundChannel[soundName.substr(0, charPosition)] = canal;
		}
		else {
			_soundChannel[soundName] = canal;
		}
	}//playSound3D
	//--------------------------------------------------------

	void CServer::stopSound(const string& soundName){
		auto it = _soundChannel.find(soundName);
		if( it != _soundChannel.end() ){
			it->second->stop();
			_soundChannel.erase(it);
		}
	}//stopSound
	//--------------------------------------------------------

	void CServer::stopAllSounds(){
		Channel *canal;
		//Recorremos los 32 canales y paramos su reproduccion
		for(int i=0;i<32;i++){
			_system->getChannel(i,&canal);
			canal->stop();
		}
		//Limpiamos el mapa idSonido-canal
		_soundChannel.clear();
	}//stopAllSounds
	//--------------------------------------------------------

	void CServer::mute(){
		Channel* canal;
		//Si el server estaba muteado lo desmuteamos y viceversa
		if(_isMute){
			_isMute=false;
			_volume=0.5;
			//Recorremos los 32 canales y cambiamos su volumen
			for(int i=0;i<32;i++){
				_system->getChannel(i,&canal);
				canal->setVolume(0);
			}
		}
		else{
			_isMute=true;
			_volume=0;
			//Recorremos los 32 canales y cambiamos su volumen
			for(int i=0;i<32;i++){
				_system->getChannel(i,&canal);
				canal->setVolume(0);
			}

		}

	}//mute
	//--------------------------------------------------------



} // namespace Audio