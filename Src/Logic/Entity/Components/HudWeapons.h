/**
@file Graphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica del jugador, es decir, todas las armas que este portara.

@see Logic::CGraphics
@see Logic::IComponent

@author Antonio Narv�ez
@date Enero, 2013
*/
#ifndef __Logic_HudWeapons_H
#define __Logic_HudWeapons_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics {
	class CEntity;
	class CScene;
	class COverlay;
}

//declaraci�n de la clase
namespace Logic {

	/**
	Componente que se encarga de la representaci�n gr�fica de una entidad.
	En concreto se encarga de las entidades con representaciones gr�ficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan �ste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de cambio de posici�n y orientaci�n (matriz de 
	transformaci�n) mediante el mensaje SET_TRANSFORM.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
	*/

	class CHudWeapons : public IComponent {
		DEC_FACTORY(CHudWeapons);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CHudWeapons();

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CHudWeapons();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deber� cargar e invoca al m�todo virtual
		createGraphicsEntity() para generar la entidad gr�fica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void onDeactivate();

		/**
		Metodo para cambiar el grafico en funcion del arma actual
		*/
		void changeWeapon(int newWeapon);

		/**
		Metodo que controla el movimiento del arma,
		el comentario te lo dejo a ti fran :D
		*/
		void movement(unsigned int msecs);

	protected:

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Lo uso para cambiar los valores del debug

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onFixedTick(unsigned int msecs);

		/**
		arma actual equipada
		*/
		int _currentWeapon;
		
		/**
		Estructura donde se guardara el offset y las modificaciones en el arma
		*/
		struct TGraphicsWeapon{
			Graphics::CEntity *graphicsEntity;
			Vector3 offset;
			float yaw;
			float pitch;
			float roll;
		};

		/**
		Entidades gr�fica.
		*/
		TGraphicsWeapon *_graphicsEntities;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		//Graphics::SceneNode* _scene;
		Graphics::CScene* _scene;

		float _currentHeight;
		float _verticalSpeed;

		struct RunAnim {
			float currentHorizontalPos;
			float horizontalSpeed;
			float horizontalOffset;
		};
		
		RunAnim _runAnim;

		//////////////////////Gestion de armas
		Graphics::COverlay *_overlayWeapon3D[5];
	}; // class CGraphics

	REG_FACTORY(CHudWeapons);

} // namespace Logic

#endif // __Logic_Graphics_H
