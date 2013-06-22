package respawn 
{
	import flash.display.MovieClip;
	import flash.external.ExternalInterface
	/**
	 * ...
	 * @author Rubén Mulero Guerrero
	 */
	public class RespawnManager extends MovieClip 
	{
		
		public function RespawnManager() 
		{
			ExternalInterface.addCallback("time", respawnTime);
		}
		
		public function respawnTime(time: int) {
			this.time.text = time.toString();
		}
	}

}