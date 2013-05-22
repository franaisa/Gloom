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
		


	} // activate

	//--------------------------------------------------------

	void CPoolParticle::deactivate()
	{
		

	} // deactivate
	
	//--------------------------------------------------------

	
	void CPoolParticle::tick(float secs)
	{	
		
	} // tick

	//--------------------------------------------------------

} // namespace Graphics
