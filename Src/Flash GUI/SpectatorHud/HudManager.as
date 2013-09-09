package SpectatorHud 
{
	import flash.display.MovieClip;
	
	import flash.external.ExternalInterface;
	/**
	 * ...
	 * @author ...
	 */
	public class HudManager extends MovieClip 
	{
		
		public function HudManager() 
		{
			ExternalInterface.addCallback("freelook", freelook);
			ExternalInterface.addCallback("lookAt", lookAt);
			
			freelook();
		}
		public function freelook() {
			cross.visible = false;
			freetitle.visible = true;
			follow.visible = false;
		}
	
		
		public function lookAt(name: String) {
			cross.visible = true;
			freetitle.visible = false;
			follow.visible = true;
			follow.player.text = name;
			trace (lookmessage.lookmessage.message.text);
			lookmessage.lookmessage.message.text = "Following " + name;
			trace (lookmessage.lookmessage.message.text);
			lookmessage.gotoAndPlay("look");
		}
	}
	
	

}