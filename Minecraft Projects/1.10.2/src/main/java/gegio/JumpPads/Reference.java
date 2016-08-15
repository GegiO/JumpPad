package gegio.JumpPads;

public class Reference {
	
	public static final String MOD_ID = "jumppads";
	public static final String MOD_NAME = "JumpPads";
	public static final String MOD_VERSION = "1.0.0";
	public static final String CLIENT_PROXY_CLASS = "gegio.JumpPads.proxy.ClientProxy";
	public static final String SERVER_PROXY_CLASS = "gegio.JumpPads.proxy.CommonProxy";
	
	
	public static final int LAUNCH_SPEED_MAX = 4;
	public static final double MINECRAFT_GRAVITY = 0.08;
	
	//angle in degrees
	public static final int ANGLE_DEGS = 45;
	
	//angle in radians
	public static final double ANGLE = ANGLE_DEGS * Math.PI/180;
	

}
