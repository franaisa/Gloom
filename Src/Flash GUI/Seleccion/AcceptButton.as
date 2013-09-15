package Seleccion 
{
	import flash.events.MouseEvent;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author ...
	 */
	public class AcceptButton extends GloomButton 
	{
		private var manager: SelectionManager;
		public function AcceptButton() 
		{
			super();
			manager = parent as SelectionManager;
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			manager.selectionFinished();
			trace("accept");
		}
		
	}

}