/**
@file CWeaponsManagerClient.h

Contiene la declaración de la cabecera del componente
que controla el manejo de las armas en el cliente.

@see Logic::CWeaponsManagerClient
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Abril, 2013
*/

#ifndef __Logic_WeaponsManagerClient_H
#define __Logic_WeaponsManagerClient_H

#include "Logic/Entity/Component.h"

#include <vector>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic {
	class CShoot;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la activacion/desactivacion de las armas que tenemos.
	En este caso al ser el cliente solo interesan los proyectiles.
*/


	class CWeaponsManagerClient : public IComponent
	{
		DEC_FACTORY(CWeaponsManagerClient);

	public:

		enum WeaponType {
			eHAMMER,
			eSNIPER,
			eSHOTGUN,
			eMINIGUN,
			eGRENADE_LAUNCHER,
			eROCKET_LAUNCHER
		};

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CWeaponsManagerClient();
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo SHOOT (por ahora).
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo SHOOT la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

				/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual void activate();

		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void deactivate();

		void changeWeapon(unsigned char newWeapon);

		void addWeapon(int ammo, int weaponIndex);

	protected:
		/**
		Función que se llama cuando se quiere cambiar de arma utilizando el scroll. A partir del arma actual,
		recorre el array del inventario de armas hacia adelante (iWeapon == 100) o hacia atrá (iWeapon == -100),
		en función de la dirección que se le haya dado al scroll.
		@return Devuelve el índice del siguiente arma que tenemos (hacia arriba del inventario u hacia abajo)
		Si no ha encontrado ningún arma siguiente, devuelve -1
		*/
		int selectScrollWeapon(int iWeapon);

		/**
		arma actual equipada
		*/
		int _currentWeapon;

		/**
		numero de armas que tengo
		*/
		const unsigned int _numWeapons;

		std::vector< std::pair<bool, CShoot*> > _weaponry;

	}; // class WeaponsManagerClient

	REG_FACTORY(CWeaponsManagerClient);

} // namespace Logic

#endif // __Logic_WeaponsManagerClient_H
