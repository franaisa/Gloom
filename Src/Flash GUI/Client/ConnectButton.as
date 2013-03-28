package  
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class ConnectButton extends MovieClip 
	{
		private var manager: NetMenu;
		
		public function ConnectButton() 
		{
			
			manager = parent as NetMenu;
			
			this.buttonMode = true;
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.CLICK, onMouseClick);
		}
		
		private function onMouseOver(event: MouseEvent): void {
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.gotoAndPlay("rollover");
			
		}
		
		private function onMouseOut(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.gotoAndPlay("rollout");
		}
		
		private function onMouseClick(e:MouseEvent):void {
			
			var ip: String;
			var nick: String;
			ip = manager.ip;
			nick = manager.nick;
			
			ExternalInterface.call("connect", ip, nick);
		}
		
	}

}