package  principal
{
	import flare.basic.Scene3D;
	import flare.basic.Viewer3D;
	import flare.core.Camera3D;
	import flare.core.Pivot3D;
	import flare.core.Texture3D;
	import flare.materials.filters.TextureMapFilter;
	import flare.materials.Shader3D;
	import flare.primitives.Plane;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.display.Stage;
	import flash.display.StageAlign;
	import flare.utils.Pivot3DUtils;
	import flash.display.StageScaleMode;
	
	
	/**
	 * The basics, easy! Drag to look around.
	 * 
	 * @author Ariel Nehamd
	 */
	public class test extends Sprite
	{
		private var scene:Scene3D;
		private var car:Pivot3D;
		private var axis:Pivot3D;
		private var floor:Plane;
		
		public function test() 
		{
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
			// creates a new 3d scene.
			scene = new Scene3D( this );
			
			scene.camera = new Camera3D();
			// add global scene progress and complete events.
			scene.addEventListener( Scene3D.PROGRESS_EVENT, progressEvent );
			scene.addEventListener( Scene3D.COMPLETE_EVENT, completeEvent );
			
			// loads the objects.
			
			
			floor = new Plane("floor", 10000, 10000); 
			floor.rotateX(90);
			scene.addChild(floor);
			
			var mat:Shader3D = new Shader3D();
			var tf:TextureMapFilter = new TextureMapFilter(new Texture3D("floor.jpg"));
			
			tf.repeatX = 100;
			tf.repeatY = 200;
			mat.filters = [tf];
			
			floor.setMaterial(mat);
			
			car = scene.addChildFromFile( "heavy.zf3d" );
		}
		
		private function progressEvent(e:Event):void 
		{
			// gets the global loading progress.
			trace( scene.loadProgress );
		}
		
		private function completeEvent(e:Event):void 
		{
			trace( "complete!" );
			scene.addChild(car);
			
			scene.addEventListener(Scene3D.UPDATE_EVENT, onUpdate);
		}
		
		protected function onUpdate(e:Event): void {
			
			Pivot3DUtils.setPositionWithReference(scene.camera, 0, 20, -5, car, 0.1);
			Pivot3DUtils.lookAtWithReference(scene.camera, 0, 10, 0, car, car.getDir(), 0.05);
			
		}
	}
}