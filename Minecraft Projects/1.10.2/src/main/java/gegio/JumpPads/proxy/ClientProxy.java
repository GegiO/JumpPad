package gegio.JumpPads.proxy;

import gegio.JumpPads.init.InitBlocks;
import gegio.JumpPads.init.InitItems;

public class ClientProxy extends CommonProxy {
	@Override
	public void registerRenders(){
		InitItems.registerRenders();
		InitBlocks.registerRenders();
	}
}
