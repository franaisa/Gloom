/**
@file DynamicLight.h

Contiene la declaración del componente 
que encapsula el manejo de luces que siguen
a la entidad.

@see Logic::CDynamicLight
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#ifndef __Logic_DynamicLight_H
#define __Logic_DynamicLight_H

#include "Logic/Entity/Component.h"
#include "Light.h"

namespace Logic {

	/**
	Componente que encapsula el uso de luces y además toma constantemente
	la posición y orientación de la entidad a la que pertenece.
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Julio, 2013
	*/

	class CDynamicLight : public CLight {
		DEC_FACTORY(CDynamicLight);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CDynamicLight();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CDynamicLight();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		No acepta ningún tipo de mensajes. Sobreescrito para evitar el comportamiento
		de la clase padre.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message) { return false; }


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Crea una luz con los parámetros leidos de mapa o establecidos manualmente.

		@param offset Offset que queremos que la luz tenga sobre la posición de la
		entidad.
		@param timeToLive Indica el tiempo que queremos que la luz esté encendida.
		Si el valor es 0 se asume que no queremos apagar la luz (por lo menos de forma
		automática con un contador).
		*/
		void turnOn(const Vector3& offset = Vector3::ZERO, float timeToLive = 0.0f);


	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================
		

		/**
		Actualiza la posición y orientación de la luz constantemente.
		*/
		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Controla el tiempo que la luz está encendida. */
		float _timer;
		
		/** Offset sobre la posición de la entidad. */
		Vector3 _offset;
	}; // class CDynamicLight

	REG_FACTORY(CDynamicLight);

} // namespace Logic

#endif // __Logic_Light_H
