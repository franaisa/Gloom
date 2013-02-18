#ifndef __Logic_Shoot_H
#define __Logic_Shoot_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la capacidad de disparo de una entidad, asi como las armas que puede tener. Procesa mensajes de tipo 
	SHOOT (indican que la entidad ha disparado)
	<p>
	Poseera una vector, con las armas que tiene, asi como su daño y alcance.
	Tambien poseera un vector de booleanos que indicara las armas que posee.

*/


	class CShoot : public IComponent
	{
		//DEC_FACTORY(CShoot);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CShoot(const std::string &shoot) : IComponent(),  _timeSinceLastShoot(0),_capsuleRadius(3.0f),_canShoot(true), _coldDownTime(0), _temporal(0), _nameWeapon(shoot), _currentAmmo(0)  {}
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo SHOOT (por ahora).
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo SHOOT la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

		virtual void shoot();

		virtual void activate();

		virtual void addAmmo(int weapon, int ammo, bool iAmCatch);

		// Metodo que decrementa la municion del arma. Es virtual porque existen armas como el
		// hammer que no tienen municion (y por lo tanto no siguen la pauta habitual).
		virtual void decrementAmmo();

		// Metodo estatico que resetea la cantidad de municion del arma
		void resetAmmo();

		virtual void tick(unsigned int msecs);

		// Metodo estatico que implementa el patron template
		void shoot2();

		// Metodo que realmente realiza el disparo
		virtual CEntity* fireWeapon();

		// Metodo que se encarga de mandar los mensajes que correspondan cuando se hace hit
		virtual void triggerHitMessages(CEntity* entityHit);

	protected:

		void drawRaycast(const Ray& raycast);

		/**
		caracteristicas de armas
		*/
		std::string _name;
		int _heightShoot;
		unsigned char _damage;
		unsigned char _damageReflect;
		float _dispersion;
		float _distance;
		unsigned char _numberShoots;
		unsigned char _coldDown;
		int _maxAmmo; //esta como int, por que si es -1 esq no tienes esta arma.
		int _currentAmmo;
		int _id;

		std::string _nameWeapon;
		
		/**
		Contendra el radio de la capsula leido del mapa
		*/
		float _capsuleRadius;

		/**
		Para comprobar que un arma esta disparando y no puede disparar 
		"tan rapido como puedas arreglar el gatillo" (Marcus, Borderlands 2)
		*/
		bool _canShoot;
		int _coldDownTime;

		/**
		mide el tiempo de cooldown
		*/
		float *_timeSinceLastShoot;


		/**
			Variable de debug, se usa para darle un nombre unico a cada raycast.
		*/
		int _temporal;
	}; // class CShoot

	//REG_FACTORY(CShoot);

} // namespace Logic

#endif // __Logic_Shoot_H
