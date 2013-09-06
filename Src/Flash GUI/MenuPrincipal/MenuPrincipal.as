package MenuPrincipal 
{
	import principal.MenuManager;
	import flash.external.ExternalInterface;
	
	/**
	 * ...
	 * @author ...
	 */
	public class MenuPrincipal extends MenuManager 
	{
		
		public function MenuPrincipal() 
		{
			super();
			ExternalInterface.addCallback("netError", netError);
		}
		
		public function netError():void {
			neterror.visible = true;
		}
		
	}

}