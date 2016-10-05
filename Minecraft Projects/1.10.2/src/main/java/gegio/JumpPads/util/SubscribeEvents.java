package gegio.JumpPads.util;

import gegio.JumpPads.Subscriptions.AirResistNeutralizer;
import gegio.JumpPads.Subscriptions.FallDamageNeutralizer;
import gegio.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraftforge.common.MinecraftForge;

public class SubscribeEvents {
	
	public void subscribeAirResist(){
		MinecraftForge.EVENT_BUS.register(new AirResistNeutralizer());
	}
	
	public void unsubscribeAirResist(){
		MinecraftForge.EVENT_BUS.unregister(new AirResistNeutralizer());
	}
	
	public void subscribeFallNegate(){
		MinecraftForge.EVENT_BUS.register(new FallDamageNeutralizer());
	}
	

}
