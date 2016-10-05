package gegio.JumpPads.init;


import gegio.JumpPads.JumpPads;
import gegio.JumpPads.Reference;
import gegio.JumpPads.items.PadMarker;
import net.minecraft.client.Minecraft;
import net.minecraft.client.renderer.block.model.ModelResourceLocation;
import net.minecraft.item.Item;
import net.minecraftforge.client.model.ModelLoader;
import net.minecraftforge.fml.common.registry.GameRegistry;

public class InitItems {
	
	public static Item CreativeIcon;
	public static PadMarker PadMarker;
	
	public static void init(){
		CreativeIcon = new Item().setUnlocalizedName("CreativeIcon");
		PadMarker = new PadMarker();
	}
	
	public static void register(){
		CreativeIcon.setRegistryName(Reference.MOD_ID,"CreativeIcon");
		GameRegistry.register(CreativeIcon);
		PadMarker.setRegistryName(Reference.MOD_ID,"PadMarker");
		GameRegistry.register(PadMarker);
		
		
	}
	
	public static void registerRenders(){
		registerRender(CreativeIcon);
		registerRender(PadMarker);
	}
	
	public static void registerRender(Item item){
		ModelLoader.setCustomModelResourceLocation(item, 0, new ModelResourceLocation(item.getRegistryName(),"inventory"));
	}
}