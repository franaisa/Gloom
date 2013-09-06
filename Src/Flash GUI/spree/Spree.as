package spree 
{
	import flash.display.MovieClip;
	import flash.external.ExternalInterface;
	/**
	 * ...
	 * @author ...
	 */
	public class Spree extends MovieClip 
	{
		
		public function Spree() 
		{
			ExternalInterface.addCallback("spree", spreeM);
		}
		
		
		public function spreeM(nick:String, message: String):void {
			spreemsg.nick.nick.text = nick;
			spreemsg.spreetext.spree.text = message;
			spreemsg.gotoAndPlay("spree");
		}
	}

}