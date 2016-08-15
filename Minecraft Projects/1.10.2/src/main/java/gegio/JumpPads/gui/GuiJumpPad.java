package gegio.JumpPads.gui;

import gegio.JumpPads.Blocks.JumpPadContainer;
import gegio.JumpPads.tileentities.TileEntityJumpPad;
import net.minecraft.client.Minecraft;
import net.minecraft.client.gui.inventory.GuiContainer;
import net.minecraft.client.renderer.GlStateManager;
import net.minecraft.entity.player.InventoryPlayer;
import net.minecraft.inventory.Container;
import net.minecraft.util.ResourceLocation;
import net.minecraftforge.fml.relauncher.Side;
import net.minecraftforge.fml.relauncher.SideOnly;

@SideOnly(Side.CLIENT)
public class GuiJumpPad extends GuiContainer {
	
	private static final ResourceLocation jumpPadGuiTextures = new ResourceLocation("jumppads", "textures/gui/jumppadgui.png");
	private TileEntityJumpPad tileEntityJumpPad;

	public GuiJumpPad(InventoryPlayer invPlayer, TileEntityJumpPad tileEntityJumpPad) {
		super(new JumpPadContainer(invPlayer,tileEntityJumpPad));
		
		this.tileEntityJumpPad = tileEntityJumpPad;		
		
	}

	@Override
	protected void drawGuiContainerBackgroundLayer(float partialTicks, int mouseX, int mouseY) {
		Minecraft.getMinecraft().getTextureManager().bindTexture(jumpPadGuiTextures);
		GlStateManager.color(1.0f, 1.0f, 1.0f);
		drawTexturedModalRect(guiLeft,guiTop,0,0,xSize,ySize);
	}

}
