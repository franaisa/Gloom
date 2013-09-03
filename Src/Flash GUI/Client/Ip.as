package  Client
{
	import flash.display.Sprite;
	import flash.events.TextEvent;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import flash.text.*;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class Ip extends Sprite 
	{
		private var _ip: String;
		private var manager: NetMenu;
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  BUILDERS  /////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		public function Ip() 
		{
			manager = parent as NetMenu;;
			manager.registerIp(this);
			
			var txt: TextField = getChildByName("inputIp") as TextField;
			
			txt.text = "127.0.0.1";
			
			
			_ip = txt.text;
			//listen to text events
			this.addEventListener(TextEvent.TEXT_INPUT, onTextEnter);
			
			//listen to mouse events to hide the custom mouse cursor
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			
			ExternalInterface.addCallback("getIp", getIp);
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////  CLASS METHODS  //////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////	
		
		public function getIp(): String {
			return _ip;
		}
		
		public function get ip():String {
			return _ip;
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  LISTENERS  ////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		private function onTextEnter(e: TextEvent): void {
			var txt: TextField = getChildByName("inputIp") as TextField;
			
			_ip = txt.text+e.text;
		}
		
		private function onMouseOver(event: MouseEvent): void {
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			//manager.mouseHide();
		}
		
		private function onMouseOut(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			//manager.mouseShow();
		}
		
	}

}