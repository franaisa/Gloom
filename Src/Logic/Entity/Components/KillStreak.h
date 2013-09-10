/**
@file Elevator.h

Contiene la declaración del componente que controla el componente de los kill streak

@see Logic::CKillStreak
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Agosto, 2013
*/
#ifndef __Logic_KillStreak_H
#define __Logic_KillStreak_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la gestión de los kill streak.
	
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Agosto, 2013
*/


	/* EXPLICACIÓN KILL STREAKS

	===Killing Streak (V2)===

		*Hound
		x3: Aumento de al velocidad de movimiento.
		x5: Aumento del daño y absorcion de vida del "bocao".
		x10: Aumento de la duración de berserker y reducciónd e su CD.
		x20: Aura de velocidad de movimiento. Posibilidad de "esquivar" las balas de un X% (las balas se desvian o els atraviesan sin causar daño)

		*Archangel
		x3: Disminución considerable de la reducción de vida base.
		x5: Aumento de la duración de la inmunidad.
		x10: Aumento de la cantidad de vida que cura (a sí mismo y a los aliados). Aumento del área de acción de la curación.
		x20: Zona inmune. Crea un área (escudo) donde no pueden atravesar los proyectiles (o ataques melee). Si pueden atravesarlo cualquier jugador (aliado o enemigo).

		*Shadow
		x3: Aumento del daño con la melee.
		x5: Aumento de la duración de la invisibilidad.
		x10: Aumento de la duración de la cegera. Se incrementa el daño que reciben los objetivos cegados.
		x20: Aura de invisiblidad. Sólo cuando un shadow es invisible, tambn lo son sus compañeros en un pequeño radio.

		*Screamer
		x3: Aumento de la vida.
		x5: Aumento de la cantidad de daño absorbida.
		x10: Aumento del daño, área y fuerza del empujón.
		x20: Aura de fuerza. Los aliados cercanos ganan la habilidad de empujar en sus armas (ligeramente).

		A nivel 20 de KS se gana "inestabilidad" (de tanto consumir almas) El jugador atrae items pasivamente en un área. Si un jugador muere explotará en un area grande, haciendo daño a aliados y enemigos e incluso eliminando los items (hasta que respawneen)

		Todos los niveles de KS deberían representarse gráficamente. El numero de muertes (x3, x5, x10, x20) es variable.

		===Formas inestables (V1)===
		(Representación gráfica con KS x20)

		*Hound
		Forma animal: 
		-Mas grande y musculado (solo requiere deformación de la malla).
		-Peludo (cambio de textura).
		-Con cuernos/pinchos (más complejo, se peude hacer desde el modelo original o añadiendole nuevas mallas que sean hijas de los huesos desde el ogre).
		-Fuego naranja de los ojos (partículas).
		-Genera continuamente sonidos de animal/demonio.

		*Archangel
		Forma celestial:
		-Levita una animación más para las piernas).
		-Se convierte en un ser de luz (cambio de texturas, shader glow "exagerado", halo y posiblemente partículas) Que moleste a la vista, que casi ciegue.
		-Alas de energía (por aprtículas, q es caro, o con dos planos con texturas con glow/additive).

		*Shadow
		Forma oscura:
		-Semitransparente (cambio del alfa de las texturas)
		-Humo negro girando a su alrededor (partículas).
		-Oscurece la vision (shader) Igual que el arcángel te ciega con la luz, la pantalla se oscurece si se apunta directamente al shadow.

		*Screamer
		Forma caótica:
		-Ondas envolventes, de la concentración de ondas sonoras el aire se enturbia violéntamente a su alrededor.
		-Venas negras por todo el cuerpo,especialmente la boca (cambiar textura).
		-Decalls de grietas y roturas a su alrededor, por las ondas.
		-Todos los jugadores pueden oir susurros, pasos y gritos continuamente (así molesta a la percepción)
	*/

	class CKillStreak : public IComponent
	{
		DEC_FACTORY(CKillStreak);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CKillStreak() : IComponent(){
						_iKillsLevel1 = 3;
						_iKillsLevel2 = 5;
						_iKillsLevel3 = 10;
						_iKillsLevel4 = 20;
					}

		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void onActivate();

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		/**
		Método llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajará cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el último tick. Son siempre constantes.
		*/
		virtual void onFixedTick							(unsigned int msecs);

		//Función que se llama cuando llega un mensaje de kill streak
		void dead											(const std::shared_ptr<CMessage>& message);

		//Función que setea el nivel en función del número de muertes
		void SetLevel										();

		//Función que lanza el kill streak correspondiente a la clase que sea la entidad
		void LaunchClassKillStreak							();

		void ArchangelKillStreak							();



	private:

		//Contador de muertes que lleva el jugador
		int _iContKills;		

		//Muertes límite que hay que hacer para subir de nivel. Se setean desde fichero
		int _iKillsLevel1, _iKillsLevel2, _iKillsLevel3, _iKillsLevel4;

		//Nivel de killstreak del jugador
		int _iLevel;

	}; // class CElevator

	REG_FACTORY(CKillStreak);

} // namespace Logic

#endif // __Logic_Elevator_H
