/**
@file MagneticBullet.h

Contiene la declaración del componente que controla las balas de las shotGun .

@see Logic::CMagneticBullet
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Mayo, 2013
*/
#ifndef __Logic_MagneticBullet_H
#define __Logic_MagneticBullet_H

#include "Logic/Entity/Component.h"


namespace Logic {
	class CShootShotGun;
	class CPhysicDynamicEntity;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla las balas de la shotgun. 
	
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Marzo, 2013
*/
	class CMagneticBullet : public IComponent
	{
		DEC_FACTORY(CMagneticBullet);
	public:
		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CMagneticBullet() : _speed(0), _returning(false), _heightShoot(0), IComponent() {}

		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		mensajes aceptados por el componente
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		procesa los mensajes que son aceptados
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		establece quien es el dueño de esta clase
		@param owner, dueño de esta clase
		*/
		void setOwner(CShootShotGun *owner);

		/**
		Establece las propiedades para que cada bala funcione bien.
		@param owner, dueño de la clase
		@param speed, velocidad de movimiento del proyectil
		@param projectileDirection, direccion por la que ira el proyectil
		@param heightShoot, altura del disparo del jugador
		*/
		void setProperties(CShootShotGun *owner, float speed, Vector3 projectileDirection, int heightShoot){ 
			_owner = owner; 
			_speed = speed; 
			_projectileDirection = projectileDirection;
			_heightShoot = heightShoot;
		
		};

		/**
		Establece la velocidad de la bala
		@param speed, velocidad de desplazamiento
		*/
		void setSpeed(float speed){ _speed = speed;};
		
		/**
		Establece direccion del proyectil
		@param projectileDirection, direccion en la que ira el proyectil
		*/
		void setProjectileDirection(Vector3 projectileDirection);
		
		/**
		meotdo que se invoca cuando recive la colision con alguna entidad.
		Si es el world la bala sera destruida y si es el player se pueden diferenciar dos casos:
		<ol>
			<li>Que este recien disparado y por lo tanto ignara el contacto</li>
			<li>O que se haya pulsado el disparo secundario y la municion este volviendo asi que se añadira a la municion actual</li>
		</ol>
		*/
		void impact(CEntity *impactEntity);
	protected:

		/**
		Método llamado en cada frame que actualiza el estado del componente de luz.
				

		@param msecs Milisegundos transcurridos desde el último tick.
		*/


		virtual void onFixedTick(unsigned int msecs);

		/**
		Puntero al componente fisico de la bala, se tiene por optimizacion
		*/
		CPhysicDynamicEntity *_physicComponent;

		/**
		Velocidad de desplazamiento de la bala
		*/
		float _speed;

		/**
		Direccion en la que se movera la bala
		*/
		Vector3 _projectileDirection;

		/**
		Puntero al dueño de la bala
		*/
		CShootShotGun *_owner;

		/**
		Variable booleana que indica si se ha activado el boton derecho
		*/
		bool _returning;

		/**
		Altura del disparo
		*/
		int _heightShoot;

	}; // class CMagneticBullet

	REG_FACTORY(CMagneticBullet);

} // namespace Logic

#endif // __Logic_MagneticBullet_H
