package Client
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class ConnectButton extends GloomButton 
	{
		private var manager: NetMenu;
		
		public function ConnectButton() 
		{
			super();
			manager = parent as NetMenu;
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			
			if (manager.currentFrameLabel == "ip") {
				var ip: String;
				var nick: String;
				ip = manager.ip;
				nick = manager.nick;
				this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
				ExternalInterface.call("connect", ip, nick);
			}
			
		}
		
	}

}