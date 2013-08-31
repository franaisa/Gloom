package principal 
{
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Rectangle;
	import Server.Maps;
	
	/**
	 * ...
	 * @author ...
	 */
	public class ScrollBar extends MovieClip 
	{
		var padre:Maps;
		var upper: Number;
		var initPos:Number;
		public function ScrollBar() 
		{
			padre = parent as Maps;
			stage.addEventListener(MouseEvent.MOUSE_DOWN, onPressed);
			stage.addEventListener(MouseEvent.MOUSE_UP, onReleased);
			
			initPos = padre.content.y;
			trace( - (this.height / 2) + (scroller.height/2));
			scroller.x = 0;
			scroller.y = 0 - (this.height / 2) + (scroller.height/2)
		}
		
		public function onPressed(e: Event):void {
			upper = 0 - (this.height / 2) + (scroller.height/2);
			trace(scroller.y);
			
			scroller.startDrag(false, new Rectangle( scroller.x, upper, 0, this.height - scroller.height ) );
			stage.addEventListener(MouseEvent.MOUSE_MOVE, this.moved);
			
		}
		
		public function onReleased(e: Event):void {
			stage.removeEventListener(MouseEvent.MOUSE_MOVE, this.moved);
			scroller.stopDrag();
			
		}
		
		public function moved(e:Event):void {
			if (padre.content.height < 315.95) {
				return;
			}
			
			
			var moved: Number = scroller.y - upper;
			var range:Number = -upper*2;
			var pctMoved: Number = moved / (range);
			padre.content.y = initPos - (padre.content.height - 315.95) * pctMoved;
		
		}
		
	}

	
	
}