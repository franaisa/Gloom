//---------------------------------------------------------------------------
// IronHellGoat.h
//---------------------------------------------------------------------------

/**
@file IronHellGoat.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CIronHellGoat
@see Logic::IWeapon

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_IronHellGoat_H
#define __Logic_IronHellGoat_H

#include "Logic/Entity/Components/Weapon.h"

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

	class CIronHellGoat : public IWeapon {
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
		virtual void primaryFire();

		//__________________________________________________________________

		virtual void stopPrimaryFire(unsigned int elapsedTime);

		//__________________________________________________________________

		virtual void secondaryFire() { /* No necesito hacer nada aunque redefino */ }

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

		std::string _shootAudio;

		std::set<CFireBallController*> _controllableFireBalls;

		unsigned int _elapsedTime;
		unsigned int _maxLoadingTime;
		unsigned int _ammoSpentTimeStep;
		unsigned int _ammoSpentTimer;
		unsigned int _currentSpentAmmo;

		float _defaultFireBallRadius;
		float _defaultFireBallSpeed;
		float _defaultFireBallExplotionRadius;
		float _defaultFireBallDamage;

		unsigned int _maxAmmoPerShot;
		float _fireBallRadiusTemporalIncrement;
		float _fireBallSpeedTemporalIncrement;
		float _fireBallExplotionRadiusTemporalIncrement;
		float _fireBallDamageTemporalIncrement;
	};

	REG_FACTORY(CIronHellGoat);

} // namespace Logic

#endif // __Logic_Shoot_H
