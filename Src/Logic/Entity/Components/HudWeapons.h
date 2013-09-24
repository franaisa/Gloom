/**
@file Graphics.h

Contiene la declaración del componente que controla la representación
gráfica del jugador, es decir, todas las armas que este portara.

@see Logic::CGraphics
@see Logic::IComponent

@author Antonio Narváez
@date Enero, 2013
*/
#ifndef __Logic_HudWeapons_H
#define __Logic_HudWeapons_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"
#include "AvatarController.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics {
	class CEntity;
	class CScene;
	class COverlay;
}

//declaración de la clase
namespace Logic {

	class CAvatarController;

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

	class CHudWeapons : public IComponent, public CAvatarController::IObserver {
		DEC_FACTORY(CHudWeapons);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CHudWeapons();

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CHudWeapons();
		
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

		Graphics::CEntity* getCurrentWeapon();

		Vector3 getCurrentWeaponWorldPos();

		/**
		Metodo para cambiar el grafico en funcion del arma actual
		*/
		void changeWeapon(int newWeapon);

		virtual void onLand();
		virtual void onWalk();
		virtual void onIdle();
		virtual void onAir();

		/**
		Metodo que controla el movimiento del arma,
		el comentario te lo dejo a ti fran :D
		*/
		void walkAnim(unsigned int msecs);

		void changeWeaponAnim(unsigned int msecs);

		void landAnim(unsigned int msecs);

		void idleAnim(unsigned int msecs);

		void shootAnim(float force);

		void fallAnim(unsigned int msecs);

		void rapidShootAnim(unsigned int msecs);

		void continouosShooting(bool state);

		void offsetRecovery(unsigned int msecs);

		void playerIsFalling(bool falling, int direction = 0);
	
		void loadWeaponAnim(unsigned int msecs);

		void loadingWeapon(bool state);

		void linking(bool state) { _linking = state; }

		void linkAnim(unsigned int msecs);

	protected:

		virtual void onStart();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Lo uso para cambiar los valores del debug

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onFixedTick(unsigned int msecs);

		float sineStep(unsigned int msecs, float& currentSinePosition, float offset, float speed, float loBound = 0.0f, float hiBound = 2 * Math::PI);

		/**
		arma actual equipada
		*/
		int _currentWeapon;

		bool _playerIsWalking;
		bool _playerIsLanding;
		bool _loadingWeapon;
		bool _continouslyShooting;
		bool _playerIsFalling;
		bool _changingWeapon;
		bool _linking;

		/**
		Estructura donde se guardara el offset y las modificaciones en el arma
		*/
		struct TGraphicsWeapon{
			Graphics::CEntity *graphicsEntity;

			Vector3 offset;
			Vector3 defaultPos;
			float defaultYaw;
			float defaultPitch;
			float defaultRoll;
		};

		/**
		Entidades gráfica.
		*/
		TGraphicsWeapon *_graphicsEntities;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		//Graphics::SceneNode* _scene;
		Graphics::CScene* _scene;

		//__________________________________________________________________

		struct FallAnim {
			float currentHorizontalPos;
			float horizontalSpeed;
			float horizontalOffset;
			int movementDir;

			Vector3 offset;
		};

		//__________________________________________________________________

		struct RunAnim {
			// Horizontal movement
			float currentHorizontalPos;
			float horizontalSpeed;
			float horizontalOffset;

			float currentVerticalPos;
			float verticalSpeed;
			float verticalOffset;

			int currentStrafingDir;
			int oldStrafingDir;

			Vector3 offset;
		};
		
		//__________________________________________________________________

		struct LandAnim {
			float force;
			float currentOffset;
			float recoverySpeed;

			Vector3 offset;
		};

		//__________________________________________________________________

		struct IdleAnim {
			float currentVerticalPos;
			float verticalSpeed;
			float verticalOffset;

			Vector3 offset;
		};

		//__________________________________________________________________

		struct ShootAnim {
			float shootForce;
			float shootRecoveryCoef;

			Vector3 offset;
		};

		//__________________________________________________________________

		struct UnstableLoadAnim {
			float currentVerticalPos;
			float currentVerticalSpeed;
			float verticalOffset;

			float initVerticalSpeed;
			float maxVerticalSpeed;
			float speedInc;

			float currentNoise;
			float initNoiseSpeed;
			float maxNoiseSpeed;
			float noiseInc;

			Vector3 offset;
		};

		//__________________________________________________________________

		struct RapidShootAnim {
			float shakeOffset;
			float recoveryCoef;

			float currentVerticalPos;
			float verticalSpeed;
			float verticalOffset;

			Vector3 offset;

			int acumFiringTime;
			int firingRate;
		};

		//__________________________________________________________________

		struct ChangeWeaponAnim {
			float sineOffset;
			float xSpeed;
			float x;

			float horizontalSpeed;

			bool takingAway;

			int nextWeapon;

			Vector3 offset;
		};

		struct LinkAnim {
			float sinOffset;
			float xSpeed;
			float x;
			Vector3 offset;
		};

		//__________________________________________________________________

		LinkAnim _linkAnim;
		ChangeWeaponAnim _chgWpnAnim;
		RunAnim _runAnim;
		LandAnim _landAnim;
		IdleAnim _idleAnim;
		ShootAnim _shootAnim;
		UnstableLoadAnim _unstableLoadAnim;
		RapidShootAnim _rapidShootAnim;
		FallAnim _fallAnim;

		//////////////////////Gestion de armas
		//Graphics::COverlay *_overlayWeapon3D[WeaponType::eSIZE];

		CAvatarController* _avatarController;

		float _threePiQuarters;

		Quaternion _halfPi;
	}; // class CGraphics

	REG_FACTORY(CHudWeapons);

} // namespace Logic

#endif // __Logic_Graphics_H
