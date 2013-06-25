//---------------------------------------------------------------------------
// WeaponFeedback.h
//---------------------------------------------------------------------------

/**
@file WeaponFeedback.h

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_WeaponFeedback_H
#define __Logic_WeaponFeedback_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

#include <string>

namespace Logic {

	// Forward declarations
	class CHudWeapons;

	/**
	@ingroup logicGroup

	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class IWeaponFeedback : public IComponent {
	public:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/**
		Constructor parametrizado. Las clases hijas deberán llamar a este constructor
		con el nombre de su arma.

		@param weaponName Nombre del arma implementada por la clase derivada.
		*/
		IWeaponFeedback(const std::string& weaponName);

		//__________________________________________________________________

		/** Destructor. */
		virtual ~IWeaponFeedback();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>MaxAmmo:</strong> Munición máxima que puede llevar el arma. </li>
			<li><strong>ID:</strong> Identificador de arma. </li>
			<li><strong>physic_radius:</strong> Radio de la cápsula física del personaje. </li>
			<li><strong>heightShoot:</strong> Altura desde donde sale el disparo. </li>
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
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Metodo virtual invocado cuando se desactiva la entidad
		*/
		virtual void onDeactivate();

		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void primaryFire() = 0;

		//__________________________________________________________________
		
		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo secundario.
		*/
		virtual void secondaryFire() = 0;

		//__________________________________________________________________

		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopPrimaryFire() { };

		//__________________________________________________________________

		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		secundario.
		*/
		virtual void stopSecondaryFire() { };


		virtual void resetAmmo() {};
	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/**
		Emite el sonido pasado por parámetro.

		@param soundName Nombre del archivo de audio que queremos reproducir.
		@param loopSound true queremos reproducir el sonido en loop.
		@param play3d true si queremos que el sonido se reproduzca en 3d.
		@param streamSound true si queremos que el sonido se reproduzca en streaming. Util
		para ficheros muy grandes como por ejemplo la música.
		*/
		void emitSound(const std::string &soundName, bool loopSound, bool play3d, bool streamSound);

		//__________________________________________________________________

		/**
		Pinta un decal dada una entidad y una posicion

		@deprecated Temporalmente está implementada como un helper, pero
		en el futuro está función debería formar parte del componente gráfico.

		@param pEntity Entidad sobre la que queremos pintar el decal.
		@param vPos Posición en la que queremos pintar el decal.
		*/
		void drawDecal(Logic::CEntity* pEntity, Vector3 vPos);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		
		/** Enumerado que indica cual es el identificador de arma. */
		WeaponType::Enum _weaponID;

		/** Ruta del sonido del arma. */
		std::string _weaponSound;

		/** Nombre del arma con el formato: weapon + <nombre arma>.*/
		std::string _weaponName;

		CHudWeapons* _hudWeapon;

	}; // class IWeaponFeedback

} // namespace Logic

#endif // __Logic_WeaponFeedback_H
