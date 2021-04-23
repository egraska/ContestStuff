import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.io.*;
import java.net.*;
import java.math.*;
import java.security.*;

// Opens a box that lists Vehicles and Weapons under 'Assets', has a menu listing
// potential war victims under 'I declare war with player', a numeric pad to enter 
// quantities of Vehicles and Weapons to commit to the war, and a keyboard to
// enter the hostname and port of the victim's server.  The TextFields adjacent
// on the right side of assets display current holding if the player is logged
// in and the 'Update assets' button is clicked.  To enter an amount of asset
// to commit to the war click on the asset's button then use the number pad.
// Populate the victim menu by clicking the 'Update players' button.  Then select
// the victim from the menu.  Use the keyboard to enter the hostname and port
// number, separating the two with a space.  Click OK to send the command.
//
// Use like this in the Project class:
//   wp = new WarParms("box title goes here", 10, this);
//   String out = wp.value();
//   wp.dispose();
//   wp = null;
//
// The 1st line opens the box and control proceeds to the 2nd line which is
// stopped by the 'wait()' in the value function of TradeParms.  When OK or
// cancel are clicked a notify causes execution past the wait and a string is
// returned - the string was assembled before the notify.  Variable out gets
// that string for use later.  The 3rd line closes the box and the 4th line
// clears the reference to the box.
//
// The variable wp is declared in class Project and initialized to null.
// In class ActiveClient, handleResult, section PLAYER_STATUS, there needs to
// be a line
//    if (parent.wp != null) parent.wp.updateAssets(resultString);
// which activates the 'Update assets' button in WarParms
// In the GET_GAME_IDENTS section of the ActiveClient's handleResult there needs
// to be a line
//    if (parent.wp != null)
//       parent.updateBox(parent.wp.targets, parent.command.cmd12_arg1, false);
// which activates the 'Update players' button in WarParms.
public class OfferedParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];	
	String str[] = new String[11];
	Color bkgcolor = new Color(255,255,223);
	int which = -1; 
	JButton accept, decline, updateResources, help;
	String out="";
	Project project;
	JTextField target = new JTextField(12);;
	JLabel label;
	String response;

	public OfferedParms (String ttl, String res, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		JLabel label;
		project = pr;
		response = res;
		
		setTitle(ttl);
		setLayout(new BorderLayout());
		setBackground(bkgcolor);		
		
		JPanel x = new JPanel(new GridLayout(1,2));
		x.setBackground(bkgcolor);

		JPanel p = new JPanel(new GridBagLayout());
		p.setBackground(bkgcolor);
		GridBagConstraints gc = new GridBagConstraints();

		str[0] = "Vehicles";
		str[1] = "Weapons";
		str[2] = "Computers";
		str[3] = "Oil";
		str[4] = "Steel";
		str[5] = "Plastic";
		str[6] = "Copper";
		str[7] = "Rubber";
		str[8] = "Glass";
		str[9] = "Rupyulars";
		str[10] = "TRUCE_OFFERED";

		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 4;
		
		JPanel xx = new JPanel(new BorderLayout());
		xx.setBackground(bkgcolor);
		xx.add("North", new JLabel("Assets", JLabel.CENTER));

		JPanel xz = new JPanel(new GridLayout(1,2));
		xz.setBackground(bkgcolor);
		xz.add(new JLabel("offered", JLabel.LEFT));
		xz.add(new JLabel("have", JLabel.RIGHT));
		xx.add("Center", xz);
		xx.add("South", label = new JLabel(" "));
		label.setFont(new Font("Helvetica", Font.PLAIN, 6));
		p.add(xx, gc);
		
		for (int i=0; i < 10 ; i++) {
			JPanel xy = new JPanel(new BorderLayout());
			xy.setBackground(bkgcolor);
			xy.add("West", t[i] = new JTextField(2));
			t[i].setEditable(false);
			t[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			t[i].setHorizontalAlignment(JTextField.RIGHT);

			xy.add("Center", b[i] = new JButton(str[i]));
			
			xy.add("East", g[i] = new JTextField(2));
			g[i].setEditable(false);
			g[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			g[i].setHorizontalAlignment(JTextField.RIGHT);
			
			b[i].addActionListener(this);
			
			gc.gridx = 0;
			gc.gridy = i+1;
			gc.gridwidth = 3;
			p.add(xy, gc);
		}
		x.add(p);

		p = new JPanel(new BorderLayout());
		p.setBackground(bkgcolor);

		JPanel q = new JPanel(new GridLayout(3,1));
		q.setBackground(bkgcolor);

		q.add(accept = new JButton("Accept Truce"));
		q.add(new JLabel(" "));
		q.add(decline = new JButton("Reject Truce"));

		// The text above the number pad start
		JPanel r = new JPanel(new GridLayout(4,1));
		r.setBackground(bkgcolor);
		r.add(new JLabel(" "));
		r.add(new JLabel(" "));		
		r.add(new JLabel("Accept or reject the truce offer", JLabel.CENTER));
		r.add(new JLabel(" "));		
		// The text above the number pad end

		// The text and number pad start
		JPanel xs = new JPanel(new BorderLayout());
		xs.setBackground(bkgcolor);
		xs.add("North", r);
		xs.add("Center", q);
		xs.add("West", new JLabel("           "));
		xs.add("East", new JLabel("           "));
		xs.add("South", label = new JLabel("  "));
		label.setFont(new Font("Helvetica", Font.PLAIN, 170));
		// The text and number pad end

		// The Players list start
		JPanel xp = new JPanel(new FlowLayout());
		xp.setBackground(bkgcolor);		
		JPanel xr = new JPanel(new BorderLayout());
		xr.setBackground(bkgcolor);
		xr.add("Center", target);
		target.setEditable(false);
		target.setFont(new Font("Helvetica", Font.PLAIN, 14));
		target.setHorizontalAlignment(JTextField.CENTER);
	
		xp.add(xr);
		// The Players list end
		
		p.add("North", xp);
		p.add("Center", xs);

		x.add(p);

		add("Center", x);

		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);
		p.add(updateResources = new JButton("Update assets"));
		p.add(new JLabel("     "));
		p.add(help = new JButton("Help"));

		add("South", p);

		JPanel xq = new JPanel(new BorderLayout());
		xq.setBackground(bkgcolor);
		
		p = new JPanel(new GridLayout(1,2));
		p.setBackground(bkgcolor);
		p.add(new JLabel(" "));
		
		q = new JPanel(new GridLayout(2,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));		
		q.add(new JLabel("I was offered a truce by player", JLabel.LEFT));
		p.add(q);
		
		xq.add("Center", p);
		
		add("North", xq);

		updateResources.addActionListener(this);
		accept.addActionListener(this);
		decline.addActionListener(this);
		help.addActionListener(this);

		setResponses(response);

		(new PlayClip("arrive.wav")).start();
		
		setSize(500,470);
		setVisible(true);
	}

		public void setResponses (String response) {
		String [] res = new String [20];
		StringTokenizer st = new StringTokenizer(response, " ");
		int i=0;
		try {
			while (st.hasMoreTokens()) {
				res[i++] = st.nextToken();
			}
		} catch (Exception e) { }
		
		for (int j=0 ; j < 10 ; j++) {
			for (int k=0 ; k < i ; k++) {
				if (str[j].trim().equalsIgnoreCase(res[k])) {
					t[j].setText(res[k+1]);
					break;
				}
			}
		}
		target.setText(res[0]);
	}

	void updateAssets (String msg) {
		String assets[][] = new String[20][2];
		int i=0;

		// There are only 10 resources but room for 20 assets is reserved
		for (int k=0 ; k < 20 ; k++) { assets[k][0] = null; assets[k][1] = null; }
		
		StringTokenizer st = new StringTokenizer(msg," ");
		st.nextToken();
		try {
			while (st.hasMoreTokens()) {
				assets[i][0] = st.nextToken();
				assets[i][1] = st.nextToken();
				i++;
			}
		} catch (Exception e) { }

		for (int j=0 ; j < 10 ; j++) {
			i=0;
			while (assets[i][0] != null) {
				if (assets[i][0].equalsIgnoreCase(str[j])) {
					g[j].setText(assets[i][1]);
					break;
				}
				i++;
			}
		}
	}

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == help) {
			if (project.mousehelp != null) project.mousehelp.truce_offered_window();
			return;
		}
		out = "";
		if (evt.getSource() == accept) {
			out = "WAR_TRUCE_RESPONSE ACCEPT";
			notify();
			return;
		} else if (evt.getSource() == decline) {
			out = "WAR_TRUCE_RESPONSE DECLINE";
			notify();
			return;
		} else if (evt.getSource() == updateResources) {
			project.ac.execute("PLAYER_STATUS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		}
	}
	
	synchronized public String value() {
		try { wait(); } catch (Exception e) { }
		return out;
	}
}

