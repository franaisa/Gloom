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
		for(auto it = _particles.begin(); it != _particles.end(); ++it){
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

		std::string nameParticleSystemAndAmount = completeNameParticleSystem.substr(0, completeNameParticleSystem.find(".particle"));
		int positionUnderscore = nameParticleSystemAndAmount.length()-1;
		int amount = std::atoi(nameParticleSystemAndAmount.substr(positionUnderscore).c_str());
		std::string nameParticleSystem = nameParticleSystemAndAmount.substr(0,positionUnderscore-1);
		printf("\nCargada particula: %s, %d veces", nameParticleSystem.c_str(), amount);

		TParticles aux;
		aux.index = amount;
		std::vector<CParticle*> vectorParticle;
		for(unsigned int i = 0; i < amount; ++i){
			vectorParticle.push_back(new CParticle(nameParticleSystem));
		}
		aux.particles=vectorParticle;

		vectorParticle[0]->loadResources();

		_particles.insert(std::pair<std::string, TParticles>(nameParticleSystem,aux));
	}
	void CPoolParticle::tick(float secs)
	{	
		
	} // tick

	//--------------------------------------------------------

} // namespace Graphics
