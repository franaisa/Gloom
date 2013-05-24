//---------------------------------------------------------------------------
// ShootSniper.h
//---------------------------------------------------------------------------

/**
@file ShootSniper.h

Contiene la declaración del componente que implementa el arma sniper.

@author Antonio Jesus Narváez Corrales
@date Febrero, 2013
*/

#ifndef __Logic_ShootSniper_H
#define __Logic_ShootSniper_H

#include "Logic/Entity/Components/ShootRaycast.h"
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

	class CShootSniper : public CShootRaycast {
		DEC_FACTORY(CShootSniper);

	public:

		/** Constructor por defecto. */
		CShootSniper() : CShootRaycast("sniper") {}

		virtual ~CShootSniper();

		//__________________________________________________________________

		
		/**
		Redefinimos porque la sniper tendrá un comportamiento diferente.
		*/
		virtual void primaryShoot();

		/**
		Redefinimos porque la sniper tendrá un comportamiento diferente.
		*/
		virtual void secondaryShoot();

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
		
		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		Método que se encarga de realizar el disparo primario del raycast por fisicas.
		También aplicará el daño.

		*/
		void primaryFireWeapon();


		/**
		Método que se encarga de realizar el disparo secundario del raycast por fisicas.
		También aplicará el daño.
		*/
		void secondaryFireWeapon();


		/**
		Método que se encarga de intentar aplicar daño por expansión al utilizar el disparo secundario de la sniper.

		@param entityHit Entidad a partir de la cual queremos expandir el daño.
		@return Devuelve la entidad a la que hay que aplicar daño de expansión.
		*/
		CEntity* findEnemyToExpand(CEntity* entityHit);

	private:

		/**
		Distancia máxima de expansión para el disparo secundario de la sniper.
		*/
		float _maxExpansiveDistance;

		/**
		Numero de balas que consume el disparo secundario.
		*/
		int _secondaryConsumeAmmo;

	}; // class CShootSniper

	REG_FACTORY(CShootSniper);

} // namespace Logic

#endif // __Logic_Shoot_H
