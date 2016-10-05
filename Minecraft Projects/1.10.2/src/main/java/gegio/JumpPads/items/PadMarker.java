package gegio.JumpPads.items;

import java.util.ArrayList;
import java.util.List;

import gegio.JumpPads.JumpPads;
import gegio.JumpPads.Reference;
import gegio.JumpPads.Subscriptions.AirResistNeutralizer;
import gegio.JumpPads.util.LaunchContainer;
import gegio.JumpPads.util.LaunchHelper;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.item.Item;
import net.minecraft.item.ItemStack;
import net.minecraft.nbt.NBTTagCompound;
import net.minecraft.nbt.NBTTagInt;
import net.minecraft.util.math.BlockPos;
import net.minecraft.util.EnumActionResult;
import net.minecraft.util.EnumFacing;
import net.minecraft.util.EnumHand;
import net.minecraft.world.World;
import net.minecraftforge.common.MinecraftForge;

//Item used by the jump pad to determine landing coordinates

public class PadMarker extends Item {
	
	private int dimension;
	private double power = 0;
	private double angle = 0;

	public PadMarker(){
		this.setUnlocalizedName("PadMarker");
		this.setCreativeTab(JumpPads.cTabs);
	}
	
	//When the pad marker is pointed to a block and right clicked
	//The coordinates of that block are saved in this items NBT data
	@Override
	public EnumActionResult onItemUse(ItemStack stack, EntityPlayer playerIn, World worldIn, BlockPos pos, EnumHand hand,
			EnumFacing facing, float hitX, float hitY, float hitZ){
		
			if(stack.getTagCompound() == null){
				stack.setTagCompound(new NBTTagCompound());
			}
			NBTTagCompound nbt = new NBTTagCompound();
			nbt.setDouble("posX", pos.getX() + .5);
			nbt.setDouble("posY", pos.getY());
			nbt.setDouble("posZ", pos.getZ() + .5);
			nbt.setInteger("dimension&check", playerIn.dimension);
			
			stack.getTagCompound().setTag("coords", nbt);


		return super.onItemUse(stack, playerIn, worldIn, pos, hand, facing, hitX, hitY, hitZ);
	}
	
	//Update the tooltip of the item with saved coordinates
	@Override
	public void addInformation(ItemStack stack, EntityPlayer playerIn, List<String> tooltip, boolean advanced) {
		if(stack.getTagCompound() != null){
			NBTTagCompound nbt = (NBTTagCompound) stack.getTagCompound().getTag("coords");
			tooltip.add("X: "+ nbt.getLong("posX") +" Y: "+ nbt.getLong("posY") +" Z " + nbt.getLong("posZ"));
		}
	}
}
	