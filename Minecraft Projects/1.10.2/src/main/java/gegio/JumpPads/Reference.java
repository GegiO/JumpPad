package gegio.JumpPads;

import java.io.File;

import gegio.JumpPads.proxy.CommonProxy;
import net.minecraftforge.common.config.Configuration;

public class Reference {
	
	public static final String MOD_ID = "jumppads";
	public static final String MOD_NAME = "JumpPads";
	public static final String MOD_VERSION = "1.0.0";
	public static final String CLIENT_PROXY_CLASS = "gegio.JumpPads.proxy.ClientProxy";
	public static final String SERVER_PROXY_CLASS = "gegio.JumpPads.proxy.CommonProxy";
	
	public static final double MINECRAFT_GRAVITY = 0.08;
}
