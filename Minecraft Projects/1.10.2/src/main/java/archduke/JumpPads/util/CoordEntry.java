package archduke.JumpPads.util;

import net.minecraft.util.math.BlockPos;

//Simple class for recording coordinates
public class CoordEntry {
	private int dimension;
	private double x,y,z;
	
	public CoordEntry(int dimension, double x, double y, double z) {
		this.dimension = dimension;
		this.x = x;
		this.y = y;
		this.z = z;
	}


	public int getDimension() {
		return dimension;
	}
	
	public BlockPos getPos(){
		return new BlockPos(x,y,z);
	}
	
	
}
