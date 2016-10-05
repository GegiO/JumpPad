package gegio.JumpPads.init;

import gegio.JumpPads.JumpPads;
import gegio.JumpPads.Reference;
import gegio.JumpPads.Blocks.JumpPad;
import net.minecraft.block.Block;
import net.minecraft.block.material.Material;
import net.minecraft.client.Minecraft;
import net.minecraft.client.renderer.block.model.ModelResourceLocation;
import net.minecraft.item.Item;
import net.minecraft.item.ItemBlock;
import net.minecraftforge.client.model.ModelLoader;
import net.minecraftforge.fml.common.registry.GameRegistry;

public class InitBlocks {
	
	public static JumpPad jumppad;
	public static ItemBlock jumppad_itemblock;

	
	
	public static void init(){
		jumppad = new JumpPad();
		jumppad_itemblock = new ItemBlock(jumppad);
	}
	
	public static void register(){
		jumppad.setRegistryName(Reference.MOD_ID, "jumppad");
		jumppad.setUnlocalizedName(jumppad.getRegistryName().toString());
		jumppad_itemblock.setRegistryName(Reference.MOD_ID, "jumppad");
		jumppad_itemblock.setUnlocalizedName(jumppad_itemblock.getRegistryName().toString());
		GameRegistry.register(jumppad);
		GameRegistry.register(jumppad_itemblock);
	}
	
	public static void registerRenders(){
		registerRender(jumppad);

	}
	
	public static void registerRender(Block block){
		Item item = Item.getItemFromBlock(block);
		
		ModelLoader.setCustomModelResourceLocation(item, 0, new ModelResourceLocation(item.getRegistryName(),"inventory"));
	}
	

}
