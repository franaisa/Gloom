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
			ExternalInterface.addCallback("localSpree", localSpreeM);
			ExternalInterface.addCallback("remoteSpree", remoteSpreeM);
		}
		
		
		public function localSpreeM(message: String):void {
			localspreemsg.spreetext.spree.text = message;
			localspreemsg.gotoAndPlay("spree");
		}
		
		public function remoteSpreeM(message: String):void {
			remotemsg.spreetext.spree.text = message;
			remotemsg.gotoAndPlay("spree");
		}
	}

}