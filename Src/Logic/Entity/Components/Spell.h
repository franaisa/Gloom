//---------------------------------------------------------------------------
// Spell.h
//---------------------------------------------------------------------------

/**
@file Spell.h

Contiene la declaración de la interfaz común
a todas las armas.

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_Spell_H
#define __Logic_Spell_H

#include "Logic/Entity/Component.h"
#include "SpellType.h"

#include <string>

namespace Logic {

	/**
	@ingroup logicGroup

	Interfaz común a todas las armas del juego. El objetivo
	de esta clase es asegurarse de que todas siguen una interfaz
	común y sencilla que facilite la implementación y la comprensión
	de las armas.

	Debido a que el funcionamiento de cada arma es realmente 
	único, hemos preferido evitar generalizar en esta clase.

	IMPORTANTE: Decrementar la munición y emitir los sonidos
	de disparo es responsabilidad del cliente. Las animaciones
	de las armas tambien son responsabilidad del cliente.

	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class ISpell : public IComponent {
	public:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/**
		Constructor parametrizado. Las clases hijas deberán llamar a este constructor
		con el nombre de su arma.

		@param spellName Nombre del arma implementada por la clase derivada.
		*/
		ISpell(const std::string& spellName);

		//__________________________________________________________________

		/** Destructor. */
		virtual ~ISpell();


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

		
		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell(){ };

		//__________________________________________________________________
		
		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopSpell() { /* Por defecto nada */ }

		//__________________________________________________________________

	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Llamado cuando el arma pasa a ser activa.
		En los hechizos passivos aqui hara su accion.
		*/
		virtual void onAvailable();


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/**
		Helper para emitir un sonido dada una ruta.

		@param ruta Ruta del sonido que queremos reproducir.
		@param sound String para identificar el sonido??.
		@param notIfPlay ?¿?
		*/
		void emitSound(const std::string &ruta, const std::string &sound, bool notIfPlay = false);

		//__________________________________________________________________

	
		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Radio de la cápsula del personaje */
		float _capsuleRadius;
		
		/** Altura desde la que sale el disparo. */
		float _heightShoot;
		
		/** Nombre del arma con el formato: weapon + <nombre arma>.*/
		std::string _spellName;

		unsigned int _spellID;

		/** variable que controlara si es conjuro primario o secundario. */
		bool _isPrimarySpell;

		/** Variable para controlar si un hechizo es activo o pasivo */
		bool _isPassive;

	}; // class ISpell

} // namespace Logic

#endif // __Logic_Spell_H
