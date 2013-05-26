/**
@file ShootMiniGun.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShootMiniGun
@see Logic::CShootRaycast
*/

#include "ShootMiniGun.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageHudDispersion.h"



namespace Logic {
	IMP_FACTORY(CShootMiniGun);
	
	CShootMiniGun::~CShootMiniGun() {
		// Nada que hacer
	}

	void CShootMiniGun::process(const std::shared_ptr<CMessage>& message) {
		ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				
				if(type==Control::LEFT_CLICK) {
					_pressThenShoot=true;
					_bLeftClicked = true;
				}
				else if(type==Control::UNLEFT_CLICK) {
					_pressThenShoot=false;
					_bLeftClicked = false;
					_iContadorLeftClicked = 0;
					
					//Envío el mensaje con valores para que resetee la mirilla
					auto m = std::make_shared<CMessageHudDispersion>();
					m->setTime(0);
					m->setReset(true);
					_entity->emitMessage(m);

					_bMensajeDispMandado = false;
					std::cout << "cambio mensajo dispmandado " << _bMensajeDispMandado << std::endl;
				}
				else if(type==Control::RIGHT_CLICK) {
					_acumulando = true;
				}
				else if(type==Control::UNRIGHT_CLICK) {
					_iRafagas = _contador / 10;
					_acumulando = false;
					_contador = 0;
				}
				
				break;
			}
		}
	} // process
	//__________________________________________________________________

	void CShootMiniGun::onTick(unsigned int msecs) 
	{
	}	
	//__________________________________________________________________

	void CShootMiniGun::onFixedTick(unsigned int msecs) 
	{
		//std::cout << "fixed" << std::endl;
		if (_bLeftClicked)
		{
			_iContadorLeftClicked++;

			//std::cout << "Fixed = " << _iContadorLeftClicked << " y envio = " << _bMensajeDispMandado << std::endl;
			
			//Modificar la dispersión
			if ((_iContadorLeftClicked < 10) && (!_bMensajeDispMandado))
			{
				_dispersion = _dispersionOriginal + 15.0f;
				//Enviamos el mensaje para que empiece a modificar la mirilla con la dispersión
				std::shared_ptr<CMessageHudDispersion> m = std::make_shared<CMessageHudDispersion>();
				m->setHeight(8.0f);
				m->setWidth(9.0f);
				m->setTime(2500);//Tiempo máximo que bajará el tamaño de la mirilla
				m->setReset(false);
				_entity->emitMessage(m);
				_bMensajeDispMandado = true;
				printf("\nReduciendo mira");
				/**
				NOTA: De momento tiene el bug de que si disparas cuando no tienes munición, sigue haciendo la dispersión.
				La movida es que se sabe si tienes munición o no en el método primaryShoot, de su padre ShootRaycast.
				Habría que hacer que este método fuese un booleano y devolviese true si ha podido disparar y false si no.
				De momento no lo cambio porque creo que se va a cambiar la forma de gestionar estos raycast más adelante,
				pero hay que tenerlo en cuenta (también se tiene que tener en cuenta para cuando se ponga la animación
				de vibración de la minigun).
				*/
			}			
			else if (_iContadorLeftClicked < 20)
			{
				_dispersion = _dispersionOriginal + 5.0f;
			}
			else
			{
				_dispersion = _dispersionOriginal;
			}
		}


		if(_primaryCooldownTimer < _primaryCooldown)
		{
			_primaryCooldownTimer += msecs;
		}
		else
		{
			if(_pressThenShoot){
				_primaryCanShoot=true;				
				primaryShoot();
			}
		}

		//Comprobamos la funcionalidad del botón derecho
		if (_acumulando)
		{
			//Si tenemos el botón derecho pulsado, seguimos aumentando el contador
			_contador++;
		}
		else
		{
			//No tenemos pulsado el derecho, así que comprobamos si tenemos rafagas que lanzar
			if (_iRafagas > 0)
			{
				//Controlo que no se tengan más ráfagas del máximo (en su caso lo seteo a este valor)
				if (_iRafagas > _iMaxRafagas)
				{
					_iRafagas = _iMaxRafagas;
				}

				secondaryShoot(_iRafagas);
				_iRafagas = 0;

			}
		}
	}

} // namespace Logic

