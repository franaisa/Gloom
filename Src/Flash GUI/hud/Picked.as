package hud 
{
	import flash.display.MovieClip;
	import flash.events.Event;
	
	/**
	 * ...
	 * @author ...
	 */
	public class Picked extends MovieClip 
	{
		private var frames:int;
		public function Picked() 
		{
			this.alpha = 0;
			addEventListener(Event.ENTER_FRAME, update);
		}
		
		public function picked(weapon:String): void {
			
			this.weapon.text = weapon;
			frames = 100;
			alpha = 1;
		}
		
		private function update(e: Event) {
			if (this.alpha < 0.01) {
				return;
			}
			if (frames < 50) {
				alpha = frames / 50;
			}
			frames--;
		}
		
	}

}