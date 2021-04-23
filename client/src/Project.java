import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.io.*;
import java.net.*;
import java.math.*;
import java.security.*;
import java.time.*;

/**
 * IWar is a game-player in Dr. Franco's I-Wars Competition.
 *
 *@author Greg Larson
 *@version 022704
 *
 * + JVF made extensive modifications to this code, May-July, 2019, including
 * + layout, buildCmd, and code organization
 */
public class Project extends JFrame implements ActionListener, MouseListener {

   // Player-related networking variables, for common use by IWar,
   // ActiveClient, and Server classes
   public static String PLAYER_HOST_NAME = "";
	long start_time = 0, end_time = 0, cur_time = 0;

   // used for text outputs from the ActiveClient and Server classes
   public Log client_log, server_log;
   
   // data structures
   public Bank bank;
   ActiveClient ac;
   Server srv;
   CommandPanel command;

   static boolean helpstate = false;  // true=help tips are shown, false=not shown
	static boolean accessible = false; // true=show messages with monitor on console
	static boolean clientFontSize = true;
	static boolean serverFontSize = true;
   // GUI component - 3rd row

   // GUI components - top and bottom rows
   JTextField identity = new JTextField();
   JTextField passwd = new JTextField();
   JTextField timeleft = new JTextField(8);
   JTextArea status = new JTextArea(2,40);
   JTextField monlocation = new JTextField(12);
   JTextField monport = new JTextField(8);
   JComboBox <String> srvlocation = new JComboBox <String> ();
   JTextField srvport = new JTextField(8);
   Agent server, client;
	WarParms wp = null;
	TradeParms tp = null;
	StatusParms sp = null;
	SynthesizeParms yp = null;
	CrackParms kp = null;
	DefendParms dp = null;
	WarStatusParms ap = null;
	TruceParms up = null;
	OfferedParms op = null;

   String name, pwd, sport, mport, sloc, mloc;
	Info info = null;
   
   // GUI components - 2nd row
   private JButton clientLogOn;
   private JButton clientLogOff;
   private JButton serverLogOn;
   private JButton serverLogOff;
   private JButton clientLogFont;
   private JButton serverLogFont;
   private JButton help;
	private JButton accessibility;

	DefaultListCellRenderer listRenderer;
   
   boolean encryption_on = false;
   private Color back = new Color(238,238,238);
   JLabel label, ll, la, lb, lc, ld, le, time_label;
   private String rounds = "0";  // for ZKP
	PubRSA monitorPK = Pitbull.createPubMonitorRSA();
	State state = null;

	Mouse mousehelp = null;

	BigInteger challenge = null;
	static int challenge_length = 128;

	ContestTimer ct = null;

   public Project() {
      super("I-WARS Client");
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter() {
				public void windowClosing(WindowEvent e) {
					(new PlayClip("leave-12.wav")).start();
					try { Thread.sleep(600); } catch (Exception exc) { }
					System.exit(0);
				}
			});
		
		Pitbull.setProject(this);
      
      PLAYER_HOST_NAME = "HostNotSet"; // retrieve from GUI later

		listRenderer = new DefaultListCellRenderer();
      listRenderer.setHorizontalAlignment(DefaultListCellRenderer.CENTER);
		srvlocation.setRenderer(listRenderer);

      // initialize the Bank for treasure-keeping
      bank = new Bank();
      bank.initialize();
		mousehelp = new Mouse();

      // initialize logs
      client_log = new Log(this, "Client Log");
      server_log = new Log(this, "Server Log");

      client_log.setSize(750,500);
      client_log.setVisible(true);
      client_log.fontDn();
      server_log.setSize(750,500);
      server_log.setVisible(true);
      server_log.fontDn();

      initGUI();

		try {
			cur_time = Instant.now().getEpochSecond();
			start_time = Long.parseLong(MessageParser.getSetup("# Start Time"));
			end_time = Long.parseLong(MessageParser.getSetup("# End Time"));
		} catch (Exception e) {
			start_time = 0;
			end_time = 0;
		}

		if (start_time > end_time) {
			status.setText("No times: improper start and end epochs");
		} else if (start_time > 0 && end_time > 0) {
			if (cur_time < start_time)
				time_label.setText("Time to start:");
			else if (cur_time < end_time)
				time_label.setText("Time to end:");
			else if (cur_time >= end_time)
				time_label.setText("Contest:");
					
			ct = new ContestTimer(this);
			ct.setTimes(start_time, end_time);
			ct.start();
		} else {
			status.setText("Something is wrong - Parms file with contest parameters missing?");
		}
		
		setSize(1080,640);
      setVisible(true);

		if (MessageParser.keysWarn) {
			info = new Info("  Parms file:",
								 "A Parms file with contest parameters could "+
								 "not be found - using defaults");
			MessageParser.keysWarn = false;
		}
   }

   // Set up the user interface
   public void initGUI() {
      setLayout(new GridBagLayout());
      GridBagConstraints gc = new GridBagConstraints();

      //---- begin left component of 1st line           +-----------+
      //---- 1st column is a BorderLayout where         |          V|
      //---- North is a 2 column, 1 row layout          +-----------+
      //---- and each column is a BorderLayout             username
      //---- where North is a widget and South
      //---- is a name for the meaning of widget.       +-----------+
      //---- These are illustrated to the right.        |           |
      //----                                            +-----------+
      //----                                              password
      //---- begin 1st line
      JPanel y = new JPanel(new GridLayout(1,2));
      
      //---- begin 1st Component on 1st line - borderlayout g
      JPanel g = new JPanel(new BorderLayout(20,0));

      //---- begin North of borderlayout g - username and passwd
      //---- use gridlayout p of 1 row and 2 columns
      JPanel p = new JPanel(new GridLayout(1,2,20,0));

      //---- begin username widget and name of North of g
      JPanel qr = new JPanel(new BorderLayout());

      qr.add("Center", identity);
      qr.add("West", new JLabel("       "));
      qr.add("South", new JLabel("username", JLabel.CENTER));
      p.add(qr);
      //---- username widget and name are added to 1st column of p

      //---- begin password widget and name of North of g
      qr = new JPanel(new BorderLayout());
      
      qr.add("Center",passwd);
      passwd.addActionListener(this);      
      qr.add("East", new JLabel("      "));
      qr.add("South", new JLabel("passwd", JLabel.CENTER));
      p.add(qr);
      //---- password widget and name are added to 2nd column of p
      g.add("North",p);
      //---- layout p, with username and passwd widgets, is added to North of g

      //---- begin assembly of Center of g - use two 'Agent' panels in
      //---- one row of two columns established by new gridlayout p
      p = new JPanel(new GridLayout(1,2,10,0));   //  ++--------+ ++---------+
      p.add(server = new Agent("Server", this));  //  ||--------| ||---------|
      p.add(client = new Agent("Client", this));  //  ++--------+ ++---------+
      g.add("Center",p);                          //     server       client
      //---- the structure shown to the right is added to the Center of g

      //---- g gets a little padding
      g.add("West", new JLabel("      "));
      g.add("East", new JLabel("      "));

      //---- the 1st component of the 1st line is added to the 1st column of y
      y.add(g);
      
      //---- begin 2nd Component of the 1st line - borderlayout g
      g = new JPanel(new BorderLayout(0,10));

      //---- begin North of borderlayout g 
      //---- monitor and server host location      +--------------+ +--------------+
      //---- gridlayout p: 1 row, 2 cols           +--------------+ +--------------+
      p = new JPanel(new GridLayout(1,2,10,0)); // monitor location  server host loc
      
      //---- begin monitor location widget and name of North of g - col 1 of p
      qr = new JPanel(new BorderLayout());
      
      qr.add("Center", monlocation);
      qr.add("East", new JLabel(" "));
      qr.add("West", new JLabel("      "));
      qr.add("South", new JLabel("monitor location", JLabel.CENTER));
      p.add(qr);
      //---- monitor location widget and name are added to 1 column of p

      //---- begin server host location widget and name of North of g
      qr = new JPanel(new BorderLayout());
      
      qr.add("Center",srvlocation);
      qr.add("East",new JLabel("      "));
      qr.add("West",new JLabel(" "));
      qr.add("South",new JLabel("server host location", JLabel.CENTER));
      p.add(qr);
      //---- server host location widget and name are added to 2nd column of p
      g.add("North",p);
      //---- layout p, with monitor and server location widgets, added to g North

      //---- begin assembly of South of g
      p = new JPanel(new GridLayout(1,2,10,0));   // +------------+ +------------+
                                                  // +------------+ +------------+
      qr = new JPanel(new BorderLayout());        //  monitor port  server host port
      
      qr.add("Center", monport);
      qr.add("South", new JLabel("monitor port", JLabel.CENTER));
      qr.add("East", new JLabel(" "));
      qr.add("West", new JLabel("      "));
      p.add(qr);
      //---- monitor location widget and name are added to 1st column of p

      qr = new JPanel(new BorderLayout());
      
      qr.add("Center", srvport);
      qr.add("South", new JLabel("server host port", JLabel.CENTER));
      qr.add("East", new JLabel("      "));
      qr.add("West", new JLabel(" "));
      p.add(qr);
      //---- server host location widget and name are added to 2nd column of p
      g.add("Center",p);
      //---- layout p, with monitor and server location widgets, added to g North
      g.add("South", ll = new JLabel("  "));
      //---- extra space for better looks
      y.add(g);
      //---- add 2nd component to first line

      gc.fill = GridBagConstraints.HORIZONTAL;
      gc.gridx = 0;
      gc.gridy = 0;
      gc.gridwidth = 7;
      add(y, gc);
      //---- end 1st line

      //---- begin 2nd line
      JPanel x = new JPanel(new GridLayout(1,2));

      JPanel xr = new JPanel(new BorderLayout(10,10));
      
      JPanel xp = new JPanel(new GridLayout(2,2,5,5));
      
      xp.add(clientLogOn = new JButton("Show Client Log"));
      xp.add(clientLogFont = new JButton("Large Client Log Font"));
      xp.add(clientLogOff = new JButton("Hide Client Log"));
		xp.add(accessibility = new JButton("Not Showing Messages"));
      xr.add("Center", xp);
		
      xr.add("North", lb = new JLabel("  "));
      lb.setFont(new Font("Helvetica", Font.PLAIN, 4));
      xr.add("South", lc = new JLabel("  "));
      lc.setFont(new Font("Helvetica", Font.PLAIN, 4));
      xr.add("East", new JLabel("     "));
      xr.add("West", new JLabel("        "));
      x.add(xr);
      
      xr = new JPanel(new BorderLayout(10,10));
      
      xp = new JPanel(new GridLayout(2,2,5,5));
      
      xp.add(serverLogOn = new JButton("Show Server Log"));
      xp.add(serverLogFont = new JButton("Large Server Log Font"));
      xp.add(serverLogOff = new JButton("Hide Server Log"));
      xp.add(help = new JButton("Not Showing Help"));
      xr.add("Center", xp);
		
      xr.add("North", ld = new JLabel("  "));
      xr.add("South", le = new JLabel("  "));
      ld.setFont(new Font("Helvetica", Font.PLAIN, 4));
      le.setFont(new Font("Helvetica", Font.PLAIN, 4));      
      xr.add("East", new JLabel("        "));
      xr.add("West", new JLabel("     "));
      x.add(xr);
      
      gc.fill = GridBagConstraints.HORIZONTAL;
      gc.gridx = 0;
      gc.gridy = 2;
      gc.gridwidth = 7;
      add(x, gc);
      //---- end of 2nd line

      //----------- begin command panel on 3rd line
      command = new CommandPanel("Commands", this);
      
      gc.fill = GridBagConstraints.HORIZONTAL;
      gc.gridx = 0;
      gc.gridy = 4; 
      gc.gridwidth=7;
      add(command, gc);
      //----------- end command panel on 3rd line

      //------------------ begin time left and messages line on 4th line
      JPanel h = new JPanel(new BorderLayout(10,10));

      g = new JPanel(new BorderLayout(10,10));
      g.add("West", time_label = new JLabel("time left:",JLabel.RIGHT));
      g.add("Center", timeleft);
      h.add("West", g);

      g = new JPanel(new BorderLayout(10,10));
      g.add("West", new JLabel("msgs:",JLabel.RIGHT));
      g.add("Center", status);
      h.add("Center", g);
      h.add("North", la = new JLabel(" "));
      
      gc.fill = GridBagConstraints.HORIZONTAL;
      gc.gridx = 0;
      gc.gridy = 6;
      gc.gridwidth=7;
      add(h, gc);
      //-------------------- end time left and messages on 4th line

      name = MessageParser.getSetup("# Username");
      if (name.equals("")) {
			name = MessageParser.getSetup("# Username editable");
			if (name.equals("")) 
				identity.setText("please-edit");
			else
				identity.setText(name);
			identity.setEditable(true);
      } else {
         identity.setText(name);
         identity.setEditable(false);
      }
      identity.setFont(new Font("Helvetica",Font.PLAIN, 14));
      identity.setHorizontalAlignment(JTextField.CENTER);

      pwd = MessageParser.getSetup("# Password");
      if (pwd.equals("")) {
			pwd = MessageParser.getSetup("# Password editable");
			if (pwd.equals(""))
				passwd.setText("please-edit");
			else
				passwd.setText(pwd); 
         passwd.setEditable(true);
      } else {
         passwd.setText(pwd);
         passwd.setEditable(false);
      }
      passwd.setFont(new Font("Helvetica",Font.PLAIN, 14));
      passwd.setHorizontalAlignment(JTextField.CENTER);

      sport = MessageParser.getSetup("# Server Port");
      if (sport.equals("")) {
			sport = MessageParser.getSetup("# Server Port editable");
			if (sport.equals(""))
				srvport.setText("please-edit");
			else
				srvport.setText(sport);
         srvport.setEditable(true);
         command.arg[4].setEditable(true);
      } else {
         srvport.setText(sport);
         srvport.setEditable(false);
         command.arg[4].setEditable(false);         
      }
      srvport.setHorizontalAlignment(JTextField.CENTER);
      srvport.setFont(new Font("Helvetica",Font.PLAIN,14));

      mport = MessageParser.getSetup("# Monitor Port");
      if (mport.equals("")) {
			mport = MessageParser.getSetup("# Monitor Port editable");
			if (mport.equals(""))
				monport.setText("please-edit");
			else
				monport.setText(mport);
         monport.setEditable(true);
      } else {
         monport.setText(mport);
         monport.setEditable(false);
      }
      monport.setHorizontalAlignment(JTextField.CENTER);
      monport.setFont(new Font("Helvetica",Font.PLAIN,14));
      
      mloc = MessageParser.getSetup("# Monitor Location");
      if (mloc.equals("")) {
			mloc = MessageParser.getSetup("# Monitor Location editable");
			if (mloc.equals(""))
				monlocation.setText("please-edit");
			else
				monlocation.setText(mloc); 
         monlocation.setEditable(true);
      } else {
         monlocation.setText(mloc);
         monlocation.setEditable(false);
      }
      monlocation.setHorizontalAlignment(JTextField.CENTER);
      monlocation.setFont(new Font("Helvetica",Font.PLAIN,14));

      sloc = MessageParser.getSetup("# Server Location");
      if (sloc.equals("")) {
			sloc = MessageParser.getSetup("# Server Location editable");
			if (sloc.equals("")) {
				srvlocation.addItem("please-edit");
			} else if (sloc.toUpperCase().equals("EDITABLE")) {
				/*
				try (final DatagramSocket socket = new DatagramSocket()) {
					socket.connect(InetAddress.getByName("8.8.8.8"), 10002);
					String ip = socket.getLocalAddress().getHostAddress();
					srvlocation.setText(ip);
				} catch (Exception e) {
					srvlocation.setText("enter server location");
				}*/
				try {
					Enumeration<NetworkInterface> n = NetworkInterface.getNetworkInterfaces();
					while (n.hasMoreElements()) {
						NetworkInterface ni = (NetworkInterface) n.nextElement();
						Enumeration<InetAddress> nias = ni.getInetAddresses();
						while(nias.hasMoreElements()) {
							InetAddress ia = (InetAddress)nias.nextElement();
							if (!ia.isLinkLocalAddress() 
								 && !ia.isLoopbackAddress()
								 && ia instanceof Inet4Address) {
								String str = (ia.toString().charAt(0) == '/') ? 
									ia.toString().substring(1) : ia.toString();
								srvlocation.addItem(str);
							}
						}
					}
				} catch (Exception e) {
					srvlocation.addItem("enter server location");
				}
			} else
				srvlocation.addItem(sloc); 
         srvlocation.setEditable(true);
         command.arg[3].setEditable(true);
      } else {
         srvlocation.addItem(sloc);
         srvlocation.setEditable(false); 
         command.arg[3].setEditable(false);         
      }
		JTextField editor = (JTextField)srvlocation.getEditor().getEditorComponent();
		editor.setHorizontalAlignment(JTextField.CENTER);
      srvlocation.setFont(new Font("Helvetica",Font.PLAIN,14));

		try {
			challenge_length =
				Integer.parseInt(MessageParser.getSetup("# Challenge Length"));
		} catch (Exception e) {
			status.setText(KeepTwo.rows("Exception: challenge length "+
												 challenge_length, status.getText()));
		}
			
      timeleft.setEditable(false);
      timeleft.setHorizontalAlignment(JTextField.CENTER);

      command.arg[4].setText(srvport.getText());

      clientLogOn.addActionListener(this);
      clientLogOff.addActionListener(this);
      clientLogFont.addActionListener(this);
      serverLogOn.addActionListener(this);
      serverLogOff.addActionListener(this);
      serverLogFont.addActionListener(this);
		accessibility.addActionListener(this);
      help.addActionListener(this);
		srvlocation.addActionListener(this);
		srvport.addActionListener(this);

      status.setEditable(false);
      status.setFont(new Font("Helvetica", Font.BOLD, 11));

      ll.setFont(new Font("TimesRoman",Font.PLAIN, 22));
      la.setFont(new Font("Helvetica",Font.PLAIN,8));

      passwd.addMouseListener(this);
      identity.addMouseListener(this);
      monport.addMouseListener(this);
      srvport.addMouseListener(this);
      monlocation.addMouseListener(this);
      srvlocation.addMouseListener(this);

		state = new State(this);
		state.initiate();
   }

   public void mouseEntered (MouseEvent evt) {
      if (!helpstate) return;
      if (evt.getSource() == passwd) mousehelp.password();
      else if (evt.getSource() == identity) mousehelp.identity();
      else if (evt.getSource() == monport) mousehelp.monitorPort();
      else if (evt.getSource() == srvport) mousehelp.serverPort();
      else if (evt.getSource() == monlocation) mousehelp.monitorLocation();
      else if (evt.getSource() == srvlocation) mousehelp.serverLocation();
   }

	public void mouseClicked (MouseEvent evt) { mousehelp.exit(); }
	public void mouseExited (MouseEvent evt) { }
   public void mouseReleased (MouseEvent evt) { }
   public void mousePressed (MouseEvent evt) { }

   public void actionPerformed (ActionEvent evt) {
      if (evt.getSource() == passwd) {
         Pitbull.setPassword(passwd.getText());
      } else if (evt.getSource() == clientLogOn) {
         if (client_log != null) {
            client_log.setVisible(true);
            client_log.flush();
         }
      } else if (evt.getSource() == clientLogOff) {
         if (client_log != null) client_log.setVisible(false);
      } else if (evt.getSource() == serverLogOn) {
         if (server_log != null) {
            server_log.setVisible(true);
            server_log.flush();
         }
      } else if (evt.getSource() == serverLogOff) {
         if (server_log != null) server_log.setVisible(false);
      } else if (evt.getSource() == clientLogFont) {
			if (clientFontSize == true) {
				clientFontSize = false;
				client_log.fontUp();
				clientLogFont.setText("Normal Client Log Font");
			} else {
				clientFontSize = true;
				client_log.fontDn();
				clientLogFont.setText("Large Client Log Font");
			}
      } else if (evt.getSource() == serverLogFont) {
			if (serverFontSize == true) {
				serverFontSize = false;
				server_log.fontUp();
				serverLogFont.setText("Normal Server Log Font");
			} else {
				serverFontSize = true;
				server_log.fontDn();
				serverLogFont.setText("Large Server Log Font");				
			}
      } else if (evt.getSource() == help) {
         if (helpstate == false) {
            helpstate = true;
            help.setText("Showing Help");
         } else {
				mousehelp.exit();
            helpstate = false;
            help.setText("Not Showing Help");
         }
      } else if (evt.getSource() == accessibility) {
			if (accessible == false) {
				accessible = true;
				accessibility.setText("Showing Messages");
			} else {
				accessible = false;
				accessibility.setText("Not Showing Messages");
			}
		} else if (evt.getSource() == srvlocation) {
			command.arg[3].setText((String)srvlocation.getSelectedItem());
		} else if (evt.getSource() == srvport) {
			command.arg[4].setText(srvport.getText());
		}
   }

   public JComboBox <String> copyBox (JComboBox <String> jcb, boolean go) {
      JComboBox <String> out = new JComboBox <String> ();
      int no = jcb.getItemCount();
      for (int i=0 ; i < no ; i++) {
         String name = (String)jcb.getItemAt(i);
         if (!name.toUpperCase().equals(identity.getText().toUpperCase()) || go)
            out.addItem(name);
      }
      return out;
   }

	public void updateBox(JComboBox <String> update, JComboBox <String> jcb, boolean go) {
		update.removeAllItems();
      int no = jcb.getItemCount();
      for (int i=0 ; i < no ; i++) {
         String name = (String)jcb.getItemAt(i);
         if (!name.toUpperCase().equals(identity.getText().toUpperCase()) || go)
            update.addItem(name);
      }
	}
	
   public void setResource(int resource, int val) {
      bank.setValue(resource, val);
   }

   // Adds arguments to commands from cmdVec, then appends them to
   // cmdQueueVec
   public String buildCmd(String cmd) {
      boolean debug = false;
      String amts[] = { "1","2","3","4","5","6","7","8","9","10","11","12" };
      JComboBox <String> number = new JComboBox <String> (amts);
      String cmdToAdd = "";
      JComboBox <String> jcb;

		if (cmd.equals("IDENT")) {
         String ident = identity.getText();
         String pwdFile = "password." + ident + ".dat";
         String aliveFile = "cookie."+ ident + ".dat";
         String checksumFile = ident + "ChecksumFile.txt";
         Pitbull.setFiles(pwdFile, aliveFile, checksumFile);
			Pitbull.getPassword();

         if (encryption_on) {
            MessageParser.getDHPublicKey();
            String clientDHhalf = MessageParser.dhe.getExchangeKey();
            cmdToAdd = cmd + " " + ident + " "+ clientDHhalf;
         } else {
				cmdToAdd = cmd + " " + ident;
			}
      } else if (cmd.equals("PASSWORD")) {
         cmdToAdd = cmd + " " + Pitbull.getPassword();
      } else if (cmd.equals("RANDOM_PLAYER_HOST_PORT")) {
         cmdToAdd = cmd;
      } else if (cmd.equals("WAR_DECLARE")) {
			wp = new WarParms("War Declaration", 2, this);
			String value = wp.value();
			wp.dispose();
			wp = null;

			if (value != null && !value.equals("")) {
				cmdToAdd = cmd + " " + value;
			} else {
				status.setText(KeepTwo.rows("Abort WAR_DECLARE: command format error",
													 status.getText()));
			}
      } else if (cmd.equals("HOST_PORT")) {
         if (Integer.parseInt(srvport.getText()) < 10000)
            srvport.setText("10000");
         PLAYER_HOST_NAME = (String)srvlocation.getSelectedItem();
         command.arg[3].setText(PLAYER_HOST_NAME);
         command.arg[4].setText(srvport.getText());
         cmdToAdd = cmd+" "+PLAYER_HOST_NAME+" "+command.arg[4].getText();
		} else if (cmd.equals("WAR_STATUS")) {
			ap = new WarStatusParms("War Status", this);
			String value = ap.value();
			ap.dispose();
			ap = null;
			
         if (value != null && !value.equals("")) {
            cmdToAdd = "WAR_STATUS "+value;
         } else {
				status.setText(KeepTwo.rows("Abort WAR_STATUS: no target",
													 status.getText()));
			}
      } else if (cmd.equals("CHANGE_PASSWORD")) {
         String pass = command.arg[5].getText();
         if (pass.equals("")) pass = Pitbull.newPassword();
         else Pitbull.newPassword = pass;
         command.arg[5].setText("");
         cmdToAdd = cmd + " " + Pitbull.getPassword() + " " + pass;
      } else if (cmd.equals("QUIT")) {
         cmdToAdd = cmd;
		} else if (cmd.equals("WAR_TRUCE_OFFER")) {
			up = new TruceParms("Offer Resources For Ending War", this);
			String offer = up.value();
			up.dispose();
			up = null;
			
			if (offer != null && !offer.equals("")) {
				cmdToAdd = "WAR_TRUCE_OFFER "+offer;
			} else {
				status.setText(KeepTwo.rows("Abort WAR_TRUCE_OFFER: no valid offer",
													 status.getText()));
			}
      } else if (cmd.equals("ALIVE")) {
         cmdToAdd = cmd + " " + Pitbull.getAliveCookie();
      } else if (cmd.equals("SIGN_OFF")) {
         srv.stop();
         cmdToAdd = cmd;
      } else if (cmd.equals("SYNTHESIZE")) {
			yp = new SynthesizeParms("Synthesize an Asset", this);
			String value = yp.value();
			yp.dispose();
			yp = null;
			
			if (value != null && !value.equals("")) {
				cmdToAdd = cmd + " " +value;
			} else {
				status.setText(KeepTwo.rows("Abort Synthesize", status.getText()));
			}
      } else if (cmd.equals("MAKE_CERTIFICATE")) {
			// Do not send a make_certificate unless the adjacent public key
			// fields are populated.
			if (!command.arg[7].getText().equals("") &&
				 !command.arg[8].getText().equals("")) {
				cmdToAdd = cmd + " " + command.arg[7].getText() + " " +
					command.arg[8].getText();
			} else {
				status.setText(KeepTwo.rows("Abort MAKE_CERTIFICATE: no public key",
													 status.getText()));
			}
      } else if (cmd.equals("GET_CERTIFICATE")) {
         cmdToAdd = cmd + " " + command.cmd12_arg1.getSelectedItem();
      } else if (cmd.equals("GET_GAME_IDENTS")) {
         cmdToAdd = cmd;
      } else if (cmd.equals("PLAYER_STATUS_CRACK")) {
			kp = new CrackParms("Player Status Crack", this);
			String value = kp.value();
			kp.dispose();
			kp = null;

			if (value != null && !value.equals("")) {
				cmdToAdd = cmd + " " + value;
			} else {
				status.setText(KeepTwo.rows("Abort Crack Player Status", status.getText()));
			}
      } else if (cmd.equals("PLAYER_HOST_PORT")) {
			if (command.cmd12_arg1 == null ||
				 command.cmd12_arg1.getSelectedItem() == null ||
				 ((String)command.cmd12_arg1.getSelectedItem()).equals("")) {
				cmdToAdd = cmd + " " + identity.getText();
			} else {
				cmdToAdd = cmd + " " + command.cmd12_arg1.getSelectedItem();
			}
      } else if (cmd.equals("PLAYER_STATUS")) {
			if (sp == null) sp = new StatusParms("Player Status", this);
         cmdToAdd = cmd;
		} else if (cmd.equals("TRADE_REQUEST")) {
         command.arg[6].setBackground(new JButton().getBackground());
			tp = new TradeParms("Choose Resources Offered for Trade", 10, this);
			String offered = tp.value();
			tp.dispose();
			tp = null;
			
			if (offered != null && !offered.equals("")) {
				cmdToAdd = cmd + " " +offered;
			} else {
				status.setText(KeepTwo.rows("Abort TRADE_REQUEST: incorrect format",
													 status.getText()));
			}
      }
      return cmdToAdd;
   }

	String secsToMinsAndHours (long seconds) {
		long days = seconds / 86400;
		long hours = (seconds - (days * 8600)) / 3600;
		long mins = (seconds - (days * 8600) - (hours * 3600)) / 60;
		long secs = seconds % 60;
		return days+":"+hours+":"+mins+":"+secs;
	}
}

class ContestTimer extends Thread {
	long cur_time;
	long start_time;
	long end_time;
	boolean running = true;
	Project project = null;

	public ContestTimer (Project pr) { project = pr; }

	public void setTimes (long start, long end) {
		// temporary: start is cur_epoch + some seconds, also for end_time
		cur_time = Instant.now().getEpochSecond();
		start_time = start; 
		end_time = end;

		if (start_time > end_time) {
			project.status.setText("start time must be less than end_time");
			running = false;
		}
	}

	public void run () {

		while (running) {
			cur_time = Instant.now().getEpochSecond();
			if (cur_time > end_time) {
				project.time_label.setText("Contest:");
				project.timeleft.setText("Finished");
				running = false;
			} else if (cur_time > start_time) {
				project.time_label.setText("Time to end:");
				project.timeleft.setText(secsToMinsAndHours(end_time - cur_time));
			} else {
				project.time_label.setText("Time to start:");
				project.timeleft.setText(secsToMinsAndHours(start_time - cur_time));
			}
			try { sleep(1000); } catch (Exception e) { }
		}
	}
	
	String secsToMinsAndHours (long seconds) {
		long days = seconds / 86400;
		long hours = (seconds - (days * 86400)) / 3600;
		long mins = (seconds - (days * 86400) - (hours * 3600)) / 60;
		long secs = seconds % 60;
		return days+":"+hours+":"+mins+":"+secs;
	}
}

