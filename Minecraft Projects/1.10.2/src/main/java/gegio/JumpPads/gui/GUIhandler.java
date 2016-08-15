package gegio.JumpPads.gui;

import gegio.JumpPads.Blocks.JumpPadContainer;
import gegio.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.tileentity.TileEntity;
import net.minecraft.util.math.BlockPos;
import net.minecraft.world.World;
import net.minecraftforge.fml.common.network.IGuiHandler;

public class GUIhandler  implements IGuiHandler{
	private static final int GUIID = 50;
	public static int getGuiID(){
		return GUIID;
	}

	@Override
	public Object getServerGuiElement(int ID, EntityPlayer player, World world, int x, int y, int z) {
		if(ID != getGuiID()){
			System.err.println("Invalid ID: expected " + getGuiID() + ", recieved " + ID);
		}
		
		BlockPos xyz = new BlockPos(x, y, z);
		TileEntity tileEntity = world.getTileEntity(xyz);
		if(tileEntity instanceof TileEntityJumpPad){
			TileEntityJumpPad tileEntityJumpPad = (TileEntityJumpPad) tileEntity;
			return new JumpPadContainer(player.inventory, tileEntityJumpPad);
		}
		
		return null;
	}

	@Override
	public Object getClientGuiElement(int ID, EntityPlayer player, World world, int x, int y, int z) {
		
		if(ID != getGuiID()){
			System.err.println("Invalid ID: expected " + getGuiID() + ", recieved " + ID);
		}
		
		BlockPos xyz = new BlockPos(x, y, z);
		TileEntity tileEntity = world.getTileEntity(xyz);
		if(tileEntity instanceof TileEntityJumpPad){
			TileEntityJumpPad tileEntityJumpPad = (TileEntityJumpPad) tileEntity;
			return new GuiJumpPad(player.inventory, tileEntityJumpPad);
		}
		
		return null;
	
	}
	

}
