package  
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class Texto extends MovieClip 
	{
		
		public function Texto(name: String) 
		{
			var txt: TextField = getChildByName("text") as TextField;
			txt.text = name;
			txt.visible = true;;
		}
		
	}

}