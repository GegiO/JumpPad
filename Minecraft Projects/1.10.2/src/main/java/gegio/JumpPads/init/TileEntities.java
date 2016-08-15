package gegio.JumpPads.init;

import gegio.JumpPads.Reference;
import gegio.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraftforge.fml.common.registry.GameRegistry;

public class TileEntities {
	

	public static void register(){
		GameRegistry.registerTileEntity(TileEntityJumpPad.class, Reference.MOD_ID + "jumpPad");
	}
}
