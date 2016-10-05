package gegio.JumpPads.util;

//simple class for recording unit vectors
public class LaunchContainer {
	double unitX;
	double unitY;
	double unitZ;
	
	public LaunchContainer(double unitX, double unitY, double unitZ) {
		super();
		this.unitX = unitX;
		this.unitY = unitY;
		this.unitZ = unitZ;

	}

	public double getUnitX() {
		if(!Double.isNaN(unitX))
			return unitX;
		else
			return 0;
	}

	public double getUnitY() {
		if(!Double.isNaN(unitY))
			return unitY;
		else
			return 0;
	}

	public double getUnitZ() {
		if(!Double.isNaN(unitZ))
			return unitZ;
		else
			return 0;
	}

}
