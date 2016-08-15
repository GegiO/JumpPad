package gegio.JumpPads;

import gegio.JumpPads.init.InitBlocks;
import gegio.JumpPads.init.InitItems;
import gegio.JumpPads.init.TileEntities;
import gegio.JumpPads.init.initGUI;
import gegio.JumpPads.playerMods.AirResistNeutralizer;
import gegio.JumpPads.proxy.ClientProxy;
import gegio.JumpPads.proxy.CommonProxy;
import gegio.JumpPads.util.SubscribeEvents;
import net.minecraftforge.common.MinecraftForge;
import net.minecraftforge.fml.common.Mod;
import net.minecraftforge.fml.common.Mod.EventHandler;
import net.minecraftforge.fml.common.SidedProxy;
import net.minecraftforge.fml.common.event.FMLInitializationEvent;
import net.minecraftforge.fml.common.event.FMLPostInitializationEvent;
import net.minecraftforge.fml.common.event.FMLPreInitializationEvent;

//initialize mod
@Mod(modid = Reference.MOD_ID, name = Reference.MOD_NAME, version = Reference.MOD_VERSION)
public class JumpPads{
	
	@SidedProxy(clientSide = Reference.CLIENT_PROXY_CLASS, serverSide = Reference.SERVER_PROXY_CLASS)
	public static ClientProxy proxy;
	
	public static final JumpCreativeTabs cTabs = new JumpCreativeTabs("cTabs");
	public static final SubscribeEvents airNeutralizer = new SubscribeEvents();
	
	@Mod.Instance(Reference.MOD_ID)
	public static JumpPads instance;
	
	
	@EventHandler
	public void preInit(FMLPreInitializationEvent event){
		InitItems.init();
		InitItems.register();
		InitBlocks.init();
		InitBlocks.register();
		initGUI.registerGUI();	
		proxy.registerTileEntity();
		proxy.registerRenders();
	}
	
	
	@EventHandler
	public void init(FMLInitializationEvent event){
		//proxy.registerRenders();
	}
	
	
	@EventHandler
	public void postInit(FMLPostInitializationEvent event){
		airNeutralizer.subscribeAirResist();
	}

}