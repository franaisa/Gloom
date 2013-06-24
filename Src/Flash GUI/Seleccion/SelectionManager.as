package Seleccion
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
	import principal.CustomMouse;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class SelectionManager extends MovieClip 
	{
		var myCursor:Sprite;
		
		var spell1:String;
		var spell2:String;
		var playerClass:String;
		
		public function SelectionManager() {
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
		
		public function addSpell(spell: String) {
			if (spell1 != spell && spell2 != spell) {
				if (spell2 != null && spell2.length > 0) {
					var auxspell:GloomButton = getChildByName(spell2) as GloomButton;
					auxspell.gotoAndPlay("idle");
				}
				
				spell2 = spell1;
				spell1 = spell;
			}
			
		}
		
		public function removeSpell(spell: String) {
			if (spell1 == spell) {
				spell1 = spell2;
				spell2 = "";
				return;
			}
			if (spell2 == spell) {
				spell2 = "";
			}
		}
		
		public function selectionFinished() {
			if (spell1 == null || spell2 == null || playerClass == null) {
				return;
			}
			ExternalInterface.call("selected", playerClass, spell1, spell2);
		}
		
		public function classSelected(playerClass:String):void {
			this.playerClass = playerClass;
			
			if (hound.name != playerClass && hound.currentFrameLabel != "idle") {
				hound.gotoAndPlay("idle");
			}
			if (archangel.name != playerClass && archangel.currentFrameLabel != "idle") {
				archangel.gotoAndPlay("idle");
			}
			if (screamer.name != playerClass && screamer.currentFrameLabel != "idle") {
				screamer.gotoAndPlay("idle");
			}
			if (shadow.name != playerClass && shadow.currentFrameLabel != "idle") {
				shadow.gotoAndPlay("idle");
			}
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