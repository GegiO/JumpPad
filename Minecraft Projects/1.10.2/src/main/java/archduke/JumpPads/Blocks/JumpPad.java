package archduke.JumpPads.Blocks;

import java.util.Collection;
import java.util.List;

import javax.annotation.Nullable;

import archduke.JumpPads.JumpPads;
import archduke.JumpPads.gui.GUIhandler;
import archduke.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraft.block.Block;
import net.minecraft.block.ITileEntityProvider;
import net.minecraft.block.material.Material;
import net.minecraft.block.properties.IProperty;
import net.minecraft.block.properties.PropertyBool;
import net.minecraft.block.properties.PropertyDirection;
import net.minecraft.block.properties.PropertyInteger;
import net.minecraft.block.state.BlockStateContainer;
import net.minecraft.block.state.IBlockState;
import net.minecraft.entity.Entity;
import net.minecraft.entity.EntityLivingBase;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.item.ItemStack;
import net.minecraft.tileentity.TileEntity;
import net.minecraft.util.math.AxisAlignedBB;
import net.minecraft.util.math.BlockPos;
import net.minecraft.util.EnumFacing;
import net.minecraft.util.EnumHand;
import net.minecraft.world.World;

public class JumpPad extends Block implements ITileEntityProvider{
	
	public static final PropertyDirection PROPERTYFACING = PropertyDirection.create("facing", EnumFacing.Plane.HORIZONTAL);
	public static final PropertyBool PROPERTYEXTEND = PropertyBool.create("extended");
	public static final AxisAlignedBB BOUNDINGBOX = new AxisAlignedBB(0,0,0,1,.1875,1);

	public JumpPad() {
		super(Material.IRON);
		this.setDefaultState(this.blockState.getBaseState().withProperty(PROPERTYFACING, EnumFacing.NORTH).withProperty(PROPERTYEXTEND, Boolean.valueOf(false)));
		this.setCreativeTab(JumpPads.cTabs);
		this.setUnlocalizedName("jumppad");
	}
	
	@Override
	public TileEntity createNewTileEntity(World worldIn, int meta){
		
		return new TileEntityJumpPad(this.getDefaultState());
	}
	
	@Override
    public boolean onBlockActivated(World worldIn, BlockPos pos, IBlockState state, EntityPlayer playerIn, EnumHand hand, @Nullable ItemStack heldItem, EnumFacing side, float hitX, float hitY, float hitZ)
    {
        if(worldIn.isRemote)
        	return true;
        
        playerIn.openGui(JumpPads.instance, GUIhandler.getGuiID(), worldIn, pos.getX(), pos.getY(), pos.getZ());
        
        return true;
    }
	
	
	@Override
	public void breakBlock(World world, BlockPos pos, IBlockState state){
		super.breakBlock(world, pos, state);
		world.removeTileEntity(pos);
	}
	
	@Override
	public boolean eventReceived(IBlockState state, World worldIn, BlockPos pos, int id, int param){
		
		TileEntity tileentity = worldIn.getTileEntity(pos);
        return tileentity == null ? false : tileentity.receiveClientEvent(id, param);
    }

	@Override
    public boolean isOpaqueCube(IBlockState state)
    {
        return false;
    }
	
	@Override
	public boolean isFullCube(IBlockState state) {
		
		return false;
	}
	
	@Override
    protected BlockStateContainer createBlockState()
    {
        return new BlockStateContainer(this, new IProperty[] {PROPERTYFACING, PROPERTYEXTEND});
        
    }
	
	@Override
	public IBlockState getStateFromMeta(int meta){
		EnumFacing facing = EnumFacing.getHorizontal(meta);
		boolean extend = Boolean.valueOf((meta & 8) > 0);
		return this.getDefaultState().withProperty(PROPERTYFACING, facing).withProperty(PROPERTYEXTEND, extend); 
	}
	
	@Override
	public int getMetaFromState(IBlockState state){
		EnumFacing facing = (EnumFacing)state.getValue(PROPERTYFACING);
		int bits = facing.getHorizontalIndex();
		
        if (((Boolean)state.getValue(PROPERTYEXTEND)).booleanValue())
        {
            bits |= 8;
        }
		
		return bits;
	}
    
	
	@Override
    public IBlockState onBlockPlaced(World worldIn, BlockPos pos, EnumFacing facing, float hitX, float hitY, float hitZ, int meta, EntityLivingBase placer)
    {
		EnumFacing enumfacing = (placer == null) ? EnumFacing.NORTH : EnumFacing.fromAngle(placer.rotationYaw);
		
        return this.getDefaultState().withProperty(PROPERTYFACING, enumfacing).withProperty(PROPERTYEXTEND, false);
       
    }
	
	
	
	@Override
    public void addCollisionBoxToList(IBlockState state, World worldIn, BlockPos pos, AxisAlignedBB entityBox, List<AxisAlignedBB> collidingBoxes, @Nullable Entity entityIn)
    {
		addCollisionBoxToList(pos, entityBox, collidingBoxes, BOUNDINGBOX);

    }
	
	public void extend(){
		this.getDefaultState().withProperty(PROPERTYEXTEND, true);
	}
	
	public void retract(){
		this.getDefaultState().withProperty(PROPERTYEXTEND, false);
	}
}