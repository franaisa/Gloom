package  
{
	import flash.text.TextField;
	import flash.events.Event;
	
	/**
	 * ...
	 * @author ...
	 */
	public class Killer extends TextField 
	{
		
		public function Killer() 
		{
			var padre:KilledBox = parent as KilledBox;
			trace("me ejecuto");
			padre.kller = this;
		}
		
	}

}