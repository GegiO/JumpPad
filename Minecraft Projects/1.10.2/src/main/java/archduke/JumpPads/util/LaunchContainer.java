package archduke.JumpPads.util;

//simple class for recording unit vectors
public class LaunchContainer {
	double unitX;
	double unitY;
	double unitZ;
	double launchSpeed;
	
	public LaunchContainer(double unitX, double unitY, double unitZ, double LaunchSpeed) {
		super();
		this.unitX = unitX;
		this.unitY = unitY;
		this.unitZ = unitZ;
		this.launchSpeed = LaunchSpeed;
	}

	public double getUnitX() {
		return unitX;
	}

	public double getUnitY() {
		return unitY;
	}

	public double getUnitZ() {
		return unitZ;
	}
	
	public double getLaunchSpeed(){
		return launchSpeed;
	}

	
	
	
	
}
