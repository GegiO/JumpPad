package archduke.JumpPads.util;

import archduke.JumpPads.Reference;
import net.minecraft.entity.player.EntityPlayer;

public class LaunchHelper {
	
	public LaunchHelper(){
		
	}
	
	public LaunchContainer getLaunchInfo(double posx, double posy, double posz, EntityPlayer playerIn){
		
		double changeX = posx - playerIn.posX;
		double changeY = posy - playerIn.posY;
		double changeZ = posz - playerIn.posZ;
		double gravity = Reference.MINECRAFT_GRAVITY;
		
		
		double horizontalDistance = Math.sqrt(Math.pow(changeX, 2) + Math.pow(changeZ, 2));
		
		double pitch = Reference.ANGLE;
		double yaw = Math.atan(Math.abs(changeX)/Math.abs(changeZ));
		
		double uVecZ = Math.cos(yaw) *  Math.cos(pitch) * Math.signum(changeZ);
		double uVecX = Math.sin(yaw) * Math.cos(pitch) * Math.signum(changeX);
		double uVecY = Math.sin(pitch);
		
		double horizontalDistance2 = Math.pow(horizontalDistance, 2);
		double cos2 = Math.pow(Math.cos(pitch), 2);
		
		double launchSpeed = Math.sqrt((gravity * horizontalDistance2)/(2*cos2 * (horizontalDistance * Math.tan(pitch) - Math.abs(changeY))));
		
		return new LaunchContainer(uVecX, uVecY, uVecZ, launchSpeed * 1.2);
	}

}
