//---------------------------------------------------------------------------
// Sniper.h
//---------------------------------------------------------------------------

/**
@file Sniper.h

Contiene la declaración del componente que implementa el arma sniper.

@author Antonio Jesus Narváez Corrales
@date Febrero, 2013
*/

#ifndef __Logic_Sniper_H
#define __Logic_Sniper_H

#include "Logic/Entity/Components/Weapon.h"
#include "Physics/RaycastHit.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementación por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narváez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CSniper : public IWeapon {
		DEC_FACTORY(CSniper);

	public:

		/** Constructor por defecto. */
		CSniper() : IWeapon("sniper"), _primaryFireTimer(0), _secondaryFireTimer(0), _primaryFireIsActive(false), _secondaryFireIsActive(false), _burned(false) {}

		virtual ~CSniper();

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		
		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		
		/**
		Llamado cuando el arma pasa a ser activa.
		*/
		virtual void onActivate();

		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		Método que se encarga de realizar el disparo primario del raycast por fisicas.
		También aplicará el daño.

		*/
		virtual void primaryFire();


		/**
		Método que se encarga de realizar el disparo secundario del raycast por fisicas.
		También aplicará el daño.
		*/
		virtual void secondaryFire();


		/**
		Método que se encarga de intentar aplicar daño por expansión al utilizar el disparo secundario de la sniper.

		@param entityHit Entidad a partir de la cual queremos expandir el daño.
		@return Devuelve la entidad a la que hay que aplicar daño de expansión.
		*/
		CEntity* findEnemyToExpand(CEntity* entityHit);

		/**
		Método que se encarga de mandar los mensajes que correspondan a la entidad
		que se ha golpeado en caso de hacer hit.

		A pesar de que se trata de un método virtual puro, esta clase ofrece una
		implementación por defecto que será útil para la mayoría de las subclases. 

		@param entityHit puntero a la entidad que se ha dado (o NULL si no se ha colisionado con ninguna).
		*/
		void triggerHitMessages(CEntity* entityHit, float damageFire);

		virtual void amplifyDamage(unsigned int percentage);

		/**
		Dibuja el rayo de disparo de la sniper
		*/
		void drawRaycast(const Ray& raycast, int distance);

	private:

		//void onTick(unsigned int msecs);

		/**
		Distancia máxima de expansión para el disparo secundario de la sniper.
		*/
		float _maxExpansiveDistance;

		/**
		Numero de balas que consume el disparo secundario.
		*/
		int _secondaryConsumeAmmo;
		
		/**
		Distancia
		*/
		int _shotsDistance;

		int  _defaultPrimaryFireCooldown;
		int _primaryFireCooldown;

		float _defaultPrimaryFireDamage;
		float _primaryFireDamage;

		int _defaultSecondaryFireCooldown;
		int _secondaryFireCooldown;

		float _defaultSecondaryFireDamage;
		float _secondaryFireDamage;

		int _primaryFireTimer;
		int _secondaryFireTimer;

		bool _primaryFireIsActive;
		bool _secondaryFireIsActive;

		/**
		Manejo de incremento de daño por atravesar una bola de fuego.
		*/
		float _burnedIncrementPercentageDamage;
		bool _burned;
		

	}; // class CSniper

	REG_FACTORY(CSniper);

} // namespace Logic

#endif // __Logic__H
