//---------------------------------------------------------------------------
// PoolParticle.cpp
//---------------------------------------------------------------------------

/**
@file PoolParticle.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CPoolParticle

@author Antonio Jesús Narvaez Corrales
@author Rubén Mulero Guerrero
@date February, 2010
*/

#include "Particle.h"
#include "PoolParticle.h"
#include "Server.h"
#include "Scene.h"

#include "BaseSubsystems/Math.h"

#include <iostream>
#include <windows.h>

namespace Graphics 
{
	CPoolParticle::CPoolParticle(){
		

	} // CPoolParticle
	//--------------------------------------------------------

	CPoolParticle::~CPoolParticle() 
	{
		// borro todas las particulas almacenadas
		for(auto it = _particlesMap.begin(); it != _particlesMap.end(); ++it){
			for(auto it2 = (*it).second.particles.begin(); it2 < (*it).second.particles.end(); ++it2){
				delete (*it2);
			}
		}
	} // ~CPoolParticle
	//--------------------------------------------------------

	void CPoolParticle::activate()
	{
		WIN32_FIND_DATA findFileData;
		HANDLE          hFind;
 
		char* dir(".\\media\\particles\\*");
 
		hFind = FindFirstFile(dir, &findFileData);
 
		if (hFind == INVALID_HANDLE_VALUE)
 			std::cout << "Ruta incorrecta";
		else
		{
			loadParticleSystem(findFileData.cFileName);
 
        // Listamos todos los ficheros restantes
        while (FindNextFile(hFind, &findFileData) != 0)
			loadParticleSystem(findFileData.cFileName);
		}

	} // activate
	//--------------------------------------------------------
	
	void CPoolParticle::loadParticleSystem(const std::string &completeNameParticleSystem){
		if(completeNameParticleSystem == "." || completeNameParticleSystem == ".." ||  completeNameParticleSystem ==  "LEEME.txt")
			return;
		int position = completeNameParticleSystem.find(".particle");
		if(position == std::string::npos)
			return;
		std::string nameParticleSystemAndAmount = completeNameParticleSystem.substr(0, position);
		int positionUnderscore = nameParticleSystemAndAmount.length()-2;
		int amount = std::atoi(nameParticleSystemAndAmount.substr(positionUnderscore).c_str());
		std::string nameParticleSystem = nameParticleSystemAndAmount.substr(0,positionUnderscore-1);
		printf("\nCargada particula: %s, %d veces", nameParticleSystem.c_str(), amount);

		TParticles aux;
		aux.index = 0;
		aux.maxIndex = amount;
		std::vector<CParticle*> vectorParticle;
		for(unsigned int i = 0; i < amount; ++i){
			CParticle *particle = new CParticle(nameParticleSystem);
			particle->deactivate();
			vectorParticle.push_back(particle);
		}
		aux.particles=vectorParticle;

		vectorParticle[0]->loadResources();

		_particlesMap.insert(std::pair<std::string, TParticles>(nameParticleSystem,aux));

	} // loadParticleSystem
	//--------------------------------------------------------

	CParticle* CPoolParticle::getParticle(const std::string &nameParticle){
	
		auto particleData = _particlesMap.find(nameParticle);
		if(particleData == _particlesMap.end())
			return 0;

		unsigned int index = particleData->second.index;
		CParticle * result = particleData->second.particles[index];
		// Si la particula del indice esta emitiendo, significa que he dado la vuelta al vector y he llegado hasta una particula 
		// q esta emitiendo, por lo que no puedo devolver ninguna
		if(result->isEmitting())
			return 0;

		particleData->second.index = (index+1)%(particleData->second.maxIndex);

		result->deactivate();
		result->activate();
		return result;

	} // getParticle
	//--------------------------------------------------------

	void CPoolParticle::setVisible(const std::string &nameParticle, bool visible){
		auto particleData = _particlesMap.find(nameParticle)->second.particles;
		for(auto it = particleData.begin(); it < particleData.end(); ++it){
			if((*it)->isEmitting()){
				(*it)->setVisible(visible);
			}
		}
		
	} // setVisible
	//--------------------------------------------------------

	void CPoolParticle::tick(float secs)
	{	
		
	} // tick
	//--------------------------------------------------------

} // namespace Graphics
