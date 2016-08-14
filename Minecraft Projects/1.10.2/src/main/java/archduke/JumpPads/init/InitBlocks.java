package archduke.JumpPads.init;

import archduke.JumpPads.JumpPads;
import archduke.JumpPads.Reference;
import archduke.JumpPads.Blocks.JumpPad;
import net.minecraft.block.Block;
import net.minecraft.block.material.Material;
import net.minecraft.client.Minecraft;
import net.minecraft.client.renderer.block.model.ModelResourceLocation;
import net.minecraft.item.Item;
import net.minecraft.item.ItemBlock;
import net.minecraftforge.client.model.ModelLoader;
import net.minecraftforge.fml.common.registry.GameRegistry;

public class InitBlocks {
	
	//public static Block test_block;
	
	public static JumpPad jumppad;
	public static ItemBlock jumppad_itemblock;
	//public static JumpPad_extend jumppad_extend;

	
	
	public static void init(){
		//test_block = new Block(Material.ground).setUnlocalizedName("test_block").setCreativeTab(JumpPads.cTabs);
		
		jumppad = new JumpPad();
		jumppad_itemblock = new ItemBlock(jumppad);
		//jumppad_extend = new JumpPad_extend();

	}
	
	public static void register(){
		//GameRegistry.registerBlock(test_block, test_block.getUnlocalizedName().substring(5));
		
		//GameRegistry.registerBlock(jumppad, jumppad.getUnlocalizedName().substring(5));
		
		jumppad.setRegistryName(Reference.MOD_ID, "jumppad");
		jumppad.setUnlocalizedName(jumppad.getRegistryName().toString());
		jumppad_itemblock.setRegistryName(Reference.MOD_ID, "jumppad");
		jumppad_itemblock.setUnlocalizedName(jumppad_itemblock.getRegistryName().toString());
		GameRegistry.register(jumppad);
		GameRegistry.register(jumppad_itemblock);
		

	}
	
	public static void registerRenders(){
		//registerRender(test_block);
		registerRender(jumppad);

	}
	
	public static void registerRender(Block block){
		Item item = Item.getItemFromBlock(block);
		
		ModelLoader.setCustomModelResourceLocation(item, 0, new ModelResourceLocation(item.getRegistryName(),"inventory"));
	}
	

}
