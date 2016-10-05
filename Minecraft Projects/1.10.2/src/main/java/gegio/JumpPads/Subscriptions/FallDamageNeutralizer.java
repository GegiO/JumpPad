package gegio.JumpPads.Subscriptions;

import com.google.common.eventbus.Subscribe;

import net.minecraft.entity.EntityLiving;
import net.minecraft.entity.EntityLivingBase;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.potion.Potion;
import net.minecraftforge.event.entity.living.LivingFallEvent;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;

public class FallDamageNeutralizer {

	@SubscribeEvent
	public void fallEvent(LivingFallEvent event) {

		if (event.getEntity().getEntityWorld().isRemote)
			return;


		if (event.getEntity() instanceof EntityPlayer) {
			EntityPlayer player = (EntityPlayer) event.getEntityLiving();
			if (player.getEntityData() != null) {
				
				boolean hasJumped = player.getEntityData().getBoolean("launchType");
				if (hasJumped) {
					player.getEntityData().setBoolean("launchType", false);
					event.setCanceled(true);

				}
			}
		}
	}

}
