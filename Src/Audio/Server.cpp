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
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/AudioListener.h"

#include <cassert>
#include <iostream>

using namespace FMOD;
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

		createCRCTable( _audioResourcesPath.substr(0, _audioResourcesPath.size() - 1) );

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

	void CServer::tick(unsigned int msecs) {
		_timeToExecute += msecs;
		//Si ha pasado el tiempo mínimo para actualizarnos lo hacemos
		if(_timeToExecute >= _minimumExecuteTime) {
			//Reseteamos el tiempo
			_timeToExecute = 0;
			//Si hay un player con el que actualizarnos, seteamos la nueva posición
			if(_soundAvatar) {
				Logic::CAvatarController* avatarCont = _soundAvatar->getComponent<Logic::CAvatarController>("CAvatarController");
				Vector3 momentum = avatarCont != NULL ? avatarCont->getMomentum() : Vector3::ZERO;

				Vector3 positionAvatar	=_soundAvatar->getPosition();
				Vector3 directionAvatar =_soundAvatar->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
				directionAvatar.normalise();

				FMOD_VECTOR
					listenerPos =	{positionAvatar.x, positionAvatar.y + _playerHeight, positionAvatar.z}, // posición del listener
					listenerVel =	{momentum.x, momentum.y, momentum.z},									// velocidad
					up =			{0.0f, 1.0f, 0.0f},														// vector up: hacia la “coronilla”
					at =			{directionAvatar.x, directionAvatar.y, directionAvatar.z};				// vector at: hacia donde se mira

				// se coloca el listener
				_system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &at, &up);
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
			throw;
		}
	}//ERRCHECK
	//--------------------------------------------------------

	FMOD_RESULT F_CALLBACK trackEnd(FMOD_CHANNEL *srcChannel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2) {
		if(type == FMOD_CHANNEL_CALLBACKTYPE_END && commanddata1 != NULL) {
			Channel* channel = reinterpret_cast<Channel*>(srcChannel);
			Logic::IAudioListener* listener = reinterpret_cast<Logic::IAudioListener*>(commanddata1);
			
			int channelIndex;
			channel->getIndex(&channelIndex);
			listener->trackEnd(channelIndex);
		}

		return FMOD_OK;
	}

	//--------------------------------------------------------

	unsigned int CServer::playSound(const string& soundName, bool loopSound, bool streamSound, Logic::IAudioListener* userData) {
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
		
		if( result != FMOD_OK ) {
			std::cerr << "Error: No se ha podido cargar el fichero " << soundName << std::endl;
			throw;
		}

		//Reproducción en channel
		Channel *channel;
		result = _system->playSound(
			FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
			sound, // sonido que se “engancha” a ese channel
			false, // arranca sin “pause” (se reproduce directamente)
			& channel); // devuelve el channel que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		result = channel->setVolume(_volume);
		ERRCHECK(result);

		// Si nos pasan un puntero al listener fijamos el callback
		// para que se le avise de los eventos pertinentes
		if(userData != NULL)
			channel->setCallback(trackEnd);

		channel->setUserData( (void*)userData );

		int channelIndex;
		channel->getIndex(&channelIndex);
		return channelIndex;
	}//playSound

	//--------------------------------------------------------

	unsigned int CServer::playSound3D(const string& soundName, const Vector3& position, const Vector3& speed, bool loopSound, bool streamSound, Logic::IAudioListener* userData) {
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
		
		if( result != FMOD_OK ) {
			std::cerr << "Error: No se ha podido cargar el fichero " << soundName << std::endl;
			throw;
		}

		Channel *channel;

		//Reproducción en channel
		result = _system->playSound(
		FMOD_CHANNEL_FREE , // dejamos que FMOD seleccione cualquiera
		sound, // sonido que se “engancha” a ese channel
		false, // arranca sin “pause” (se reproduce directamente)
		& channel); // devuelve el channel que asigna
		ERRCHECK(result);
		// el sonido ya está reproduciendo!!
		result = channel->setVolume(_volume);
		ERRCHECK(result);

		FMOD_VECTOR
			pos={position.x,position.y,position.z}, // posición
			vel={speed.x, speed.y, speed.z};  // velocidad (para el doppler)
			channel->set3DAttributes(&pos, &vel);
			ERRCHECK(result);

		//Distancia a la que empieza a atenuarse y a la cual ya no se atenua mas respectivamente
		result = channel->set3DMinMaxDistance(50.0f,10000.0f);
		ERRCHECK(result);

		// Si nos pasan un puntero al listener fijamos el callback
		// para que se le avise de los eventos pertinentes
		if(userData != NULL)
			channel->setCallback(trackEnd);

		channel->setUserData( (void*)userData );

		int channelIndex;
		channel->getIndex(&channelIndex);
		return channelIndex;
	}//playSound3D
	//--------------------------------------------------------

	void CServer::stopSound(int channelIndex) {
		Channel* channel;
		FMOD_RESULT result = _system->getChannel(channelIndex, &channel);
		if(result == FMOD_OK)
			channel->stop();
	}//stopSound
	//--------------------------------------------------------

	void CServer::stopAllSounds() {
		Channel* channel;
		//Recorremos los 32 canales y paramos su reproduccion
		for(int i = 0; i < 32; ++i) {
			_system->getChannel(i, &channel);
			channel->stop();
		}
	}//stopAllSounds
	//--------------------------------------------------------

	void CServer::mute() {
		Channel* channel;
		//Si el server estaba muteado lo desmuteamos y viceversa
		if(_isMute){
			_isMute=false;
			_volume=0.5;
			//Recorremos los 32 canales y cambiamos su volumen
			for(int i=0;i<32;i++){
				_system->getChannel(i,&channel);
				channel->setVolume(0);
			}
		}
		else{
			_isMute=true;
			_volume=0;
			//Recorremos los 32 canales y cambiamos su volumen
			for(int i=0;i<32;i++){
				_system->getChannel(i,&channel);
				channel->setVolume(0);
			}

		}
	}//mute

	//--------------------------------------------------------

	void CServer::createCRCTable(const string& rootDirectory) {
		DIR* dir;
		struct dirent* ent;

		// Si el directorio dado en la primera llamada es invalido,
		// no se hace nada
		if( (dir = opendir( rootDirectory.c_str() ) ) != NULL ) {
			// Explorar todos los elementos del directorio actual
			while( (ent = readdir(dir) ) != NULL ) {
				// Si se trata de un elemento que no sea el directorio actual o el padre
				// comprobamos si se trata de una carpeta o un fichero. Si es una carpeta
				// la exploramos recursivamente, si no, almacenamos el fichero en nuestra 
				// tabla con el formato crc-nombre
				if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
					if(ent->d_type == DT_DIR) {
						// Para acceder al directorio necesitamos la ruta completa
						std::string fullDirectoryPath = rootDirectory + "/" + ent->d_name;
						createCRCTable( fullDirectoryPath.c_str() );
					}
					else {
						// Nos quedamos con la ruta relativa al fichero, que es la que se
						// va a pasar por mensaje y la que queremos traducir
						std::string fileRelativePath = rootDirectory + "/" + ent->d_name;
						fileRelativePath.replace(0, _audioResourcesPath.size(), 0, ' ');

						assert( _CRCTable.insert( std::pair<int, std::string>( Math::CRC(fileRelativePath), fileRelativePath ) ).second && 
							    "Error: Existen dos rutas de audio con el mismo checksum");
					}
				}
			}

			// Una vez explorado el directorio lo cerramos
			closedir(dir);
		}
	}

	//--------------------------------------------------------

	std::string CServer::translateCRC(int CRC) {
		auto it = _CRCTable.find(CRC);
		assert(it != _CRCTable.end() && "Error: Estas buscando un CRC que no existe");

		cout << CRC << " = " << it->second << endl;

		return it->second;
	}

	//--------------------------------------------------------

	void CServer::update3DAttributes(int channelIndex, const Vector3& position, const Vector3& speed) {
		Channel* channel;
		_system->getChannel(channelIndex, &channel);

		FMOD_VECTOR
			pos = {position.x, position.y, position.z},
			vel = {speed.x, speed.y, speed.z}; 

		channel->set3DAttributes(&pos, &vel);
	}

} // namespace Audio