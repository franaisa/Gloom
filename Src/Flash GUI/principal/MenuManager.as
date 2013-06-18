package principal
{
	import flash.ui.Mouse;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.external.ExternalInterface;

	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class MenuManager extends MovieClip 
	{
		var myCursor:Sprite;

		public function MenuManager() {
			init();
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  METHODS  //////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		function init()
		{
			//hide the mouse to put our own mouse cursor
			Mouse.hide();

			//load the custom mouse cursor
			myCursor = new CustomMouse();
			myCursor.mouseEnabled = false;
			myCursor.visible = false;
			
			// add the cursor  to the scene
			addChild(myCursor);
			
			// respond to mouse move events
			stage.addEventListener(MouseEvent.MOUSE_MOVE, mouseMoveHandler);
			stage.addEventListener(Event.MOUSE_LEAVE, mouseLeaveHandler);
			
			
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		function mouseHide() {
			myCursor.visible = false;
		}
		
		function mouseShow() {
			myCursor.visible = true;
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////  LISTENERS  ////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
		
		function mouseMoveHandler(evt:MouseEvent):void
		{
			// whenever the mouse moves, place the cursor in the same spot
			myCursor.visible = true;
			myCursor.x = evt.stageX;
			myCursor.y = evt.stageY;
		}

		function mouseLeaveHandler(evt:Event):void
		{
			myCursor.visible = false;
		}

	}
}