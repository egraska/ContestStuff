import java.net.*;
import java.util.*;
import java.lang.*;
import java.awt.*;
import java.io.*;
import javax.swing.*;
import java.math.*;
import java.security.*;

public class ActiveClient extends MessageParser implements Runnable {

   private Thread runner;
   private Socket socketToMonitor;
	private Log log;
   private String userName;
   public boolean toldToQuit;

   public ActiveClient(Project gameFrame) {
      super(gameFrame, gameFrame.client_log, "ac");
      parent = gameFrame;
      log = parent.client_log;
      toldToQuit = false;
   }

   // Used to set the current user, since this information is not
   // known until we are about to start the ActiveClient thread in
   // the Project class.
   public void setIdent(String ident) {  userName = ident;  }
   
   // prepare to run the thread
   public void start() {
      runner = new Thread(this);
      runner.start();
   }

	/** Without runner.stop() ident must be clicked twice after reconnecting the Client
	    the result is a deprecation warning **/
   public void stop () {  toldToQuit = true;  runner.stop(); }

   public boolean handleResult() {
      boolean debug = false;
      
      if (debug) System.out.println("Handling Result: ["+resultString+"]");
      if (resultString.equals("none")) return false;

      GLStringTokenizer resultTokens = new GLStringTokenizer(resultString);
      if (resultTokens.hasMoreTokens()) {
         String checkCommand = resultTokens.nextToken();
         if (checkCommand.equalsIgnoreCase(lastCommand)) {
            if (checkCommand.equalsIgnoreCase("PASSWORD") || 
                checkCommand.equalsIgnoreCase("CHANGE_PASSWORD")) {
               if (resultTokens.hasMoreTokens()) {
                  // get the password hash and write it to file
                  Pitbull.setAliveCookie(resultTokens.nextToken());
                  if (checkCommand.equalsIgnoreCase("CHANGE_PASSWORD")) {
                     //"officially" change to the new password
                     //(write it to file)
                     Pitbull.passwordChangeConfirmed();
                  }// END if(checkCommand.equalsIgnoreCase("CHANGE_PASSWORD")) 
               }// end if(...hasMoreTokens())
            }// end If(equals("PASSWORD"....
            else if (checkCommand.equalsIgnoreCase("GET_GAME_IDENTS")) {
               gameIdentsVector.clear();
					String str = "xxxxxxxxxxxxxxxxxxxxxxxxx";
					parent.command.cmd12_arg1.setPrototypeDisplayValue(str);
               parent.command.cmd12_arg1.removeAllItems();
               while (resultTokens.hasMoreTokens()) {
                  String next = resultTokens.nextToken();
                  gameIdentsVector.add(next);
                  parent.command.cmd12_arg1.addItem(next);
               }
					if (parent.wp != null)
						parent.updateBox(parent.wp.targets, parent.command.cmd12_arg1, false);
					if (parent.tp != null)
						parent.updateBox(parent.tp.targets, parent.command.cmd12_arg1, false);
					if (parent.kp != null)
						parent.updateBox(parent.kp.targets, parent.command.cmd12_arg1, false);
					if (parent.ap != null)
						parent.updateBox(parent.ap.targets, parent.command.cmd12_arg1, false);
					if (parent.up != null)
						parent.updateBox(parent.up.targets, parent.command.cmd12_arg1, false);
            }// end of if(...GET_GAME_IDENTS")
            else if (checkCommand.equalsIgnoreCase("PLAYER_STATUS")) {
               String currResource;
               while (resultTokens.hasMoreTokens()) {
                  currResource = resultTokens.nextToken();
                  if (resultTokens.hasMoreTokens()) {
                     parent.bank.setValue(currResource,
                                  Integer.parseInt(resultTokens.nextToken()));
                  }
               }
               // reprint reports for all resources so they display in the GUI
               parent.bank.getResourceVec();
					if (parent.wp != null) parent.wp.updateAssets(resultString);
					if (parent.tp != null) parent.tp.updateAssets(resultString);
					if (parent.sp != null) parent.sp.updateAssets(resultString);
					if (parent.yp != null) parent.yp.updateAssets(resultString);
					if (parent.kp != null) parent.kp.updateAssets(resultString);	
					if (parent.dp != null) parent.dp.updateAssets(resultString);
					if (parent.up != null) parent.up.updateAssets(resultString);
					if (parent.op != null) parent.op.updateAssets(resultString);
            } else if (checkCommand.equalsIgnoreCase("SYNTHESIZE")) {
               String currResource;
               while (resultTokens.hasMoreTokens()) {
                  currResource = resultTokens.nextToken();
                  parent.status.setText(KeepTwo.rows("AC: currResource=" + currResource,
																	  parent.status.getText()));
                  if (resultTokens.hasMoreTokens()) {
                     parent.bank.setValue(currResource,
                                  Integer.parseInt(resultTokens.nextToken()));
                  }
               }
               // reprint reports for all resources so they display in the GUI
               parent.bank.getResourceVec();
            } else if (checkCommand.equalsIgnoreCase("QUIT")) {
					toldToQuit = true;
				} else if (checkCommand.equalsIgnoreCase("SIGN_OFF")) {
               toldToQuit = true;
            } // end of else if("QUIT"||"SIGN_OFF")
            else if (checkCommand.equalsIgnoreCase("IDENT")) {
               if (parent.encryption_on) {
                  setEncryption(resultTokens.nextToken());  // arg: Mon DH half
               } else
                  setNormalTransmission();
            }// end of if("IDENT")
            else if (checkCommand.equalsIgnoreCase("WAR_STATUS")) {
				   WarResult wr = new WarResult(resultTokens.getRemaining());					
            }// end of if("WAR_STATUS")
            else if (checkCommand.equalsIgnoreCase("TRADE_REQUEST")) {
					Info info = new Info("     TRADE_REQUEST:", resultTokens.getRemaining());
            }
            else if (checkCommand.equalsIgnoreCase("WAR_DECLARE")) {
					Info info = new Info("     WAR_DECLARE:", resultTokens.getRemaining());
            }
				else if (checkCommand.equalsIgnoreCase("GET_CERTIFICATE")) {
					parent.command.arg[6].setText("Public Key Genuine?");
					try {
						String name = resultTokens.nextToken();
						String e = resultTokens.nextToken();
						String n = resultTokens.nextToken();
						String cert = resultTokens.nextToken();
						if (Pitbull.checkRSACert(cert, e, n)) {
							parent.command.arg[6].setBackground(Color.green);
							String err = new String("Public Key Is Authentic");
							parent.status.setText(KeepTwo.rows(err,
																		  parent.status.getText()));
							(new PlayClip("clear.wav")).start();
						} else {
							parent.command.arg[6].setBackground(Color.red);
							String err = new String("Certificate Does Not"+
															"Match Digest of Public Key");
							parent.status.setText(KeepTwo.rows(err,
																	   parent.status.getText()));
							(new PlayClip("blackhole.wav")).start();
						}
						Pitbull.writeRSACert(name, cert);
						try { Thread.sleep(500); } catch (Exception et) { }
						parent.command.arg[6].setBackground(new JButton().getBackground());
						parent.command.arg[6].setText("");
					} catch (Exception e) {
						parent.command.arg[6].setBackground(Color.yellow);
						parent.status.setText(KeepTwo.rows(e.toString(),
																	   parent.status.getText()));
						parent.status.setText(KeepTwo.rows("Public Key Authenticity"+
																	  "is inconclusive",
																	   parent.status.getText()));
						try { Thread.sleep(500); } catch (Exception et) { }
						parent.command.arg[6].setBackground(new JButton().getBackground());
						parent.command.arg[6].setText("");
					}
				}
            return true;
         }
      } 
      return false;
   } // end of handleResult

   public void run() {
      try {
			int mprt;
         String name = parent.monlocation.getText();
         Globals.MONITOR_NAME = name;
         String host = parent.monport.getText();
			try { mprt = Integer.parseInt(host); } catch (Exception e) {
				Info info = new Info(" Number Format Exception:",
											"Monitor port field is not a number");
				parent.status.setText(KeepTwo.rows("Monitor port is not a number - please edit",
															  parent.status.getText()));
				return;
			}
			
         Globals.MONITOR_PORT = mprt;
         socketToMonitor = new Socket(Globals.MONITOR_NAME,Globals.MONITOR_PORT);
         log.append("[ac] socket created\n");

			try {
				out = new PrintWriter(socketToMonitor.getOutputStream(), true);
				InputStream is = socketToMonitor.getInputStream();
				in = new BufferedReader(new InputStreamReader(is));
			} catch (Exception ex) {
				Info info = new Info(" Unknown Host Exception:",
											"Monitor location field does not represent the "+
											"monitor location");
				parent.status.setText(KeepTwo.rows("Monitor location field does not represent"+
															  " the monitor location - please edit",
															  parent.status.getText()));
				return;
			}

         getMonitorMessage(null);
         if (abort) terminate();
         handleResult();
         while (!toldToQuit) {
				while (!execute(parent.buildCmd(parent.command.command())));
            getMonitorMessage(null);
            if (abort) break;
            
            //
            //  At best, this is a kludge. The current scheme is to 
            //  get all of the messages and then once, we have them
            //  we should handle the results.  The problem with the IDENT
            //  encryption is that the encryption is enable half way through
            //  the reponse, so what I did is once getMonitorMessage() notices
            //  that it got a RESULT: IDENT XXXX, it pops up the kludge flag
            //  and then ends.  Thus, we must in the active client, process 
            //  that result and then get the rest of the response because it 
            //  termintated early.  Sorry for the long explanation, but this
            //  makes it clear.
            //
            if (ShouldRunIDENTKludge) {
               handleResult();
               getMonitorMessage(null);
               ShouldRunIDENTKludge = false;
            }
            
            handleResult();
         }
         terminate();
      } catch (UnknownHostException e) {
			Info info = new Info(" Unknown Host Exception:",
										"Monitor location field does not represent the "+
										"monitor location");
			parent.status.setText(KeepTwo.rows("Monitor location field does not represent"+
														  " the monitor location - please edit",
														  parent.status.getText()));
			return;
      } catch (Exception e) {
         parent.status.setText(KeepTwo.rows("ActiveClient [run]: "+e.toString(),
														  parent.status.getText()));
		} // end catch Exception
   } // end method run

   public void terminate () {
      try {
         if (socketToMonitor != null) socketToMonitor.close();
         if (parent.srv.conhan != null) {
            parent.server_log.append("[srv] closing down\n");
            if (parent.srv.conhan.in != null) parent.srv.conhan.in.close();
            if (parent.srv.conhan.out != null) parent.srv.conhan.out.close();
         }
         
         log.append("[ac] socket closed!\n");
         
         if (out != null) out.close(); 
         if (in != null) in.close();
         setNormalTransmission();
         // reset the quit boolean
         toldToQuit = false;
      } catch (Exception e) {
         parent.status.setText(KeepTwo.rows("ActiveClient [run]: "+e.toString(),
														  parent.status.getText()));
      } // end catch Exception
   }
} // end class ActiveClient
