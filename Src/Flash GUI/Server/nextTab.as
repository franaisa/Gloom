package Server 
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	/**
	 * ...
	 * @author ...
	 */
	public class nextTab extends MovieClip 
	{
		
		public function nextTab() 
		{
			this.addEventListener(MouseEvent.CLICK, onMouseClick);
		}
		protected function onMouseClick(e:MouseEvent):void {
			var a: MovieClip = parent as MovieClip;
			var b: MovieClip = parent.parent as MovieClip;
			b.play();
			a.play();
			
		}
	}

}