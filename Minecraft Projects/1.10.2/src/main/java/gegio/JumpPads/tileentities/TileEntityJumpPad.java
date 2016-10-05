package gegio.JumpPads.tileentities;

import java.util.ArrayList;
import java.util.List;

import javax.annotation.Nullable;

import gegio.JumpPads.Blocks.JumpPad;
import gegio.JumpPads.util.CoordEntry;
import gegio.JumpPads.util.LaunchContainer;
import gegio.JumpPads.util.LaunchHelper;
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
import net.minecraft.network.play.server.SPacketUpdateTileEntity;
import net.minecraft.tileentity.TileEntity;
import net.minecraft.util.math.BlockPos;
import net.minecraft.util.EnumFacing;
import net.minecraft.util.ITickable;
import net.minecraft.world.World;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;
import net.minecraftforge.fml.common.network.IGuiHandler;
import scala.actors.threadpool.Arrays;

public class TileEntityJumpPad extends TileEntity implements IInventory, ITickable {

	int animTickCounter = 0;
	public ItemStack[] markerStorage = new ItemStack[getSizeInventory()];
	public BlockPos target;
	public int CURRENT_SELECTED = 0;
	public double ANGLE;
	public static final int NUM_SLOTS = 3;

	public boolean extendedBoolean = false;
	public boolean angleTriggerBoolean = false;
	public EnumFacing facingValue;

	public boolean hasJumped = true;

	public TileEntityJumpPad() {
	}

	// ITickable

	@Override
	public void update() {
		EntityPlayer playerIn = this.worldObj.getClosestPlayer(this.pos.getX() + .5, this.pos.getY(), this.pos.getZ() + .5, 1.5 ,false);
		EnumFacing enumfacing;
		if (!worldObj.isAirBlock(this.pos)) {
			enumfacing = this.worldObj.getBlockState(this.pos).getValue(JumpPad.PROPERTYFACING);
		} else {
			enumfacing = EnumFacing.NORTH;
		}

		EnumFacing angleMarker = enumfacing.rotateY();
		if (this.worldObj.isSidePowered(this.pos.offset(angleMarker), angleMarker)) {
			changeProperty(true, extendedBoolean, enumfacing);
		} else {
			changeProperty(false, extendedBoolean, enumfacing);
		}

		if (playerIn != null && playerIn.isSneaking() && playerIn.onGround && animTickCounter == 0) {

			CURRENT_SELECTED = getPowerSide(enumfacing);
			MarkAndNotifyPad();
			if (markerStorage[CURRENT_SELECTED] != null && markerStorage[CURRENT_SELECTED].hasTagCompound()) {

				ItemStack itemStackIn = markerStorage[CURRENT_SELECTED];
				NBTTagCompound nbt = (NBTTagCompound) itemStackIn.getTagCompound().getTag("coords");
				double targX = nbt.getLong("posX");
				double targY = nbt.getLong("posY");
				double targZ = nbt.getLong("posZ");
				
				LaunchContainer init = new LaunchHelper().getLaunchInfo(new BlockPos(playerIn.posX, playerIn.posY, playerIn.posZ), new BlockPos(targX, targY, targZ));
				
				if (this.worldObj.isRemote) {
					playerIn.setVelocity(init.getUnitX(), init.getUnitY(),init.getUnitZ());
				}
				playerIn.getEntityData().setBoolean("hasJumped", true);
				
				if(!angleTriggerBoolean){
					playerIn.getEntityData().setDouble("targetX", targX);
					playerIn.getEntityData().setDouble("targetY", targY);
					playerIn.getEntityData().setDouble("targetZ", targZ);
					playerIn.getEntityData().setBoolean("launchType", true);
				}else{
					playerIn.getEntityData().setBoolean("launchType", false);
				}
				

				if (!worldObj.isAirBlock(this.pos)) {
					changeProperty(angleTriggerBoolean, false, enumfacing);
				}
				animTickCounter++;
			} else {
				playerIn = null;
			}
		}

		if (animTickCounter > 0 && animTickCounter < 11) {
			animTickCounter++;
		} else if (animTickCounter >= 11) {
			if (!worldObj.isAirBlock(this.pos)) {
				changeProperty(angleTriggerBoolean, false, enumfacing);
			}
			animTickCounter = 0;
		}

	}
	
	public void changeProperty(boolean Anglemarker,boolean extended, EnumFacing facing){
		angleTriggerBoolean = Anglemarker; 
		extendedBoolean = extended;
		facingValue = facing;
		
		this.worldObj.setBlockState(this.pos,
				this.worldObj.getBlockState(this.pos)
				.withProperty(JumpPad.PROPERTYANGLE, angleTriggerBoolean).withProperty(JumpPad.PROPERTYFACING, facing)
				.withProperty(JumpPad.PROPERTYEXTEND, extendedBoolean),2);
	}
	
	public void MarkAndNotifyPad(){
		this.worldObj.markAndNotifyBlock(this.pos, this.worldObj.getChunkFromBlockCoords(this.pos),
				this.worldObj.getBlockState(pos), this.worldObj.getBlockState(pos), 3);
	}

	public int getPowerSide(EnumFacing blockDirection) {

		EnumFacing blue = blockDirection;
		EnumFacing red = blue.rotateYCCW();
		EnumFacing green = red.rotateYCCW();

		int maxPower;
		int bluePower = this.worldObj.getRedstonePower(this.pos.offset(blue), blue);
		int redPower = this.worldObj.getRedstonePower(this.pos.offset(red), red);
		int greenPower = this.worldObj.getRedstonePower(this.pos.offset(green), green);

		int power = bestOfThree(greenPower, redPower, bluePower);
		int tileentitycolor;

		if (power == greenPower) {
			tileentitycolor = 0;
			maxPower = greenPower;
		} else if (power == redPower) {
			tileentitycolor = 1;
			maxPower = redPower;
		} else {
			tileentitycolor = 2;
			maxPower = bluePower;
		}

		return tileentitycolor;
	}

	public int bestOfThree(int one, int two, int three) {
		if (two >= three) {
			if (one >= two) {
				return one;
			}
			return two;
		}
		return three;
	}

	// TileEntity

	@Override
	public boolean shouldRefresh(World world, BlockPos pos, IBlockState oldState, IBlockState newState) {
		return (oldState.getBlock() != newState.getBlock());
	}

	@Override
	public void readFromNBT(NBTTagCompound compound) {
		super.readFromNBT(compound);

		final byte NBT_TYPE_COMPOUND = 10;
		NBTTagList taglist = compound.getTagList("Items", NBT_TYPE_COMPOUND);

		Arrays.fill(markerStorage, null);
		for (int i = 0; i < taglist.tagCount(); ++i) {
			NBTTagCompound nbttagcompound = taglist.getCompoundTagAt(i);
			int slotIndex = nbttagcompound.getByte("Slot") & 255;

			if (slotIndex >= 0 && slotIndex < this.markerStorage.length) {
				this.markerStorage[slotIndex] = ItemStack.loadItemStackFromNBT(nbttagcompound);
			}
		}
	}

	@Override
	public NBTTagCompound writeToNBT(NBTTagCompound compound) {
		super.writeToNBT(compound);
		NBTTagList taglist = new NBTTagList();

		for (int i = 0; i < this.getSizeInventory(); ++i) {
			if (this.markerStorage[i] != null) {
				NBTTagCompound nbttagcompound = new NBTTagCompound();
				nbttagcompound.setByte("Slot", (byte) i);
				this.markerStorage[i].writeToNBT(nbttagcompound);
				taglist.appendTag(nbttagcompound);
			}
		}

		compound.setTag("Items", taglist);

		return compound;
	}

	@Override
	public SPacketUpdateTileEntity getUpdatePacket() {
		NBTTagCompound tag = new NBTTagCompound();
		writeToNBT(tag);
		return new SPacketUpdateTileEntity(this.pos, 1, tag);
	}

	@Override
	public void onDataPacket(net.minecraft.network.NetworkManager net,
			net.minecraft.network.play.server.SPacketUpdateTileEntity pkt) {
		readFromNBT(pkt.getNbtCompound());
	}

	// IInventory

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

		if (markerStorage[index] != null) {

			if (markerStorage[index].stackSize <= count) {
				ItemStack stack = markerStorage[index];
				markerStorage[index] = null;
				this.worldObj.markAndNotifyBlock(this.pos, this.worldObj.getChunkFromBlockCoords(this.pos),
						this.worldObj.getBlockState(pos), this.worldObj.getBlockState(pos), 3);
				markDirty();
				return stack;
			} else {
				ItemStack stack2 = markerStorage[index].splitStack(count);
				if (markerStorage[index].stackSize == 0)
					markerStorage[index] = null;
				this.worldObj.markAndNotifyBlock(this.pos, this.worldObj.getChunkFromBlockCoords(this.pos),
						this.worldObj.getBlockState(pos), this.worldObj.getBlockState(pos), 3);
				markDirty();
				return stack2;
			}

		}

		return null;
	}

	@Override
	public ItemStack removeStackFromSlot(int index) {
		if (markerStorage[index] != null) {
			ItemStack stack = markerStorage[index];
			markerStorage[index] = null;
			this.worldObj.markAndNotifyBlock(this.pos, this.worldObj.getChunkFromBlockCoords(this.pos),
					this.worldObj.getBlockState(pos), this.worldObj.getBlockState(pos), 3);
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
		if (this.worldObj.getTileEntity(this.pos) != this)
			return false;

		return player.getDistanceSq(pos.getX() + .5, pos.getY() + .5, pos.getZ() + .5) < 64;

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
		if (stack == null)
			return true;

		if (stack.getUnlocalizedName() == "PadMarker") {
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
