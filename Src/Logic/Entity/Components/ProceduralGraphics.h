/**
@file ProceduralGraphics.h

Contiene la implementación del componente que controla las animaciones
procedurales que se aplican a la entidad
 
@see Logic::ProceduralGraphics
@see Logic::IComponent

@author Rubén Mulero Guerrero
@date June, 2013
*/
#ifndef __Logic_ProceduralGraphics_H
#define __Logic_ProceduralGraphics_H

#include "Logic/Entity/Component.h"
#include "Graphics/AnimatedEntity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CAnimatedEntity;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representación gráfica animada de una 
	entidad. Este componente no se encarga de cargar animaciones ni de 
	quitarlas, sino que se encarga del movimiento de huesos de manera 
	procedural para, por ejemplo, hacer que la cabeza y los brazos del
	personaje apunten en la dirección en la que éste está apuntando
	
    @ingroup logicGroup

	@author Rubén Mulero Guerrero
	@date June, 2013
*/
	class CProceduralGraphics : public IComponent, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CProceduralGraphics);
	public:
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CProceduralGraphics();

		//________________________________________________________________________

		/** Destructor por defecto. */
		virtual ~CProceduralGraphics();


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
			<li><strong>bone:</strong> El nombre del hueso que estamos animando. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea los valores de inercia y desplazamiento.
		*/
		virtual void onActivate();

		/**
		Método que desactiva el componente.

		@return true si todo ha ido correctamente.
		*/
		virtual void onDeactivate();

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
			<li>ADDFORCEPLAYER</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		// =======================================================================
		//             MÉTODOS HEREDADOS DE CANIMATEDENTITYLISTENER
		// =======================================================================
		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		void animationFinished(const std::string &animation);

	protected:
		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick. Son siempre
		constantes.
		*/
		virtual void onTick(unsigned int msecs);

	private:

		/**
		Entidad gráfica animada.
		*/
		Graphics::CAnimatedEntity *_animatedEntity;

		/** Nombre del hueso que vamos a mover. */
		std::string _masterBoneName;

		/** Cota superior de pitch. */
		float _upperPitchBound;

		/** Cota inferior de pitch. */
		float _lowerPitchBound;

		/** Cota superior de yaw. */
		float _upperYawBound;

		/** Cota inferior de yaw. */
		float _lowerYawBound;

		bool _insertAnimation;

	}; // class CAnimatedGraphics

	REG_FACTORY(CProceduralGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H