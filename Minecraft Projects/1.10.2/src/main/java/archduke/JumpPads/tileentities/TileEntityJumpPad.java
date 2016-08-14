package archduke.JumpPads.tileentities;

import java.util.ArrayList;
import java.util.List;

import archduke.JumpPads.Blocks.JumpPad;
import archduke.JumpPads.util.CoordEntry;
import archduke.JumpPads.util.LaunchContainer;
import archduke.JumpPads.util.LaunchHelper;
import net.minecraft.block.state.IBlockState;
import net.minecraft.client.Minecraft;
import net.minecraft.entity.EntityLiving;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.inventory.IInventory;
import net.minecraft.item.ItemStack;
import net.minecraft.nbt.NBTTagCompound;
import net.minecraft.nbt.NBTTagList;
import net.minecraft.network.NetworkManager;
import net.minecraft.network.Packet;

import net.minecraft.tileentity.TileEntity;
import net.minecraft.util.math.BlockPos;

import net.minecraft.util.ITickable;
import net.minecraft.world.World;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;
import net.minecraftforge.fml.common.network.IGuiHandler;
import scala.actors.threadpool.Arrays;

//Does nothing yet
//will eventually calculate and launch the player
public class TileEntityJumpPad extends TileEntity implements IInventory, ITickable{
	
	
	int animTickCounter =0;
	
	
	private ItemStack[] markerStorage = new ItemStack[getSizeInventory()];
	
	public static final int GREEN = 0;
	public static final int RED = 1;
	public static final int BLUE = 2;
	public int CURRENT_SELECTED = 0;
	
	public static final int NUM_SLOTS = 3;
	
	IBlockState blockstate;
	
	public TileEntityJumpPad(){
		System.out.println("Created tile entity");
	}
	
	public TileEntityJumpPad(IBlockState BlockState){
		blockstate = BlockState;
	}
	
	//ITickable

	@Override
	public void update() {
		
		EntityPlayer playerIn = this.worldObj.getClosestPlayer(this.pos.getX(), this.pos.getY(), this.pos.getZ(), 1,false);
		
		if(playerIn != null && markerStorage[CURRENT_SELECTED] != null){
			if(playerIn.isSneaking() && playerIn.onGround){
				
				ItemStack itemStackIn = markerStorage[CURRENT_SELECTED];
				
				NBTTagCompound nbt = (NBTTagCompound) itemStackIn.getTagCompound().getTag("coords");
				
				LaunchContainer init = new LaunchHelper().getLaunchInfo(nbt.getLong("posX"), nbt.getLong("posY"), nbt.getLong("posZ"), playerIn);
				
				if(!Double.isNaN(init.getLaunchSpeed()) && this.worldObj.isRemote){

					System.out.println(" X unit: "+ init.getUnitX()+" Y unit: " + init.getUnitY() + " Z Unit: " + init.getUnitZ());
					System.out.println("launch Speed: " + init.getLaunchSpeed());
					
					double launchSpeed = init.getLaunchSpeed();
					
					playerIn.setVelocity(init.getUnitX() * launchSpeed,init.getUnitY() * launchSpeed, init.getUnitZ() * launchSpeed);
					playerIn.getEntityData().setBoolean("hasJumped", true);
					
					worldObj.setBlockState(this.pos, blockstate.withProperty(JumpPad.PROPERTYEXTEND, true), 2);
					animTickCounter++;
				}
			}else{
				playerIn = null;
			}
		}
		
		if(animTickCounter > 0 && animTickCounter < 11){
			animTickCounter++;
		}else if(animTickCounter >= 11){
			worldObj.setBlockState(this.pos, blockstate.withProperty(JumpPad.PROPERTYEXTEND, false), 2);
			animTickCounter = 0;
		}
	}
		
	//TileEntity
	
	@Override
    public boolean shouldRefresh(World world, BlockPos pos, IBlockState oldState, IBlockState newState)
    {
        return (oldState.getBlock() != newState.getBlock());
    }
	
	@Override
	public void readFromNBT(NBTTagCompound compound){
		super.readFromNBT(compound);
		
		
		final byte NBT_TYPE_COMPOUND = 10;
		NBTTagList taglist = compound.getTagList("Items", NBT_TYPE_COMPOUND);
		
		Arrays.fill(markerStorage, null);
		for(int i=0; i < taglist.tagCount(); ++i){
			NBTTagCompound nbttagcompound = taglist.getCompoundTagAt(i);
			int slotIndex = nbttagcompound.getByte("Slot") & 255;
			
			if(slotIndex >= 0 && slotIndex < this.markerStorage.length){
				this.markerStorage[slotIndex] = ItemStack.loadItemStackFromNBT(nbttagcompound);
			}
		}
		System.out.println("read from NBT");
	}
	
	@Override
	public NBTTagCompound writeToNBT(NBTTagCompound compound){
		
		NBTTagList taglist = new NBTTagList();
		
		for(int i=0 ; i < this.getSizeInventory() ; ++i){
			if(this.markerStorage[i] != null){
				NBTTagCompound nbttagcompound = new NBTTagCompound();
				nbttagcompound.setByte("Slot", (byte)i);
				this.markerStorage[i].writeToNBT(nbttagcompound);
				taglist.appendTag(nbttagcompound);
			}
		}
		
		compound.setTag("Items", taglist);
		
		
		System.out.println("Written to NBT");
		return compound;
	}
	

	//IInventory
	
	@Override
	public String getName() {
		return "Jump Pad";
	}

	@Override
	public boolean hasCustomName() {

		return false;
	}



	@Override
	public int getSizeInventory() {
		return NUM_SLOTS;
	}

	@Override
	public ItemStack getStackInSlot(int index) {
		return markerStorage[index];
	}

	@Override
	public ItemStack decrStackSize(int index, int count) {
		
		if(markerStorage[index] != null){
			
			if(markerStorage[index].stackSize <= count){
				ItemStack stack = markerStorage[index];
				markerStorage[index] = null;
				markDirty();
				return stack;
			}else{
				ItemStack stack2 = markerStorage[index].splitStack(count);
				if(markerStorage[index].stackSize == 0)
					markerStorage[index] = null;
				markDirty();
				return stack2;
			}
			
		}
		
		return null;
	}

	@Override
	public ItemStack removeStackFromSlot(int index) {
		if(markerStorage[index] != null){
			ItemStack stack = markerStorage[index];
			markerStorage[index] = null;
			markDirty();
			return stack;
		}
		return null;
	}

	@Override
	public void setInventorySlotContents(int index, ItemStack stack) {
		markerStorage[index] = stack;
	}

	@Override
	public int getInventoryStackLimit() {
		// TODO Auto-generated method stub
		return 1;
	}

	@Override
	public boolean isUseableByPlayer(EntityPlayer player) {
		if(this.worldObj.getTileEntity(this.pos) != this)
			return false;
		
		return player.getDistanceSq(pos.getX() + .5, pos.getY()+.5 , pos.getZ() + .5) < 64;
				
	}

	@Override
	public void openInventory(EntityPlayer player) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void closeInventory(EntityPlayer player) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean isItemValidForSlot(int index, ItemStack stack) {
		if(stack == null)
			return true;
		
		if(stack.getUnlocalizedName() == "PadMarker"){
			return true;
		}
		
		return false;
	}

	@Override
	public int getField(int id) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void setField(int id, int value) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public int getFieldCount() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void clear() {
		// TODO Auto-generated method stub
		
	}
	
	
}
