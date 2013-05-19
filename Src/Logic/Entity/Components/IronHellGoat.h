//---------------------------------------------------------------------------
// IronHellGoat.h
//---------------------------------------------------------------------------

/**
@file IronHellGoat.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CIronHellGoat
@see Logic::CShootProjectile

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_IronHellGoat_H
#define __Logic_IronHellGoat_H

#include "Logic/Entity/Components/ShootProjectile.h"

#include <set>

namespace Logic {
	class CFireBallController;
}

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CIronHellGoat : public CShootProjectile {
		DEC_FACTORY(CIronHellGoat);

	public:

		/** Constructor por defecto. */
		CIronHellGoat();

		//__________________________________________________________________

		virtual ~CIronHellGoat();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		// El propio shoot deberia encargarse de llamar a lo que toque teniendo
		// en cuenta la municion y las demas historias
		virtual void primaryShoot();

		//__________________________________________________________________

		virtual void stopPrimaryShoot();

		//__________________________________________________________________

		virtual void secondaryShoot();

		//__________________________________________________________________

		virtual void stopSecondaryShoot();

		//__________________________________________________________________

		virtual void fireWeapon() { /* Esto esta deprecado */ }

		//__________________________________________________________________

		void removeFireBall(CFireBallController* fireBall);

		virtual void resetAmmo();

	protected:

		virtual void onActivate();

		virtual void onAvailable();

		virtual void onTick(unsigned int msecs);

		template <typename T>
		std::string toString(const T& data) const {
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << data;
			return ss.str();
		}

		std::string toString(const Vector3& data) const {
			std::stringstream ss (std::stringstream::in | std::stringstream::out);

			ss << data.x;
			ss << " ";
			ss << data.y;
			ss << " ";
			ss << data.z;

			return ss.str();
		}

	private:

		bool _primaryFireIsActive;

		bool _secondaryFireIsActive;

		std::set<CFireBallController*> _controllableFireBalls;

		float _fireBallRadius;
		float _fireBallSpeed;
		float _fireBallExplotionRadius;
		float _fireBallDamage;
	};

	REG_FACTORY(CIronHellGoat);

} // namespace Logic

#endif // __Logic_Shoot_H
