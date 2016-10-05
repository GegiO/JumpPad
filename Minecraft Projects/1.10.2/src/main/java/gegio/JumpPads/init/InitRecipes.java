package gegio.JumpPads.init;

import net.minecraft.init.Blocks;
import net.minecraft.init.Items;
import net.minecraft.item.ItemStack;
import net.minecraftforge.fml.common.registry.GameRegistry;

public class InitRecipes {
	public static void initRecipe(){
		GameRegistry.addRecipe(new ItemStack(InitBlocks.jumppad),
				"IPI",
				"WEW",
				"IRI",
				'I', Items.IRON_INGOT,
				'P', Blocks.PLANKS,
				'W', Blocks.WOOL,
				'E', Items.ENDER_PEARL,
				'R', Blocks.REDSTONE_BLOCK);
		GameRegistry.addRecipe(new ItemStack(InitItems.PadMarker),
				"  P",
				" I ",
				"E  ",
				'P', Items.PAPER,
				'I', Items.IRON_INGOT,
				'E', Items.ENDER_PEARL);
	}
	

}
