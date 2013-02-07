#ifndef __Logic_WeaponsManager_H
#define __Logic_WeaponsManager_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la capacidad de disparo de una entidad, asi como las armas que puede tener. Procesa mensajes de tipo 
	SHOOT (indican que la entidad ha disparado)
	<p>
	Poseera una vector, con las armas que tiene, asi como su daño y alcance.
	Tambien poseera un vector de booleanos que indicara las armas que posee.

*/


	class CWeaponsManager : public IComponent
	{
		DEC_FACTORY(CWeaponsManager);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CWeaponsManager() : IComponent(), _actualWeapon(0), _numWeapons(0) {}
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo SHOOT (por ahora).
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo SHOOT la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

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

		void addAmmo(int ammo, int weapon);

		void addWeapon(int ammo, int weapon);

		void resetAmmo(int weapon);
	

	protected:



		/**
		dado un weapon desactivo su componente
		*/
		void desactivateComponent(unsigned char weapon);
		

		/**
		dado un weapon activo su componente
		*/
		void activateComponent(unsigned char weapon);


		/**
		arma actual equipada
		*/
		int _actualWeapon;


		/**
		numero de armas que tengo
		*/
		int _numWeapons;

		bool *_weapons;

	}; // class CShoot

	REG_FACTORY(CWeaponsManager);

} // namespace Logic

#endif // __Logic_Shoot_H
