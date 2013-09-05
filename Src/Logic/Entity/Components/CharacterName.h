/**
@file CharacterName.h

@see Logic::CCharacterName
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Logic_CharacterName_H
#define __Logic_CharacterName_H

#include "Logic/Entity/Component.h"

namespace Graphics {
	class CMovable2dText;
}

namespace Logic {

	/**

    @ingroup logicGroup

	@author Francisco Aisa García
	@date Agosto, 2013
	*/

	class CCharacterName : public IComponent {
		DEC_FACTORY(CCharacterName);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CCharacterName();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CCharacterName();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>lightType:</strong> Tipo de luz, focal, puntual o direccional. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		void setVisible(bool isVisible);

		bool isVisible();

	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================
		

		/**
		Dependiendo de si en el mapa se ha especificado que la luz este encendida o
		apagada, al ejecutarse este método se enciende la luz o se deja apagada.
		*/
		virtual void onStart();

		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		Graphics::CMovable2dText* _text2d;

		float _size;
		Vector3 _offset;
		Vector3 _color;
		std::string _font;

	}; // class CCharacterName

	REG_FACTORY(CCharacterName);

} // namespace Logic

#endif // __Logic_CharacterName_H
