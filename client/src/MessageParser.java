import java.security.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import java.awt.*;
import java.math.*;

/**
 * IWar is a game-player in Dr. Franco's I-Wars Competition.
 *
 *@author John Franco
/* @version 010819
 *
 */

public class MessageParser {
   static Project parent;

   protected PrintWriter out;
   protected BufferedReader in;
   protected KarnBufferedReader karnIn;
   protected KarnPrintWriter karnOut;
   protected String requireString;
   protected String resultString;
   protected String errorString;
   protected String checksumString;
   protected String commentString;
   protected Vector <String> gameIdentsVector;
   protected String lastCommand;
   protected String subclassType;
   protected boolean UsingEncryption;
   protected boolean ShouldRunIDENTKludge;
   boolean abort = false;
   static DHE dhe = null;
   static FileInputStream fin = null;
	BigInteger chal_response = null;
	static boolean keysWarn = false;
	
   // initialized by ActiveClient or Server class
   //protected JTextArea log;
	protected Log log;

   public MessageParser(Project project, Log log, String typeIndicator) {
		parent = project;
      this.log = log;
      requireString = "none";
      resultString = "none";
      gameIdentsVector = new Vector <String> ();
      UsingEncryption = false;
      subclassType = typeIndicator;
      ShouldRunIDENTKludge = false;
   } // end constructor

   static public String getSetup (String key) {
      boolean debug = false;

      String str, ret = "";
      
      if (debug)
         System.out.println("Message Parser [getSetup]: get parameter");

      try {
         // Read the setup parameter
         fin = new FileInputStream("Parms");
         BufferedReader br = new BufferedReader(new InputStreamReader(fin));
         while ((str = br.readLine()) != null) {
            if (str.equals(key)) ret = br.readLine();
         }
         fin.close();
         
         if (debug)
				System.out.println("Message Parser [getSetup]: return: "+ret);
         
			return ret;
      } catch (Exception e) {
			keysWarn = true;
      }
		return "";
   }

   // Used by setEncryption this is where the file is read for the DH public key
   // and a Diffie-Hellman exchange object is created from that key.
   static public void getDHPublicKey () {
      boolean debug = false;

      BigInteger p=null, g=null;
      String str;
      
      if (debug)
         System.out.println("Message Parser [getDHPublicKey]: get DH keys");

      try {
         // Read the DH Key parameters from file
         fin = new FileInputStream("Parms");
         BufferedReader br = new BufferedReader(new InputStreamReader(fin));
         while ((str = br.readLine()) != null) {
            if (str.equals("# Diffie-Hellman p")) {
               p = new BigInteger(br.readLine());
            }
            if (str.equals("# Diffie-Hellman g")) {
               g = new BigInteger(br.readLine());
            }
         }
         fin.close();
         
         if (debug) System.out.println("p="+p+" g="+g);
         
         if (debug) System.out.println("Create DHE object with dhk keys");
         dhe = new DHE(p,g);  // Start the DHE Object
      } catch (Exception e) {
         parent.status.setText(KeepTwo.rows("MessageParser [getDHPublicKey]: " +
														  "error:\n\t" + e.toString(),
														  parent.status.getText()));
         parent.status.setText(KeepTwo.rows("MessageParser [getDHPublicKey]: " +
														  e.toString(),
														  parent.status.getText()));
      }
   }
   
   // Creates a Diffie-Hellman object that reads public keys from a file,
   // creates a Client Diffie-Hellman half and computes a shared secret
   // using the Monitor's Diffie-Hellman half and the Client's.  The shared
   // key is input to the constructors of KarnBufferedReader and
   // KarnPrintWriter which are used like BufferedReader and PrintWriter
   // except that the messages going out from the KarnPrintWriter are
   // encrypted and the messages coming in from KarnBufferedReader are
   // decrypted.
   public void setEncryption(String monDHhalf) {
      boolean debug = false;
      
      try {
         if (dhe == null) getDHPublicKey();
         if (debug)
            System.out.println("Message Parser [setEncryption]: make sharedkey");
         
         dhe.setExchangeKey(monDHhalf);
         BigInteger sharedKey = dhe.getSharedKey();
			parent.command.arg[1].setText(dhe.getExchangeKey());
         
         if (debug)
            System.out.println("Message Parser [setEncryption]: shared key:"+
                               sharedKey);
         
         karnIn = new KarnBufferedReader(in, sharedKey);
         karnOut = new KarnPrintWriter(out, true, sharedKey);
         UsingEncryption = true;
         parent.status.setText(KeepTwo.rows("MessageParser: Encryption is "+
													  "turned on", parent.status.getText()));
      } catch (Exception e) {
			parent.status.setText(KeepTwo.rows("setEncryption has failed with "+e.toString(),
														  parent.status.getText()));
      }
   }

   // Turns encryption off
   public void setNormalTransmission() {
      karnIn = null;
      karnOut = null;
      UsingEncryption = false;
		parent.command.arg[1].setText("");
		parent.encryption_on = false;
      parent.status.setText(KeepTwo.rows("MessageParser: Encryption is turned "+
													  "off", parent.status.getText()));
   }

   // Pulls a complete message group from the Monitor, either by
   // karnIn.readLine() if encryption is used or by in.readLine() if it
   // is not.  The message group is scanned for directives and results.
   // If the directive is WAITING, QUIT, or SIGN_OFF no further action
   // takes place.  If the directive is REQUIRE, COMMAND_ERROR or COMMENT
   // then a require, error, or comment string is constructed and later
   // processed by handleResult().  If the directive is IDENT a special
   // function in handleResult() is called - see why in ActiveClient.java.
   // If the directive is PLAYER_PASSWORD_CHECKSUM then the Monitor is
   // authenticated or it is not in which case the handshake (the process
   // where the checksum is given to the Client for authentication) is
   // terminated.
   public void getMonitorMessage(ConnectionHandler handler) {
      String currLine = "";
      boolean messageComplete = false;
      String directiveString = "";
      boolean foundRequire = false;
      boolean foundCommandError = false;
      boolean foundComment = false;
      boolean foundResult = false;
      boolean foundChecksum = false;
      abort = false;
		String cmdToBuild = null;

      while (!messageComplete) {
         try {
            // check for encryption
				if (!UsingEncryption)
					currLine = in.readLine();
				else
					currLine = karnIn.readLine();
            
            if (currLine != null) {
					log.append("[MON] " + currLine + "\n");
					if (parent.accessible) System.out.println("From the monitor : "+currLine);
				}
            
            if (currLine != null && !currLine.equals("\r")) {
               GLStringTokenizer monitorTokens = new GLStringTokenizer(currLine);
               if (monitorTokens.hasMoreTokens()) {
                  directiveString = monitorTokens.nextToken();
               } else {
                  messageComplete = true;
               }

               // process the current directive (several cases)
               if (directiveString.equalsIgnoreCase("WAITING:")) {
						if (cmdToBuild != null) execute(cmdToBuild);
						cmdToBuild = null;
                  messageComplete = true;
               } else if (directiveString.equalsIgnoreCase("REQUIRE:")) {
                  foundRequire = true;
                  requireString = monitorTokens.getRemaining();
                  if (requireString == null || requireString.equals("")) {
                     requireString = "none";
						} else if (requireString.equals("CHALLENGE_RESPONSE")) {
							if (chal_response == null)
								cmdToBuild = "CHALLENGE_RESPONSE NONE";
							else
								cmdToBuild = "CHALLENGE_RESPONSE " + chal_response;
							chal_response = null;
						} else if (requireString.equals("QUIT")) {
							cmdToBuild = "QUIT";
						} else if (requireString.equals("PUBLIC_KEY")) {
							PublicKey pk = new PublicKey();
							int value = pk.value();
							pk.dispose();
							pk = null;
							String identity = parent.identity.getText();
							if (value == 1) {
								Pitbull.initializeUserRSAKey(identity, false);
								cmdToBuild = "PUBLIC_KEY RSA " +
									parent.command.arg[7].getText() + " " +
									parent.command.arg[8].getText();
								parent.state.deletePublicKey();
							} else if (value == 2) {
								Pitbull.initializeUserZKPKey(identity, false);
								cmdToBuild = "PUBLIC_KEY ZKP " +
									parent.command.arg[7].getText() + " " +
									parent.command.arg[8].getText();
								if (handler.trade == null) handler.trade = new Trade();
								handler.trade.addKey("ZKP",
															parent.command.arg[7].getText(),
															parent.command.arg[8].getText(),
															parent.command.privKey);
								parent.state.deletePublicKey();
							} else {
								cmdToBuild = "PUBLIC_KEY NONE";
								if (handler.trade == null) handler.trade = new Trade();
								handler.trade.addKey("NONE");
							}
						} else if (requireString.equals("TRADE_RESPONSE OR SUBSET_A")) {
							int rounds = Integer.parseInt(handler.trade.getRounds());
							cmdToBuild = "SUBSET_A";
							for (int i=0 ; i < rounds ; i += 2) {
								cmdToBuild += " " + i;
								handler.trade.A[i] = true;
							}
						} else if (requireString.equalsIgnoreCase("IDENT")) {
							parent.state.login("IDENT");
						} else if (requireString.equalsIgnoreCase("PASSWORD")) {
							parent.state.login("PASSWORD");
						} else if (requireString.equalsIgnoreCase("HOST_PORT")) {
							parent.state.login("HOST_PORT");
						} else if (requireString.equalsIgnoreCase("ALIVE")) {
							parent.state.login("ALIVE");
						}
					} else if (directiveString.equalsIgnoreCase("SUBSET_K")) {
						int rnds = Integer.parseInt(handler.trade.getRounds());
						String name = monitorTokens.nextToken();
						handler.trade.S = new BigInteger[rnds];  // SUBSET_K happens before
						try {                                    // SUBSET_J so S gets an
							for (int i=0 ; i < rnds ; i++) {      // array here and not below
								if (handler.trade.A[i]) {             
									handler.trade.S[i] = new BigInteger(monitorTokens.nextToken());
								}
							}
						} catch (Exception e) { }
					} else if (directiveString.equalsIgnoreCase("SUBSET_J")) {
						int rnds = Integer.parseInt(handler.trade.getRounds());
						String name = monitorTokens.nextToken();
						try {
							for (int i=0 ; i < rnds ; i++) {
								if (!handler.trade.A[i]) {
									handler.trade.S[i] = new BigInteger(monitorTokens.nextToken());
								}
							}
						} catch (Exception e) {	}
					} else if (directiveString.equalsIgnoreCase("CHALLENGE_RESPONSE")) {
						// Expected to be seen in the message group asking a trade subject
						// for a TRADE_RESPONSE.  Verification of the initiator occurs here.
						BigInteger dec = null;
						RSA rsa = null;
						
						String name = monitorTokens.nextToken();
						try {
							chal_response = new BigInteger(monitorTokens.nextToken());
							if (handler.trade.getKeyType("RSA")) {
								rsa = handler.trade.getRSA();
								if (rsa != null) dec = rsa.decryptNum(chal_response);
								if (dec.equals(handler.challenge)) handler.tradeColor=Color.green;
								else handler.tradeColor = Color.red;
							} 
						} catch (Exception e) {
							handler.tradeColor = Color.yellow;
						}
               } else if (directiveString.equalsIgnoreCase("AUTHORIZE_SET")) {
						int rounds = Integer.parseInt(handler.trade.getRounds());
						String framName = monitorTokens.nextToken();
						handler.trade.RR = new BigInteger[rounds];
						for (int i=0 ; i < rounds ; i++) 
							handler.trade.RR[i] = new BigInteger(monitorTokens.nextToken());
					} else if (directiveString.equalsIgnoreCase("TRADE_REQUEST")) {
						if (handler.trade == null) handler.trade = new Trade();
						handler.trade.addName(monitorTokens.nextToken());
						handler.trade.addTrade(monitorTokens.getRemaining());
               } else if (directiveString.equalsIgnoreCase("PUBLIC_KEY")) {
                  // this should only show up with a trade-request message group
                  // the trader was obtained from the TRADE_REQUEST directive
                  // the public key is obtained from this PUBLIC_KEY directive
						String typ = monitorTokens.nextToken();
						if (handler.trade == null) handler.trade = new Trade();
						if (typ.equals("ZKP") || typ.equals("RSA")) {
							String exp = monitorTokens.nextToken();
							String mod = monitorTokens.nextToken();
							handler.trade.addKey(typ, exp, mod);
							// the trader's certificate could also be obtained like this
							// String cert = Pitbull.readRSACert(trader.getName());
							String cert = monitorTokens.nextToken();
							handler.trade.addCert(cert);
							if (cert != null) {  // there is a trader's cert to compare
								// Record color green if certificate is good, red if no good
								if (Pitbull.checkRSACert(cert, exp, mod))
									handler.genuineColor = Color.green;
								else
									handler.genuineColor = Color.red;
							}
						} else {
                     // Record color of yellow if there is no certificate
							handler.genuineColor = Color.yellow;
							handler.trade.addKey(typ);
                  }
               } else if (directiveString.equalsIgnoreCase("COMMAND_ERROR:")) {
                  foundCommandError = true;
                  errorString = monitorTokens.getRemaining();
						Info err = new Info("   Command Error:", errorString);
               } else if (directiveString.equalsIgnoreCase("COMMENT:")) {
                  foundComment = true;
                  commentString = monitorTokens.getRemaining();
                  StringTokenizer commentTokens =
                     new StringTokenizer(commentString);
                  String commentFirstWord = "";
                  if (commentTokens.hasMoreTokens()) {
                     commentFirstWord = commentTokens.nextToken();
                  }
                  if (commentFirstWord.equalsIgnoreCase("Timeout")) {
                     messageComplete = true;
                  }
               } else if (directiveString.equalsIgnoreCase("RESULT:")) {
                  foundResult = true;
                  resultString = monitorTokens.getRemaining();
                  if (resultString == null || resultString.equals("")) {
                     resultString = "none";
                  } else if (resultString.equalsIgnoreCase("QUIT")) {
							messageComplete = true;
							setNormalTransmission();
							parent.client.state = false;
							parent.client.ind1.setBackground(Color.red);
							parent.client.ind2.setBackground(Color.red);
							parent.state.initiate();
							parent.ac.stop();
						} else if (resultString.equalsIgnoreCase("SIGN_OFF")) {
                     messageComplete = true;
							setNormalTransmission();
							parent.client.state = false;
							parent.server.state = false;
							parent.client.ind1.setBackground(Color.red);
							parent.client.ind2.setBackground(Color.red);							
							parent.server.ind1.setBackground(Color.red);
							parent.server.ind2.setBackground(Color.red);
							parent.state.initiate();
							parent.ac.stop();
							parent.srv.stop();
                  } else if (resultString.equalsIgnoreCase("HOST_PORT Command succeeded")) {
							parent.state.login("LOGGED");
							(new PlayClip("arrive.wav")).start();
						} else if (resultString.equalsIgnoreCase("ALIVE identity has been verified")) {
							parent.state.login("LOGGED");
						} else if (resultString.startsWith("IDENT")) {
                     ShouldRunIDENTKludge = true;
                     return;
                  } else if (resultString.startsWith("PLAYER_HOST_PORT")) {
							Info err = new Info("  Information", resultString);
						} else if (resultString.startsWith("RANDOM_PLAYER_HOST_PORT")) {
							Info err = new Info("  Information", resultString);
						}
               } else if (directiveString.equalsIgnoreCase("PLAYER_PASSWORD_CHECKSUM:")) {
                  checksumString = monitorTokens.getRemaining();
                  if (isMonitorAuthentic(checksumString)) foundChecksum = true;
                  else {
							foundChecksum = false;
							abort = true;
							parent.status.setText(KeepTwo.rows("MessageParser [getMonitor"+
																		  "Message]: Can't verify Monitor",
																		  parent.status.getText()));
						}
               } else if (directiveString.equalsIgnoreCase("CHALLENGE")) {
						// Received number is encrypted with player private key, if one exists,
						// and it is sent back to the verifier
						// This is expected to be seen in a message group that requires
						// CHALLENGE_RESPONSE.  The challenge is saved here then used in
						// the CHALLENGE_RESPONSE handleRequire function of the Server.
						try {
							BigInteger num = new BigInteger(monitorTokens.nextToken());
							RSA rsa = Pitbull.readUserRSAKey();
							if (rsa != null) chal_response = rsa.encryptNum(num);
							else chal_response = null;
						} catch (Exception e) {
							handler.challenge = null;
							parent.status.setText(KeepTwo.rows("MessageParser [getMonitor"+
																		  "Message]: Challenge invalid",
																		  parent.status.getText()));
						}

					} else if (directiveString.equalsIgnoreCase("ROUNDS")) {
						int rnds = 4;
						// Default rounds to 4 
						try {
							String r = monitorTokens.nextToken();
							rnds = Integer.parseInt(r);
							if (rnds < 4 || rnds > 20) r = "4";
							if (handler.trade == null) handler.trade = new Trade();
							handler.trade.addRounds(r);
						} catch (Exception e) {
							handler.trade.addRounds("4");
						}
						parent.status.setText(KeepTwo.rows("ZKP Authentication, Rounds = "+rnds,
																	  parent.status.getText()));
					} else if (directiveString.equalsIgnoreCase("SUBSET_A")) {
						for (int k=0 ; k < Integer.parseInt(handler.trade.rounds) ; k++)
							handler.trade.A[k] = false;
						int i=0;
						while (monitorTokens.hasMoreTokens()) {
							int n = Integer.parseInt(monitorTokens.nextToken());
							handler.trade.A[n] = true;
						}
					} else if (directiveString.equalsIgnoreCase("WAR_DECLARATION")) {
						handler.tmp_aggressor = monitorTokens.nextToken();
						parent.status.setText(KeepTwo.rows(handler.tmp_aggressor+" declared"+
																	  " war", parent.status.getText()));
					} else if (directiveString.equalsIgnoreCase("WAR_TRUCE_OFFERED")) {
						handler.tmp_offer = monitorTokens.getRemaining();
						parent.status.setText(KeepTwo.rows(handler.tmp_offer+" offered",
																	  parent.status.getText()));
					} else { // something's wrong!
                  messageComplete = true;
                  parent.status.setText(KeepTwo.rows("MessageParser [getMonitor"+
																	  "Message]: Unrecognized "+
																	  "directive! "+directiveString,
																	  parent.status.getText()));
               }
            } // end outer if
				else {
               messageComplete = true;
            }
         } catch(InterruptedIOException intIOExc) {
				parent.status.setText(KeepTwo.rows("MessageParser Interrupt IO Exception: "+
															  intIOExc.toString(),parent.status.getText()));
            continue; // continue to next iteration
         } catch (IOException ioExc) {
				parent.status.setText(KeepTwo.rows("MessageParser IO Exception: "+
															  ioExc.toString(), parent.status.getText()));
            break;
         } catch (Exception e) {
				parent.status.setText(KeepTwo.rows("MessageParser: not connected",
															  parent.status.getText()));
				break;
			}
      } // end while
      if (!foundRequire) requireString = "none";
      if (!foundCommandError) errorString = "none";
      if (!foundComment) commentString = "none";
      if (!foundResult) resultString = "none";
      if (!foundChecksum) checksumString = "none";
   } // end method getMonitorMessage

	/* This is for maintenance
	void showByteArray (byte[] a) {
		for (int i=0 ; i < a.length ; i++) System.out.print(a[i]);
		System.out.println();
	}
	*/

   // Check whether the Monitor is Authentic by comparing the SHA digest of the
   // Client password with the checksum returned by the Monitor.  Returns true
   // iff the two are identical.  Used by isMonitorAuthentic.
   public boolean verify(String passwd, String chksum)
      throws NoSuchAlgorithmException {
      boolean debug = false;
   
      try {
         MessageDigest md = MessageDigest.getInstance("SHA"); 
         passwd = passwd.toUpperCase();
         byte[] byteArr = passwd.getBytes();
         md.update(byteArr);      
         BigInteger big = new BigInteger(1,md.digest());

         if (debug) {
            System.out.println("Message Parser [verify]: original chksum: " +
                               chksum.trim());
            System.out.println("Message Parser [verify]: password: " + passwd);
            System.out.println("Message Parser [verify]: chksum from password: "
                               + big.toString(16));
            System.out.println("Message Parser [verify]: chksum to BI: "+
                               new BigInteger(chksum, 16));
         }
         
         if ((big.toString(16).equals(chksum.trim()))) return true;
         else {
            parent.status.setText(KeepTwo.rows("Monitor not authenticated!",
															  parent.status.getText()));
            return false;
         }
      } catch (NoSuchAlgorithmException e) {  return false;   }
   } // end of verify

   // Compares the SHA digest of the Client password with ppchecksum that
   // is received from the Monitor.  Returns true iff ppchecksum and the
   // digest are identical.  If they are not identical then it is likely
   // that someone is spoofing the Monitor.  The function verify is doing
   // all the work.
   public boolean isMonitorAuthentic(String ppchecksum) {
      boolean debug = false, success = false;
      
      if (debug)
         System.out.println("Message Parser [isMonitorAuthentic]: checksum: "+
                            ppchecksum);
      try {
         if (ppchecksum != null) {
            String password = Pitbull.getPassword();
            if (!verify(password, ppchecksum)) {
               if (debug)
                  System.out.println("Message Parser [isMonitorAuthentic]: "+
                                     "MONITOR NOT AUTHORIZED!!! QUITING");
               success = false;
            } else {
               if (debug)
                  System.out.println("Message Parser [isMonitorAuthentic]: YES");
               success= true;        
            }   
         } 
      } catch (NoSuchAlgorithmException e) {
         parent.status.setText(KeepTwo.rows("Message Parser Client [isMonitor"+
														  "Authentic]: error verifying:\n\t" +
														  e, parent.status.getText()));
      }
      return success;        
   } // end of isMonitorAuthentic
   
   // A complete Client command, formatted as ascii text, is transmitted to
   // the Monitor from here.
   //   Example: HOST_PORT 10.8.0.53 20000
   // This includes commands formed by the ActiveClient and the PassiveServer
   // If UsingEncryption is true the command is passed to a karn encryptor
   // first and the encrypted output is piped to the Monitor connection
   // Otherwise the command is piped into the Monitor connection in the clear
   public boolean execute(String command) {
      boolean debug = false;

      if (debug)
         System.out.println("Message Parser [execute]: command: "+command);

      StringTokenizer commandTokens = new StringTokenizer(command);
      if (commandTokens.hasMoreTokens()) {
         lastCommand = commandTokens.nextToken();
      } else {
         lastCommand = "none";
         parent.status.setText(KeepTwo.rows("MessageParser [execute]: blank "+
														  "command string", parent.status.getText()));
         return false;
      }

      if (!UsingEncryption) {
         try { out.println(command); } catch (Exception e) {
				parent.status.setText(KeepTwo.rows("MessageParser: not connected",
															  parent.status.getText()));
			}
			if (parent.accessible) {
				if (subclassType.equals("ac"))
					System.out.println("Command to the monitor from active client : "+command);
				else
					System.out.println("Command to the monitor from passive server : "+command);
			}
		} else {
         try { karnOut.println(command); } catch (Exception e) {
				parent.status.setText(KeepTwo.rows("MessageParser: not connected",
															  parent.status.getText()));
			}
			if (parent.accessible) {
				if (subclassType.equals("ac"))
					System.out.println("Command to the monitor encrypted from active client : "+
											 command);
				else
					System.out.println("Command to the monitor encrypted from passive server : "+
											 command);
			}
		}
      log.append("[" + subclassType + "] " + command + "\n\n");
      return true;

   } // end method execute  
} // end class MessageParser
