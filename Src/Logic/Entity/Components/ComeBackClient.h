//---------------------------------------------------------------------------
// ComeBackClient.h
//---------------------------------------------------------------------------

/**
@file ComeBackClient.h

Contiene la declaración del componente que implementa el gravity.

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ComeBackClient_H
#define __Logic_ComeBackClient_H

#include "Logic/Entity/Components/Spell.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del gravity. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit, ya que 
	el resto del comportamiento esta definido en el arquetipo que describe 
	al gravity.

	@author Jose Antonio García Yáñez
	@date Febrero, 2013
	*/
	class CPhysicController;

	class CComeBackClient : public ISpell {
		DEC_FACTORY(CComeBackClient);

	public:

		/** Constructor por defecto. */
		CComeBackClient() : ISpell("comeBack"), _ComeBackClientPosition(Vector3::ZERO), _ComeBackClientOrientation(Quaternion::ZERO) { }

		//__________________________________________________________________

		virtual ~CComeBackClient();

		//__________________________________________________________________

		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>spellGravityDamageReflect:</strong> Daño que hace el gravity al golpear contra el mundo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopSpell();

	private:
		
		CPhysicController *_physicController;

		Vector3 _ComeBackClientPosition;
		Quaternion _ComeBackClientOrientation;
	}; // class CShootRaycast

	REG_FACTORY(CComeBackClient);

} // namespace Logic

#endif // __Logic__H
