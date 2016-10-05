package gegio.JumpPads.Blocks;

import java.util.Collection;
import java.util.List;

import javax.annotation.Nullable;

import gegio.JumpPads.JumpPads;
import gegio.JumpPads.gui.GUIhandler;
import gegio.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraft.block.Block;
import net.minecraft.block.BlockRedstoneWire;
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
import net.minecraft.entity.item.EntityItem;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.init.Blocks;
import net.minecraft.inventory.InventoryHelper;
import net.minecraft.item.ItemStack;
import net.minecraft.tileentity.TileEntity;
import net.minecraft.util.math.AxisAlignedBB;
import net.minecraft.util.math.BlockPos;
import net.minecraft.util.EnumFacing;
import net.minecraft.util.EnumHand;
import net.minecraft.world.IBlockAccess;
import net.minecraft.world.World;

public class JumpPad extends Block implements ITileEntityProvider{
	
	public static final PropertyDirection PROPERTYFACING = PropertyDirection.create("facing", EnumFacing.Plane.HORIZONTAL);
	public static final PropertyBool PROPERTYEXTEND = PropertyBool.create("extended");
	public static final AxisAlignedBB BOUNDINGBOX = new AxisAlignedBB(0,0,0,1,.1875,1);
	public static final PropertyBool PROPERTYANGLE = PropertyBool.create("anglemode");
	
	private EnumFacing facing = EnumFacing.NORTH;

	public JumpPad() {
		super(Material.IRON);
		this.setDefaultState(this.blockState.getBaseState().withProperty(PROPERTYFACING, EnumFacing.NORTH).withProperty(PROPERTYEXTEND, Boolean.valueOf(false)).withProperty(PROPERTYANGLE, false));
		this.setCreativeTab(JumpPads.cTabs);
		this.setUnlocalizedName("jumppad");
		this.setHardness(5);
	}
	
	@Override
	public TileEntity createNewTileEntity(World worldIn, int meta){
		
		return new TileEntityJumpPad();
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
		TileEntityJumpPad j = (TileEntityJumpPad) world.getTileEntity(pos);
		InventoryHelper.dropInventoryItems(world, pos, j);
		super.breakBlock(world, pos, state);
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
        return new BlockStateContainer(this, new IProperty[] {PROPERTYFACING, PROPERTYEXTEND, PROPERTYANGLE});
        
    }
	
	@Override
	public IBlockState getStateFromMeta(int meta){
		EnumFacing facing = EnumFacing.getHorizontal(meta);
		boolean extend = Boolean.valueOf((meta & 8) > 0);
		boolean angle = Boolean.valueOf((meta & 9) > 0);

		
		return this.getDefaultState().withProperty(PROPERTYFACING, facing).withProperty(PROPERTYEXTEND, extend).withProperty(PROPERTYANGLE, angle); 
	}
	
	@Override
	public int getMetaFromState(IBlockState state){
		EnumFacing facing = (EnumFacing)state.getValue(PROPERTYFACING);
		int bits = facing.getHorizontalIndex();
		
        if (((Boolean)state.getValue(PROPERTYEXTEND)).booleanValue())
        {
            bits |= 8;
        }
        
        if(((Boolean)state.getValue(PROPERTYANGLE)).booleanValue()){
        	bits |= 9;
        }
       
		return bits;
	}
    
	
	@Override
    public IBlockState onBlockPlaced(World worldIn, BlockPos pos, EnumFacing facing, float hitX, float hitY, float hitZ, int meta, EntityLivingBase placer)
    {
		this.facing = (placer == null) ? EnumFacing.NORTH : EnumFacing.fromAngle(placer.rotationYaw);
		
        return this.getDefaultState().withProperty(PROPERTYFACING, this.facing).withProperty(PROPERTYEXTEND, false).withProperty(PROPERTYANGLE, false);
       
    }
	
	
	
	
	@Override
    public void addCollisionBoxToList(IBlockState state, World worldIn, BlockPos pos, AxisAlignedBB entityBox, List<AxisAlignedBB> collidingBoxes, @Nullable Entity entityIn)
    {
		addCollisionBoxToList(pos, entityBox, collidingBoxes, BOUNDINGBOX);

    }
	

}
