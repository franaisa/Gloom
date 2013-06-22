package hud 
{
	import flash.display.MovieClip;
	import flash.events.Event;
	
	/**
	 * ...
	 * @author ...
	 */
	public class SkillCoolDown extends MovieClip 
	{
		
		private var _speed:Number;
		private var _acumSpeed:Number;
		public function SkillCoolDown() 
		{
			_speed = 1.0;
			_acumSpeed = 0;
			gotoAndPlay("ready");
		}
		
		private function onUpdate(e:Event) {
			if (currentFrameLabel == "ready") {
				
				this.removeEventListener(Event.ENTER_FRAME, onUpdate);
				play();
				return;
			}
			
			_acumSpeed += _speed;
			if ( _acumSpeed > 1 ) {
				nextFrame();
				_acumSpeed -= 1.0;
			}
			
		}
		
		public function set speed(speed: Number): void {
			_speed = speed;
			trace(_speed);
		}
		
		public function coolDown() {
			this.addEventListener(Event.ENTER_FRAME, onUpdate);
			gotoAndPlay("cooldown");
			stop();
		}
	}

}