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
		DEC_FACTORY(CShoot);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CShoot() : IComponent(), _temporal(0), _timeSinceLastShoot(NULL),_weapons(NULL),_capsuleRadius(3.0f), _shooting(false), _actualWeapon(0) {}
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo SHOOT (por ahora).
		*/
		virtual bool accept(const TMessage &message);

		/**
		Al recibir un mensaje de tipo SHOOT la vida de la entidad disminuye.
		*/
		virtual void process(const TMessage &message);

		void shoot();

	protected:

		
		struct TWeapon{
			std::string name;
			float damage;
			float dispersion;
			float distance;
			float coolDown;
		};

		


		/**
		Contendra el radio de la capsula leido del mapa
		*/
		float _capsuleRadius;

		/**
		vector con caracteristicas de armas
		*/
		TWeapon *_weapons;

		/**
		Para comprobar que un arma esta disparando y no poder disparar 
		"tan rapido como puedas arreglar el gatillo" (Marcus, Borderlands 2)
		*/
		bool _shooting;

		/**
		arma actual equipada
		*/
		int _actualWeapon;

		/**
		mide el tiempo de cooldown
		*/
		float *_timeSinceLastShoot;

		/**
		esto se eliminara
		*/
		int _temporal;
	}; // class CShoot

	REG_FACTORY(CShoot);

} // namespace Logic

#endif // __Logic_Shoot_H
