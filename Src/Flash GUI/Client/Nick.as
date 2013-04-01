package  
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
	public class Nick extends Sprite 
	{
		private var manager: NetMenu;
		private var _nick: String;
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  BUILDERS  /////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		public function Nick() 
		{
			manager = parent as NetMenu;;
			manager.registerNick(this);
			
			var txt: TextField = getChildByName("inputNick") as TextField;
			
			txt.text = "rub0te";
			
			_nick = txt.text;
			
			//listen to text events
			this.addEventListener(TextEvent.TEXT_INPUT, onTextEnter);
			
			//listen to mouse events to hide the custom mouse cursor
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			
			//callback to throw information to c++
			ExternalInterface.addCallback("getNick", getNick);
		}
		
		public function getNick(): String {
			return _nick;
		}
		
		public function get nick():String {
			return _nick;
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  LISTENERS  ////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		private function onTextEnter(e: TextEvent): void {
			var txt: TextField = getChildByName("inputNick") as TextField;
			
			_nick = txt.text+e.text;
		}
		
		private function onMouseOver(event: MouseEvent): void {
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			manager.mouseHide();
		}
		
		private function onMouseOut(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			manager.mouseShow();
		}
		
	}

}