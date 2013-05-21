//---------------------------------------------------------------------------
// ShootRaycast.h
//---------------------------------------------------------------------------

/**
@file ShootRaycast.h

Contiene la declaración del componente que abstrae la implementación del 
disparo por raycast.

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_ShootRaycast_H
#define __Logic_ShootRaycast_H

#include "Logic/Entity/Components/Shoot.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Componente que implementa de forma general el comportamiento de aquellas
	armas que se basan en el uso de raycast. Contiene métodos útiles como
	el dibujado de raycast (para debuggear) y el disparo de raycast basado
	en arquetipos.

	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CShootRaycast : public CShoot {
		//DEC_FACTORY(CShootRaycast); -- Esta clase es abstracta y por lo tanto no instanciable
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShootRaycast() : CShoot(), _temporal(0) { }

		virtual ~CShootRaycast();

		//__________________________________________________________________

		/**
		Constructor parametrizado

		@param shoot Nombre del arma que vamos a inicializar.
		*/
		CShootRaycast(const std::string &shoot) : CShoot(shoot), _temporal(0) { }


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
			<li><strong>(weaponName)+Dispersion:</strong> Dispersion del arma. </li>
			<li><strong>(weaponName)+Distance:</strong> Distancia que alcanza el arma. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		
		// =======================================================================
		//                    METODOS HEREDADOS DE CSHOOT
		// =======================================================================


		/**
		Debido a que todos los disparos por raycast son realmente similares, este
		método implementa el patrón template, siendo únicamente necesario redefinir
		el método "triggerHitMessages".

		Notar, que la función shoot no se ha hecho estática en CShoot porque los
		proyectiles y los raycast tienen comportamientos muy distintos.
		*/
		virtual void primaryShoot();

		/**
		Los disparos secundarios si fieren bastante unos de otros, por ahora se ha hecho un metodo vacio,
		para poder compilar por ahora, pero todos tendran que ser redefinidos.
		*/
		virtual void secondaryShoot();

		
		// =======================================================================
		//                          METODOS PROPIOS
		// =======================================================================


		/**
		Método que se encarga de realizar el disparo del raycast por fisicas.

		Notar que este método si es estático porque el disparo mediante raycast 
		está totalmente parametrizado en los arquetipos, y por lo tanto, todas las
		armas pueden seguir el mismo patrón de disparo.

		@return Un puntero a la entidad con la que se ha colisionado (o NULL si no
		ha habido colisión) y el rayo que se lanzó para comprobar dicha colisión.
		*/
		virtual std::pair<CEntity*, Ray> fireWeapon();

		//__________________________________________________________________

		/**
		Método que se encarga de mandar los mensajes que corresopondan a la entidad
		que se ha golpeado en caso de hacer hit.

		A pesar de que se trata de un método virtual puro, esta clase ofrece una
		implementación por defecto que será útil para la mayoría de las subclases. 

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit) = 0;

	protected:


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/** 
		Dibuja un rayo en Ogre. Útil para debuggear las armas que disparan mediante
		raycast.

		@param raycast Raycast a dibujar.
		*/
		void drawRaycast(const Ray& raycast);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Daño que el arma refleja. */
		unsigned char _damageReflect;

		/** Dispersión del arma. */
		float _dispersion;

		/** Dispersión original del arma. Variable auxiliar para guardar la referencia leída del mapa.*/
		float _dispersionOriginal;

		/** Distancia de alcance del arma. */
		float _distance;
		
		/** Variable de debug, se usa para darle un nombre unico a cada raycast. */
		int _temporal;

		/** Ruta del sonido de no munición */
		std::string _noAmmo;
		
		/** Ruta del sonido de disparo */
		std::string _audioShoot;



	};

	//REG_FACTORY(CShootRaycast);

} // namespace Logic

#endif // __Logic_Shoot_H
