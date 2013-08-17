/**
@file Physics.h

Contiene la declaración de una clase abstracta de la que deben heredar todos los
componentes físicos.

@see Logic::IComponent
@see Logic::CPhysicEntity
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_Physics_H
#define __Logic_Physics_H
//
#include "Logic/Entity/Component.h"
#include <iostream>

// Predeclaración de tipos
namespace physx {
	struct PxTriggerPair;
};

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	/**
	Clase abstracta de la que deben heredar todos los componentes físicos. Proporciona un interfaz
	común para recibir eventos desde el motor de física.
	
    @ingroup logicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class IPhysics : public IComponent {
	public:

		IPhysics() : _collidersHaveName(false) {
			// Nada que hacer
		}
 
		// Te la dedico Ruben!
		class IObserver {
		public:
			virtual void onTrigger(IPhysics* otherComponent, bool enter) { }
			virtual void onContact(IPhysics* otherComponent, bool enter) { }
			virtual void onShapeHit(IPhysics* otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) { }
		};

		// Ojito con añadir dos veces un mismo listener, que no hago comprobaciones!!
		void addObserver(IObserver* observer) { 
			_observers.push_back(observer);
		}

		// Busqueda lineal para eliminar el observador
		void removeObserver(IObserver* observer) { 
			for(auto it = _observers.begin(); it != _observers.end(); ++it) {
				if(*it == observer) {
					_observers.erase(it);
					break;
				}
			}
		}

		virtual std::string getPhysicName() = 0;

		/**
		Este método es invocado desde el motor de física cuando una entidad entra o sale de un
		trigger físico. Se notifica tanto al componente asociado al trigger como al componente
		asociado a la otra entidad.

		@param otherComponent Componente asociado al trigger o a la otra entidad, dependiendo
		       de a quién se esté notificando.
		@param enter True si la entidad entra en el trigger y false si sale. 
		*/
		virtual void onTrigger(IPhysics *otherComponent, bool enter) = 0;

		/**
		Este método es invocado desde el motor de física cuando una entidad golpea a otra. Para
		comprobar exclusivamente si un controller a golpeado a otra cosa es mejor usar otros
		metodos de physX.

		Para mejorar la eficiencia de esta query, physX permite implementar filtros, de manera
		que cada actor fisico pueda indicar de que cosas quiere ser notificado (pudiendo eliminar
		onContact por ejemplo, en algunas entidades).

		@param otherComponent Componente asociado a la otra entidad que nos ha golpeado.
		*/
		virtual void onContact(IPhysics *otherComponent, bool enter) = 0;

		// Me he sacado el on shape hit aqui para evitar la ñapa de jose, pero aun asi, no deberia
		// estar aqui (o al menos eso me dice la intuicion). Quizas la interfaz Iphysics es demasiado
		// generalista.
		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) = 0;

		bool collidersHaveName() { return _collidersHaveName; }

	protected:

		std::list<IObserver*> _observers;
		bool _collidersHaveName;

	}; // class IPhysics

} // namespace Logic

#endif // __Logic_Physics_H
