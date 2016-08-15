package gegio.JumpPads.init;

import gegio.JumpPads.GuiHandlerRegistry;
import gegio.JumpPads.JumpPads;
import gegio.JumpPads.gui.GUIhandler;
import net.minecraftforge.fml.common.network.NetworkRegistry;

public class initGUI {
	
	public static void registerGUI(){
		
		NetworkRegistry.INSTANCE.registerGuiHandler(JumpPads.instance, GuiHandlerRegistry.getInstance());
		GuiHandlerRegistry.getInstance().registerGuiHandler(new GUIhandler(), GUIhandler.getGuiID());
	}

}
