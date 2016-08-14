package archduke.JumpPads.proxy;

import archduke.JumpPads.init.InitBlocks;
import archduke.JumpPads.init.InitItems;

public class ClientProxy extends CommonProxy {
	@Override
	public void registerRenders(){
		InitItems.registerRenders();
		InitBlocks.registerRenders();
	}
}
