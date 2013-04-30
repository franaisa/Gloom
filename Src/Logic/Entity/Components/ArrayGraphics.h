/**
@file Graphics.h

Contiene la declaración del componente que controla la representación
gráfica del jugador, es decir, todas las armas que este portara.

@see Logic::CGraphics
@see Logic::IComponent

@author Antonio Narváez
@date Enero, 2013
*/
#ifndef __Logic_ArrayGraphics_H
#define __Logic_ArrayGraphics_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CEntity;
	class CScene;
}

//declaración de la clase
namespace Logic
{
/**
	Componente que se encarga de la representación gráfica de una entidad.
	En concreto se encarga de las entidades con representaciones gráficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan éste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de cambio de posición y orientación (matriz de 
	transformación) mediante el mensaje SET_TRANSFORM.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/
	class CArrayGraphics : public IComponent
	{
		DEC_FACTORY(CArrayGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CArrayGraphics() : IComponent(), _actualWeapon(0), _graphicsEntities(), _numWeapons(0) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CArrayGraphics();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deberá cargar e invoca al método virtual
		createGraphicsEntity() para generar la entidad gráfica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
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
		Aqui se actualizara la funcion, la saco fuera para hacer uso de ella
		*/
		void setTransform(const Matrix4 &transform);

	protected:

		/**
		Método virtual que construye la entidad gráfica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gráficas (animadas, etc.).
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(std::string nombreEntidad, std::string modelo);

		/**
		metodo donde se hara el cambio de arma.
		*/
		


		


		/**
		arma actual equipada
		*/
		int _actualWeapon;
		
		/**
		Estructura donde se guardara el offset y las modificaciones en el arma
		*/
		struct TGraphicsWeapon{
			Graphics::CEntity *_graphicsEntity;
			Vector3 *offset;
			float yaw;
			float pitch;
		};

		/**
		Entidades gráfica.
		*/
		TGraphicsWeapon *_graphicsEntities;

		/**
		Aqui almaceno el numero de aramas (lo mas seguro que esto deba de ser borrado) !!!! cuidado que en el destructor se usa
		*/
		int _numWeapons;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		//Graphics::SceneNode* _scene;
		Graphics::CScene* _scene;

	}; // class CGraphics

	REG_FACTORY(CArrayGraphics);

} // namespace Logic

#endif // __Logic_Graphics_H
