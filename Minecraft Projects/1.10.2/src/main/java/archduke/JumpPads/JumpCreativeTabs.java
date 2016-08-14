package archduke.JumpPads;

import archduke.JumpPads.init.InitItems;
import net.minecraft.creativetab.CreativeTabs;
import net.minecraft.item.Item;


//Initializes the creative tab, which allows players to cheat items in
public class JumpCreativeTabs extends CreativeTabs{

	public JumpCreativeTabs(String label) {
		super(label);
	}
	
	@Override 
	public Item getTabIconItem(){
		return InitItems.CreativeIcon;
	}
}