import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.io.*;
import java.net.*;
import java.math.*;
import java.security.*;

// Opens a box that lists some or all resources.  Next to each resource is a
// TextField.  On the right side is a numeric pad plus backspace (BS).  The
// output is a string consisting of Resource Quantity pairs.  A user of the 
// box creates a string of pairs by clicking on a resource button, then using
// the numeric pad to enter a number in the TextField adjacent to the button
// that was clicked.  Click OK to output the string.  Click Cancel to output
// the empth string.  This appears in WAR_TRUCE_OFFER, WAR_DEFEND, WAR_DECLARE,
// and TRADE_REQUEST.  Use like this (assume all 10 resources will show):
//   TradeParms parm = new TradeParms("box title goes here", 10);
//   String out = parm.value();
//   parm.dispose();
//   parm = null;
// The 1st line opens the box and control proceeds to the 2nd line which is
// stopped by the 'wait()' in the value function of TradeParms.  When OK or
// cancel are clicked a notify causes execution past the wait and a string is
// returned - the string was assembled before the notify.  Variable out gets
// that string for use later.  The 3rd line closes the box and the 4th line
// clears the reference to the box.
public class StatusParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];	
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	JButton OK, updateResources, help;
	Project project;
	JLabel label;
	String out = null;

	public StatusParms (String ttl, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		JLabel label;
		project = pr;
		
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

		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 4;
		
		p.add(new JLabel("Assets",JLabel.CENTER), gc);
		
		for (int i=0; i < 10 ; i++) {
			JPanel xy = new JPanel(new BorderLayout());
			xy.setBackground(bkgcolor);
			xy.add("Center", b[i] = new JButton(str[i]));
			xy.add("East", g[i] = new JTextField(2));
			g[i].setEditable(false);
			g[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			g[i].setHorizontalAlignment(JTextField.RIGHT);
			
			gc.gridx = 0;
			gc.gridy = i+1;
			gc.gridwidth = 3;
			p.add(xy, gc);
		}

		JPanel q = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 25));
		q.setBackground(bkgcolor);
		q.add(updateResources = new JButton("Update assets"));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 1;
		gc.gridy = 11;
		gc.gridwidth = 4;
		p.add(q, gc);

		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);
		q.add(OK = new JButton("OK"));
		q.add(new JLabel("  "));
		q.add(help = new JButton("Help"));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 12;
		gc.gridwidth = 6;
		p.add(q, gc);

		x.add(p);

		add("Center", x);

		updateResources.addActionListener(this);
		OK.addActionListener(this);
		help.addActionListener(this);

		(new PlayClip("hold.wav")).start();
		
		setSize(250,450);
		setVisible(true);

		// for (int i=0 ; i < 10 ; i++) g[i].setText("10");  // <- for testing only
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
			if (project.mousehelp != null) project.mousehelp.status_window();
			return;
		} else if (evt.getSource() == updateResources) {
			project.ac.execute("PLAYER_STATUS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		}
		if (evt.getSource() == OK) {
			project.sp = null;
			this.dispose();
		}
	}
}

