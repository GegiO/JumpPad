package gegio.JumpPads.Blocks;

import gegio.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraft.client.gui.GuiScreen;
import net.minecraft.client.gui.inventory.GuiContainer;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.entity.player.InventoryPlayer;
import net.minecraft.inventory.Container;
import net.minecraft.inventory.IInventory;
import net.minecraft.inventory.Slot;
import net.minecraft.item.ItemStack;
import net.minecraft.util.ResourceLocation;

public class JumpPadContainer extends Container {
	

	
	private final int HOTBAR_SLOTS = 9;
	private final int PLAYER_INVENTORY_ROW = 3;
	private final int PLAYER_INVENTORY_COLUMN = 9;
	private final int PLAYER_INVENTORY_TOTAL = PLAYER_INVENTORY_ROW * PLAYER_INVENTORY_COLUMN;
	private final int TOTAL_VANILLA_SLOTS = PLAYER_INVENTORY_TOTAL + HOTBAR_SLOTS;
	
	private final int VANILLA_INDEX = 0;
	private final int JUMPPAD_INVENTORY_INDEX = VANILLA_INDEX + TOTAL_VANILLA_SLOTS;
	private final int JUMPPAD_SLOT_COUNT = 3;
	
	private TileEntityJumpPad tileEntityJumpPad;
	
	public JumpPadContainer(InventoryPlayer invPlayer, TileEntityJumpPad tileEntityJumpPad){
		this.tileEntityJumpPad = tileEntityJumpPad;
		
		final int SLOT_X_SPACING = 18;
		final int SLOT_Y_SPACING = 18;
		final int HOTBAR_XPOS = 8;
		final int HOTBAR_YPOS = 142;
		
		for(int x = 0; x < this.HOTBAR_SLOTS; x++){
			addSlotToContainer(new Slot(invPlayer, x, HOTBAR_XPOS + (SLOT_X_SPACING * x), HOTBAR_YPOS));
		}
		
		final int PLAYER_INVENTORY_XPOS = 8;
		final int PLAYER_INVENTORY_YPOS = 84;
		
		for(int y=0; y < PLAYER_INVENTORY_ROW; y++){
			for(int x=0; x < PLAYER_INVENTORY_COLUMN; x++){
				int slotNumber = HOTBAR_SLOTS + y * PLAYER_INVENTORY_COLUMN + x;
				int xpos = PLAYER_INVENTORY_XPOS + x * SLOT_X_SPACING;
				int ypos = PLAYER_INVENTORY_YPOS + y * SLOT_Y_SPACING;
				addSlotToContainer(new Slot(invPlayer,slotNumber,xpos,ypos));
			}
		}
		
		if(JUMPPAD_SLOT_COUNT != tileEntityJumpPad.getSizeInventory()){
			System.err.println("Mismatched slot count for tiletentity and container");
		}
		
		final int TILE_INVENTORY_SLOT1_XPOS = 33;
		
		final int TILE_INVENTORY_SLOT2_XPOS = 80;

		final int TILE_INVENTORY_SLOT3_XPOS = 127;
		
		final int TILE_INVENTORY_YPOS = 37;
		
		addSlotToContainer(new Slot(tileEntityJumpPad,0,TILE_INVENTORY_SLOT1_XPOS,TILE_INVENTORY_YPOS));
		addSlotToContainer(new Slot(tileEntityJumpPad,1,TILE_INVENTORY_SLOT2_XPOS,TILE_INVENTORY_YPOS));
		addSlotToContainer(new Slot(tileEntityJumpPad,2,TILE_INVENTORY_SLOT3_XPOS,TILE_INVENTORY_YPOS));
		
		
	}
	
	@Override
	public boolean canInteractWith(EntityPlayer playerIn) {
		return tileEntityJumpPad.isUseableByPlayer(playerIn);
	}
	
	@Override 
	public ItemStack transferStackInSlot(EntityPlayer player, int sourceSlotIndex){
		return null;
	}

	
	
}
