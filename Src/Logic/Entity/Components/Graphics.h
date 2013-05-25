/**
@file Graphics.h

Contiene la declaración del componente que controla la representación
gráfica de la entidad.

@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_Graphics_H
#define __Logic_Graphics_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CEntity;
	class CGlowingEntity;
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
	class CGraphics : public IComponent
	{
		DEC_FACTORY(CGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CGraphics() : _graphicsEntity(NULL) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CGraphics();
		
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
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		void setTransform(const Matrix4& transform);

		void setVisible(bool renderGraphicEntity);

		virtual void changeMaterial(const std::string& materialName);

		virtual void onTick(unsigned int msecs);

		std::string getMeshName(){return _model;}

		/**
		Método que se encarga de cambiar la escala de la entidad gráfica.

		@param newScale escala nueva.
		*/
		void changeScale(Vector3 newScale);

	protected:

		/**
		Método virtual que construye la entidad gráfica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gráficas (animadas, etc.).
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);

		/**
		Atributo con el nombre del modelo gráfico de la entidad.
		*/
		std::string _model;
		
		/**
		Nombre del material de la entidad
		*/
		std::string _materialName;

		/**
		Entidad gráfica.
		*/
		Graphics::CEntity *_graphicsEntity;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		Graphics::CScene* _scene;

	}; // class CGraphics

	REG_FACTORY(CGraphics);

} // namespace Logic

#endif // __Logic_Graphics_H
