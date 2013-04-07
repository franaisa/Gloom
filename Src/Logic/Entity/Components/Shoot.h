//---------------------------------------------------------------------------
// Shoot.h
//---------------------------------------------------------------------------

/**
@file Shoot.h

Contiene la declaración del componente general de disparo.

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_Shoot_H
#define __Logic_Shoot_H

#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Component.h"

namespace Logic {

	/**
	@ingroup logicGroup

	Este componente define la interfaz que van a utilizar todas las armas
	del player. Cada arma es un nuevo componente que hereda de esta clase
	y que únicamente debe preocuparse de redefinir el método shoot, que 
	será el método que se dispare cuando se detecte que el player ha 
	inicializado la acción de disparo.

	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CShoot : public IComponent {
		//DEC_FACTORY(CShoot); -- Esta clase es abstracta y por lo tanto no instanciable
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShoot() : IComponent(), 
				   _capsuleRadius(3.0f),
				   _cooldownTimer(0),
				   _canShoot(true),
				   _nameWeapon(0), 
				   _currentAmmo(0),
				   _particlePosition(Vector3::ZERO),
				   _isInUse(false) {
		
			// No hay memoria dinamica que inicializar
		}

		//__________________________________________________________________

		/**
		Constructor parametrizado

		@param shoot Nombre del arma que vamos a inicializar.
		*/
		CShoot(const std::string &shoot) : IComponent(),  
										   _capsuleRadius(3.0f),
										   _cooldownTimer(0),
										   _canShoot(true),
										   _nameWeapon(shoot), 
										   _currentAmmo(0),
										   _particlePosition(Vector3::ZERO),
										   _isInUse(false) {
			
			// No hay memoria dinámica que inicializar
		}
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>physic_radius:</strong> Radio de la cápsula fisica del jugador. </li>
			<li><strong>heightShoot:</strong> Altura de la que sale el disparo. </li>
			<li><strong>(weaponName)+Name:</strong> Nombre del arma. </li>
			<li><strong>(weaponName)+Damage:</strong> Daño que hace cada impacto del arma. </li>
			<li><strong>(weaponName)+NumberShoots:</strong> Número de balas por disparo. </li>
			<li><strong>(weaponName)+ColdDown:</strong> Cooldown del arma (tiempo entre disparos). </li>
			<li><strong>(weaponName)+MaxAmmo:</strong> Máxima cantidad de munición equipable. </li>
			<li><strong>(weaponName)+Id:</strong> Identificador del arma. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		//__________________________________________________________________

		/**
		Método llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Este es el método que todas las armas deben redefinir. Es el que implementa
		la acción de disparar.
		*/
		virtual void shoot() = 0;

		//__________________________________________________________________

		/** 
		Activa el componente. Lo utilizaremos para activar el componente del arma
		que esté en uso.
		*/
		virtual void activate();

		//__________________________________________________________________

		/** 
		Añade munición a un arma concreta.

		@param weapon Número de arma a la que queremos añadir munición.
		@param ammo Cantidad de munición que vamos a añadir.
		@param iAmCatch true si tengo el arma de la munición que he recogido.
		*/
		virtual void addAmmo(int weapon, int ammo, bool iAmCatch);

		//__________________________________________________________________

		/**
		Método que decrementa la cantidad de munición que tenemos. Debido a que
		no todas las armas decrementan su munición de la misma forma (algunas
		ni siquiera tienen munición) este método es virtual.
		*/
		virtual void decrementAmmo();

		//__________________________________________________________________

		/** Método estático que resetea la cantidad de munición del arma. */
		virtual void resetAmmo();

		//__________________________________________________________________

		/** Método estático que dibuja la particula. Todas las armas tendran una particula al disparar. 
		@param nombreParticula, nombre para poder identificar esta particula en concreto
		@param Particula, nombre del template de particula definido en el archivo .particle
		*/
		void drawParticle(const std::string &nombreParticula, const std::string &Particula);

		//__________________________________________________________________

		/** 
		Método que se encarga de incrementar el daño del arma 
		
		@param percent Porcentaje de incremento de daño.
		*/
		void incrementDamage(int percent);

		//__________________________________________________________________

		/** 
		Método que se encarga de reducir el cooldown del arma. 

		@param percent Porcentaje de reducción de cooldown.
		*/
		void reduceCooldown(int percent);

		//__________________________________________________________________

		/**
		Sirve para indicar si este arma es el arma actualmente equipada.

		Este método es necesario para evitar que se acepten mensajes en 
		armas que no están siendo equipadas, ya que las armas no pueden desactivarse
		(de lo contrario seria imposible actualizar los timers de los cooldowns).

		@param state true si el arma está equipada, false si no esta en uso.
		*/
		void inUse(bool state);


	protected:


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Nombre del arma. */
		std::string _name;

		/** Nombre del arma. */
		std::string _nameWeapon;

		/** Identificador del arma. */
		int _id;

		/** Radio de la capsula leido del mapa. */
		float _capsuleRadius;

		/** Altura de disparo. */
		int _heightShoot;

		/** Daño actual del arma (modificable por powerups). */
		int _damage;

		/** Daño por defecto del arma. */
		int _defaultDamage;

		/** Número de balas por disparo. */
		int _numberShots;

		/** Munición máxima que puede tener el arma. -1 si no tienes el arma. */
		int _maxAmmo;

		/** Munición actual. */
		int _currentAmmo;

		/** Timer para el cooldown. */
		float _cooldownTimer;

		/** Cooldown actual (modificable por powerups). */
		float _cooldown;

		/** Cooldown por defecto del arma (tiempo entre disparo y disparo). */
		int _defaultCooldown;

		/**
		Para comprobar que un arma esta disparando y no puede disparar 
		"tan rapido como puedas arreglar el gatillo" (Marcus, Borderlands 2)
		*/
		bool _canShoot;

		/**
		Vector donde se posionara la particula de emision de disparo
		*/
		Vector3 _particlePosition;

		/** true si el arma está actualmente equipada por el jugador */
		bool _isInUse;
	}; // class CShoot

	//REG_FACTORY(CShoot);

} // namespace Logic

#endif // __Logic_Shoot_H
