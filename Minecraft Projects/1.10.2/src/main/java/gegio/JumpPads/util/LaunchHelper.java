package gegio.JumpPads.util;

import gegio.JumpPads.Reference;
import net.minecraft.entity.player.EntityPlayer;
import net.minecraft.util.math.BlockPos;

public class LaunchHelper {
	
	public LaunchHelper(){
		
	}
	
	public LaunchContainer getLaunchInfo(BlockPos padPos, BlockPos targetPos){
		
		double changeX = targetPos.getX() - padPos.getX();
		double changeY = targetPos.getY() - padPos.getY();
		double changeZ = targetPos.getZ() - padPos.getZ();
		double gravity = Reference.MINECRAFT_GRAVITY;
		
		double time = 20;
		
		double xVelocity = Math.floor(changeX)/time;
		double zVelocity = Math.floor(changeZ)/time;
		double yVelocity = (changeY/time) + .08 * time;
		
		return new LaunchContainer(xVelocity * .85
								 , yVelocity
								 , zVelocity * .85);
	}

}
