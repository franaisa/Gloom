package Client
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import principal.MenuManager
	import flash.external.ExternalInterface;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class NetMenu extends MenuManager 
	{
		private var myIp: Ip;
		private var myNick: Nick;
		public function NetMenu() 
		{
			super();
			ExternalInterface.addCallback("connectionFailed", connectionFailed);
		}
		
		public function registerIp(ip: Ip): void {
			myIp = ip;
		}
		
		public function registerNick(nick: Nick): void {
			myNick = nick;
		}
		
		public function get ip():String {
			return myIp.ip;
		}
		
		public function get nick():String {
			return myNick.nick;
		}
		
		public function connectionFailed():void {
			
			connectWindow.gotoAndPlay("failed");
			
		}
	}

}