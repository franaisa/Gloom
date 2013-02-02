#ifndef __Logic_ShootMiniGun_H
#define __Logic_ShootMiniGun_H

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


	class CShootMiniGun : public IComponent
	{
		DEC_FACTORY(CShootMiniGun);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CShootMiniGun() : IComponent(),  _timeSinceLastShoot(0),_weapon(),_capsuleRadius(3.0f), _shooting(false), _temporal(0)  {}
		

	
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

		virtual void desactivate();

		void shoot();

	protected:

		
		struct TWeapon{
			std::string name;
			unsigned char damage;
			float dispersion;
			float distance;
			unsigned char numberShoots;
			unsigned char coolDown;
			int ammo; //esta como int, por que si es -1 esq no tienes esta arma.
		};


		/**
		Contendra el radio de la capsula leido del mapa
		*/
		float _capsuleRadius;

		/**
		vector con caracteristicas de armas
		*/
		TWeapon _weapon;

		/**
		Para comprobar que un arma esta disparando y no puede disparar 
		"tan rapido como puedas arreglar el gatillo" (Marcus, Borderlands 2)
		*/
		bool _shooting;

		/**
		mide el tiempo de cooldown
		*/
		float *_timeSinceLastShoot;


		/**
			Variable de debug.
		*/
		int _temporal;
	}; // class CShoot

	REG_FACTORY(CShootMiniGun);

} // namespace Logic

#endif // __Logic_Shoot_H
