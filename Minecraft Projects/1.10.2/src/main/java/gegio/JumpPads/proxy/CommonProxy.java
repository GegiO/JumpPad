package gegio.JumpPads.proxy;

import java.io.File;

import gegio.JumpPads.Reference;
import gegio.JumpPads.init.TileEntities;
import gegio.JumpPads.util.SubscribeEvents;
import net.minecraftforge.common.config.Configuration;
import net.minecraftforge.fml.common.event.FMLPostInitializationEvent;
import net.minecraftforge.fml.common.event.FMLPreInitializationEvent;

public class CommonProxy {
	
	public static Configuration config;
	
	public void registerRenders(){
		
	}
	
	public void registerTileEntity(){
		TileEntities.register();
	}
	
	public void preInit(FMLPreInitializationEvent e){
	}
	
	public void postInit(FMLPostInitializationEvent e){
	}
	
}
