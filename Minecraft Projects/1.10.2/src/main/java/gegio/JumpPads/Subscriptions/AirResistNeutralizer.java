package gegio.JumpPads.Subscriptions;

import java.util.Random;

import gegio.JumpPads.Reference;
import net.minecraft.client.Minecraft;
import net.minecraft.entity.EntityLiving;
import net.minecraft.entity.EntityLivingBase;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.util.EnumParticleTypes;
import net.minecraft.util.math.BlockPos;
import net.minecraftforge.event.entity.living.LivingEvent;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;

public class AirResistNeutralizer {
	
	public static final double X_AIR_RESIST = 0.91D;
	public static final double Z_AIR_RESIST = 0.91D;
	public static final double Y_AIR_RESIST = 0.9800000190734863D;
	
	public static final double X_AIR_RESIST_3RD = 0.98662D;
	public static final double Z_AIR_RESIST_3RD = 0.98662D;
	public static final double Y_AIR_RESIST_3RD = 0.99712D;
	

	@SubscribeEvent
	public void LivingUpdateEvent(LivingEvent entityEvent) {
		EntityLivingBase entityLiving = entityEvent.getEntityLiving();

		if (entityLiving instanceof EntityPlayer) {
			if(entityLiving.getEntityData() != null){
				
				EntityPlayer player = (EntityPlayer) entityLiving;
				boolean hasJumped = player.getEntityData().getBoolean("hasJumped");
				boolean launchType = player.getEntityData().getBoolean("launchType");
				
				if(!player.onGround && !player.capabilities.isFlying && hasJumped && !player.isElytraFlying()){
					if (Minecraft.getMinecraft().gameSettings.thirdPersonView == 0 && entityLiving.worldObj.isRemote) {
						player.motionX /= X_AIR_RESIST;
						player.motionZ /= Z_AIR_RESIST;
						player.motionY /= Y_AIR_RESIST;
					} 
					else if(Minecraft.getMinecraft().gameSettings.thirdPersonView > 0 && entityLiving.worldObj.isRemote){
						player.motionX /= X_AIR_RESIST_3RD;
						player.motionZ /= Z_AIR_RESIST_3RD;
						player.motionY /= Y_AIR_RESIST_3RD;
					}
				}
				
				if (launchType) {
					if(Math.floor(player.getEntityData().getDouble("targetX")) == Math.floor(player.posX) 
							&& Math.floor(player.getEntityData().getDouble("targetZ")) == Math.floor(player.posZ) 
							&& player.posY - player.getEntityData().getDouble("targetY") > 2){
						player.setVelocity(0, player.motionY, 0);
						player.getEntityData().setBoolean("hasJumped", false);
						player.getEntityData().setDouble("targetX", 0);
						player.getEntityData().setDouble("targetZ", 0);
						
					}
				}
			}
		}
		
	}

}
