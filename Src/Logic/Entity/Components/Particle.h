/**
@file Particle.h

Componente que contiene la declaracion de las particulas.

@see Logic::CParticle
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/
#ifndef __Logic_Particle_H
#define __Logic_Particle_H

#include "Graphics/Particle.h"
#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"

namespace Graphics{
	class CScene;
}
//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la particula que tiene un objeto. 
	
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Marzo, 2013
*/
	class CParticle : public IComponent
	{
		DEC_FACTORY(CParticle);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CParticle();

		virtual ~CParticle();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void onActivate();

		
		/**
		mensajes aceptados por el componente
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		

	protected:


		/**
		Metodo que se llama en el 1º tick
		*/
		virtual void onStart();


		/**
		Método llamado en cada frame que actualiza el estado del componente de la particula.
				

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		struct TParticle{
			std::string _particleName;
			Vector3 _particleOffset;
			Vector3 _particleEmitterDirection;
			bool _particleVisible;
			Graphics::CParticle *_particle;
			TParticle (): _particleName(""), _particleOffset(Vector3::ZERO), _particleEmitterDirection(Vector3::ZERO), _particleVisible(true), _particle(0){};
		};
		/**
		Puntero al objeto particula
		*/
		std::vector<TParticle> _particles;

		Graphics::CScene *_scene;
	}; // class CParticle

	REG_FACTORY(CParticle);

} // namespace Logic

#endif // __Logic_Particle_H
