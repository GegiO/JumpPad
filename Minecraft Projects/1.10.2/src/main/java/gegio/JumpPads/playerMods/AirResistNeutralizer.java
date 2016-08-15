package gegio.JumpPads.playerMods;

import gegio.JumpPads.Reference;
import net.minecraft.client.Minecraft;
import net.minecraft.entity.EntityLiving;
import net.minecraft.entity.EntityLivingBase;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraftforge.event.entity.living.LivingEvent;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;

public class AirResistNeutralizer {
	
	public static final float X_AIR_RESIST = 0.91F;
	public static final float Z_AIR_RESIST = 0.91F;
	public static final double Y_AIR_RESIST = 0.98D;
	
	public static final float X_AIR_RESIST_3RD = 0.98662F;
	public static final float Z_AIR_RESIST_3RD = 0.98662F;
	public static final double Y_AIR_RESIST_3RD = 0.99712D;
	
	public int resistTimeout = 0;

	@SubscribeEvent
	public void LivingUpdateEvent(LivingEvent entityEvent) {
		EntityLivingBase entityLiving = entityEvent.getEntityLiving();

		if (entityLiving instanceof EntityPlayer && entityLiving.worldObj.isRemote) {
			if(entityLiving.getEntityData() != null){

				EntityPlayer player = (EntityPlayer) entityLiving;
				boolean hasJumped = player.getEntityData().getBoolean("hasJumped");
				
				
				if (!player.onGround && !player.capabilities.isFlying && hasJumped && Minecraft.getMinecraft().gameSettings.thirdPersonView == 0) {
					player.motionX /= X_AIR_RESIST;
					player.motionZ /= Z_AIR_RESIST;
					player.motionY /= Y_AIR_RESIST;
					resistTimeout++;
				} else if(!player.onGround && !player.capabilities.isFlying && hasJumped && Minecraft.getMinecraft().gameSettings.thirdPersonView > 0){
					player.motionX /= X_AIR_RESIST_3RD;
					player.motionZ /= Z_AIR_RESIST_3RD;
					player.motionY /= Y_AIR_RESIST_3RD;
					resistTimeout++;
				} else if(player.motionY == -0.0784000015258789 || resistTimeout == 40){
					player.getEntityData().setBoolean("hasJumped", false);
				}
			}
		}
	}

}
