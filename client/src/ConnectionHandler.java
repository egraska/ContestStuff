import java.net.*;
import java.util.*;
import java.lang.*;
import javax.swing.*;
import java.io.*;
import java.awt.*;
import java.math.*;
import java.security.*;

public class ConnectionHandler extends MessageParser implements Runnable {
   private Socket incoming = null;
   private int counter; 
   private Thread runner;
   private Project parent;
	private Log log;
   private String userName;
	Trade trade = null;
	Color genuineColor = Color.white;
	Color tradeColor = Color.white;
	BigInteger challenge;
	String tmp_aggressor = null;
	String tmp_offer = null;	
   
   public ConnectionHandler (Project gameFrame, Socket i, int c, String name) {
      super(gameFrame, gameFrame.server_log, "srv" + c);
      parent = gameFrame;
      log = parent.server_log;
      incoming = i;
      counter = c; 
      userName = name;
   }

   // automatic login -- used by Server class
   public boolean login() {
      getMonitorMessage(this);
      if (abort) stop();
      consumeDirectives();
      return handleResult();
   } // end method login
   
   // manages the response to a message from the Monitor
   public void consumeDirectives() {
      while (!requireString.equals("none")) {
         handleResult();
         handleRequire();
         getMonitorMessage(this);
         if (ShouldRunIDENTKludge) {
            handleResult();
            getMonitorMessage(null);
            ShouldRunIDENTKludge = false;
         }
      } // end while
   } // end method consumeDirectives
   
   public boolean handleResult() {
      if (resultString.equals("none")) {
         return false;
      }
      GLStringTokenizer resultTokens = new GLStringTokenizer(resultString);
      if (resultTokens.hasMoreTokens()) {
         String checkCommand = resultTokens.nextToken();
         if (checkCommand.equalsIgnoreCase("TIME_LEFT")) {
            if (resultTokens.hasMoreTokens()) {
               parent.timeleft.setText(resultTokens.nextToken());
               return true;
            } else {
               parent.timeleft.setText("unknown");
               return false;
            }
         } else if (checkCommand.equalsIgnoreCase(lastCommand)) {
            if (checkCommand.equalsIgnoreCase("PASSWORD") || 
                checkCommand.equalsIgnoreCase("CHANGE_PASSWORD")) {
               if (resultTokens.hasMoreTokens()) {
                  // get the password hash and write it to file
                  Pitbull.setAliveCookie(resultTokens.nextToken());
                  if (checkCommand.equalsIgnoreCase("CHANGE_PASSWORD")) {
                     //"officially" change to the new password
                     //(write it to file)
                     Pitbull.passwordChangeConfirmed();
                  }
               }
            } else if (checkCommand.equalsIgnoreCase("GET_GAME_IDENTS")) {
               gameIdentsVector.clear();
					String str = "xxxxxxxxxxxxxxxxxxxxxxxxx";
					parent.command.cmd12_arg1.setPrototypeDisplayValue(str);
               parent.command.cmd12_arg1.removeAllItems();
               while (resultTokens.hasMoreTokens()) {
						String next = resultTokens.nextToken();
                  gameIdentsVector.add(next);
						parent.command.cmd12_arg1.addItem(next);
               }
					
            } else if (checkCommand.equalsIgnoreCase("SIGN_OFF")) {
					parent.server_log.append("Sign Off\n");
            } else if (checkCommand.equalsIgnoreCase("WAR_STATUS")) {
				   WarResult wr = new WarResult(resultTokens.getRemaining());
				} else if (checkCommand.equalsIgnoreCase("IDENT")) {
               if (parent.encryption_on) {
                  setEncryption(resultTokens.nextToken());  // arg: Mon DH half
               } else
                  setNormalTransmission();
				}
            return true;
         } // end if checkCommand equals lastCommand
      }
      return false;
   } // end method handleResult

   // automatically responds to Monitor directives
   public boolean handleRequire() {
      // error-check
      if (requireString == null) {
         requireString = "none";
         return false;
      }
      // regular processing (several cases)
      if (requireString.equalsIgnoreCase("IDENT")) {
			if (parent.encryption_on) {
				getDHPublicKey();
				String clientDHhalf = dhe.getExchangeKey();
				execute("IDENT "+ userName + " " + clientDHhalf);
			} else 
				execute("IDENT " + userName);
      } else if (requireString.equalsIgnoreCase("QUIT")) {
         execute("QUIT");
      } else if (requireString.equalsIgnoreCase("PASSWORD")) {
         execute("PASSWORD " + Pitbull.getPassword());
      } else if (requireString.equalsIgnoreCase("CHANGE_PASSWORD")) {
         execute("CHANGE_PASSWORD " + Pitbull.getPassword() + 
                 " " + Pitbull.newPassword());
      } else if (requireString.equalsIgnoreCase("HOST_PORT")) {
         execute("HOST_PORT " + parent.PLAYER_HOST_NAME + " " + 
                 parent.srvport.getText());
      } else if (requireString.equalsIgnoreCase("SIGN_OFF")) {
         execute("SIGN_OFF");
      } else if (requireString.equalsIgnoreCase("ALIVE")) {
         execute("ALIVE " + Pitbull.getAliveCookie());
		} else if (requireString.equalsIgnoreCase("TRADE_RESPONSE OR CHALLENGE")) {
			Challenge chal = new Challenge(this);
			int response = chal.value();
			chal.dispose();
			chal = null;
			if (response == 1) {
				SecureRandom r = new SecureRandom();
				try {
					challenge = new BigInteger(parent.challenge_length, 100, r);
				} catch (Exception e) {
					challenge = new BigInteger("0");
				}
				String arg = String.valueOf(challenge);
				execute("CHALLENGE " + arg);  // The server sends this back.
			} else if (response == 2) {
				execute("TRADE_RESPONSE ACCEPT");
				trade = null;
			} else if (response == 3) {
				execute("TRADE_RESPONSE DECLINE");
				trade = null;
			}
		} else if (requireString.equalsIgnoreCase("TRADE_RESPONSE OR ROUNDS")) {
			Rounds rnds = new Rounds(this);
 			int response = rnds.value();
			rnds.dispose();
			rnds = null;
			if (response > 0) {
				if (trade == null) trade = new Trade();
				trade.addRounds(String.valueOf(response));
				execute("ROUNDS " + response);  // The server sends this back.
			} else if (response == -2) {
				execute("TRADE_RESPONSE ACCEPT");
				trade = null;
			} else if (response == -3) {
				execute("TRADE_RESPONSE DECLINE");
				trade = null;
			}
		} else if (requireString.equalsIgnoreCase("TRADE_RESPONSE")) {
			TradeResponse tr = null;
			Color zkpColor;
			if (trade.keyType != null) {	
				String keyType = trade.keyType;
				if (keyType.equals("RSA")) {
					tr = new TradeResponse(this, "Challenge Response OK?", tradeColor, 0);
				} else if (keyType.equals("ZKP")) {
					zkpColor = trade.check();
					tr = new TradeResponse(this, "ZKP Response OK?", zkpColor, 0);
				} else if (keyType.equals("NONE")) {
					tr = new TradeResponse(this, "No Authentication", Color.yellow, 1);
				}
			} else {
				tr = new TradeResponse(this, "Choose response", Color.cyan, 1);
			}
			int response = tr.value();
			tr.dispose();
			tr = null;
			if (response == 1) {
				execute("TRADE_RESPONSE ACCEPT");
			} else {
				execute("TRADE_RESPONSE DECLINE");
			}
			trade = null;
		} else if (requireString.equalsIgnoreCase("CHALLENGE_RESPONSE")) {
			RSA rsa = Pitbull.readUserRSAKey();
			BigInteger response = rsa.encryptNum(challenge);
			execute("CHALLENGE_RESPONSE " + response);
			parent.challenge = null;
      } else if (requireString.equalsIgnoreCase("AUTHORIZE_SET")) {
			BigInteger sqr = new BigInteger("2", 10);
			BigInteger n = new BigInteger(trade.getMod());
			String r = trade.getRounds();
			int rounds = Integer.parseInt(r);
			if (rounds < 4 || rounds > 20) rounds = 4;
			Random rnd = new Random();
			trade.R = new BigInteger[rounds];
			String cmdToBuild = "AUTHORIZE_SET ";
			for (int i = 0; i < rounds; i++) {
				trade.R[i] = new BigInteger(parent.challenge_length/4, rnd);
				cmdToBuild += " " + trade.R[i].modPow(sqr,n).toString();
			}
			execute(cmdToBuild);
		} else if (requireString.equalsIgnoreCase("SUBSET_K")) {
			BigInteger sqr = new BigInteger("2", 10);
			BigInteger n = new BigInteger(trade.getMod());
			BigInteger s = new BigInteger(trade.getDec());
			int rnds = Integer.parseInt(trade.rounds);
			String cmdToBuild = "SUBSET_K ";
			for (int i=0 ; i < rnds ; i++) {
				if (trade.A[i])
					cmdToBuild += trade.R[i].multiply(s).mod(n) + " ";
			}
			execute(cmdToBuild);			
		} else if (requireString.equalsIgnoreCase("SUBSET_J")) {
			BigInteger n = new BigInteger(trade.getMod());
			
			int rnds = Integer.parseInt(trade.rounds);
			String cmdToBuild = "SUBSET_J ";
			for (int i=0 ; i < rnds ; i++) {
				if (!trade.A[i]) cmdToBuild += trade.R[i] + " ";
			}
			execute(cmdToBuild);
		} else if (requireString.equals("WAR_DEFEND")) {
			parent.dp = new DefendParms("War Defend", tmp_aggressor, parent);
			String value = parent.dp.value();
			parent.dp.dispose();
			parent.dp = null;
			tmp_aggressor = null;

			if (value != null && !value.equals("")) {
				execute(value);
			} else {
				parent.status.setText(KeepTwo.rows("WAR_DEFEND failed",
															  parent.status.getText()));
			}
      } else if (requireString.equalsIgnoreCase("WAR_TRUCE_RESPONSE")) {
			parent.op = new OfferedParms("War Truce Offer", tmp_offer, parent);
			String value = parent.op.value();
			parent.op.dispose();
			parent.op = null;
			tmp_offer = null;

			if (value != null && !value.equals("")) {
				execute(value);
			} else {
				parent.status.setText(KeepTwo.rows("TRUCE_RESPONSE failed",
															  parent.status.getText()));
			}

      } else { // something's wrong!
         requireString = "none";
         return false;
      }
      return true;
   } // end method handleRequire
   
   public void start() {
      if (runner == null) {
         runner = new Thread(this);
         runner.start();
      }
   } // end method start

   public void stop() {
      try {
         if (incoming != null) incoming.close();
      } catch (Exception e) {
			parent.server_log.append("connection handler socket closed\n");
      }
      // runner.stop();   <------- this may have to be uncommented
   }
   
   public void run() {
      try {
			if (incoming != null) {
				InputStream is = incoming.getInputStream();
				in = new BufferedReader(new InputStreamReader(is));
				out = new PrintWriter (incoming.getOutputStream(),true);
				if (login()) log.append("[srv" + counter + "]: Logged In!" + "\n");
				log.append("[srv"+counter+"]: Terminating session\n");
				incoming.close();
			} else {
				in = null;
				out = null;
			}
      } catch (IOException e) {
      } catch (NullPointerException n) { }
   } // end method run
   
} // end class ConnectionHandler
