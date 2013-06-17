package KillerInterface
{
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.external.ExternalInterface;
	
	/**
	 * ...
	 * @author ...
	 */
	public class KillerManager extends MovieClip 
	{
		private var tableBox: Vector.<KilledBox> = new Vector.<KilledBox>();
		private var lastElement: int;
		private var currentElement: int;
		public function KillerManager() 
		{
			lastElement = 0;
			currentElement = 0;
			
			this.addEventListener(Event.ENTER_FRAME, update);
			ExternalInterface.addCallback("addKiller", addKiller);
		}
		
		public function addKiller(kill:String, victim:String) {
			//primero recorremos los que ya hay y los bajamos para abajo
			for each(var killerbox: KilledBox in tableBox) {
				killerbox.y += 30;
			}
			//creamos la nueva killedbox
			var tempKill: KilledBox = new KilledBox(stage.stageWidth / 2, 8);
			
			
			//la seteamos
			tempKill.addkillers(kill, victim);
			
			//si hay mas de 4, borramos la ultima
			if (tableBox.length >= 4) {
				//primero borramos la ultima
				removeChild(tableBox[lastElement]);
				
				//añadimos la nueva
				tableBox[lastElement] = tempKill;
				//la añadimos a la escena
				addChild(tempKill);
				
				lastElement = (lastElement+1) % 4;
			}else {
				addChild(tempKill);
				tableBox.push(tempKill);
			}
		}
		
		public function update(e:Event) {
			for each(var killerbox: KilledBox in tableBox) {
				if (killerbox.alpha <= 0) {
					tableBox.splice(tableBox.indexOf(killerbox), 1);
				}
			}
		}
		
	}

}