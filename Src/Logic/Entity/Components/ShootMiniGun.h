//---------------------------------------------------------------------------
// ShootMiniGun.h
//---------------------------------------------------------------------------

/**
@file ShootMiniGun.h

Contiene la declaración del componente que implementa el arma minigun.

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#ifndef __Logic_ShootMiniGun_H
#define __Logic_ShootMiniGun_H

#include "Logic/Entity/Components/ShootRaycast.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementación por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narváez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CShootMiniGun : public CShootRaycast {
		DEC_FACTORY(CShootMiniGun);

	public:

		/** Constructor por defecto. */
		CShootMiniGun() : CShootRaycast("miniGun"), _pressThenShoot(false), _contador(0), _acumulando(false),
													_iRafagas(0), _bLeftClicked(false), _iContadorLeftClicked(0) 
		{
			_bMensajeDispMandado = false;
		}

		//__________________________________________________________________

		virtual ~CShootMiniGun();

		//__________________________________________________________________

		/**
		Método que se encarga de mandar los mensajes que correspondan a la entidad
		que se ha golpeado en caso de hacer hit.

		Sencillamente reutiliza la implementación por defecto de CShootRaycast.

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit) { CShootRaycast::triggerHitMessages(entityHit); }

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		/**
		Método llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

	private:

		/**
		Namespace para los tipos de mensajes de control posibles.
		*/
		bool _pressThenShoot;

		/**
			Contador para ir acumulando el tiempo/ticks que se tiene el botón derecho apretado
		*/
		int		_contador;

		/**
			Variable booleana que indica si se está acumulando tiempo/ticksporque
			se tiene el botón derecho pulsado
		*/
		bool	_acumulando;

		/**
			Numero de rafagas que se tienen acumuladas.
			Una rafaga = 10 unidades de contador.
		*/
		int		_iRafagas;

		bool	_bLeftClicked;

		int		_iContadorLeftClicked;

		bool	_bMensajeDispMandado;


	}; // class CShootMiniGun

	REG_FACTORY(CShootMiniGun);

} // namespace Logic

#endif // __Logic_Shoot_H
