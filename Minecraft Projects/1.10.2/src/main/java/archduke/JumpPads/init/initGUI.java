package archduke.JumpPads.init;

import archduke.JumpPads.GuiHandlerRegistry;
import archduke.JumpPads.JumpPads;
import archduke.JumpPads.gui.GUIhandler;
import net.minecraftforge.fml.common.network.NetworkRegistry;

public class initGUI {
	
	public static void registerGUI(){
		
		NetworkRegistry.INSTANCE.registerGuiHandler(JumpPads.instance, GuiHandlerRegistry.getInstance());
		GuiHandlerRegistry.getInstance().registerGuiHandler(new GUIhandler(), GUIhandler.getGuiID());
	}

}
