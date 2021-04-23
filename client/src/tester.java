import java.awt.*;

public class tester {
	public static void main (String args[]) {
		/*
		WarResult c =
			new WarResult("WAR_STATUS Aggressor BLOWER Defender STANDPIPE Winner BLOWER "+
							  "Battles 23 Status COMPLETED Weapons 21 Vehicles 18");
		*/
		/**/
		TradeResponse c =
			new TradeResponse(new ConnectionHandler(new Project(), null, 10, "Name"),
									"No Authentication", Color.yellow, 0);
		/**/
		/*
		OfferedParms c =
			new OfferedParms("Hello",
								  "blower to standpipe Weapons 6 Vehicles 3 "+
								  "Copper 5 Glass 2",
								  new Project());
		*/
		// TradeParms c = new TradeParms("Trade", 10, new Project());
		// SynthesizeParms c = new SynthesizeParms("Hello", new Project());
		// CrackParms c = new CrackParms("Hello", new Project());
		// DefendParms c = new DefendParms("Hello", "Buddy", new Project());
	   // WarParms c = new WarParms("Hello", 2, new Project());
		// WarStatusParms c = new WarStatusParms("War Status", new Project());
		// TruceParms c = new TruceParms("Hello", new Project());
      // StatusParms c = new StatusParms("Status Parms", new Project());
		// Challenge c = new Challenge(new ConnectionHandler(new Project(), null, 10, "Name"));
		// PublicKey c = new PublicKey();

		/* Help c = new Help("Help", false, "jpg/trade-request-help.png");
			 c.append("This is a test");
			 c.setSize(800,500);
			 c.setVisible(true);
		*/
		 
		/**/
		System.out.println("Value="+c.value());
		//c.dispose();
		//c = null;
		/**/
	}
}
