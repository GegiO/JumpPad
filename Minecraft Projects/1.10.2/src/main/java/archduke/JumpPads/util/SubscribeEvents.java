package archduke.JumpPads.util;

import archduke.JumpPads.playerMods.AirResistNeutralizer;
import archduke.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraftforge.common.MinecraftForge;

public class SubscribeEvents {
	
	public void subscribeAirResist(){
		MinecraftForge.EVENT_BUS.register(new AirResistNeutralizer());
	}
	
	public void unsubscribeAirResist(){
		MinecraftForge.EVENT_BUS.unregister(new AirResistNeutralizer());
	}

}
