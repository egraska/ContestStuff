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
class WarStatusParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JButton a[] = new JButton[10];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];
	JTextField h[] = new JTextField[10];
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	JButton cancel, OK, updatePlayers, updateResources, undo;
	String out="";
	int type;
	Project project;
	JComboBox <String> targets = null;
	JLabel label;

	public WarStatusParms (String ttl, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		project = pr;
		targets = project.copyBox(project.command.cmd12_arg1, false);		
		
		setTitle(ttl);
		setLayout(new BorderLayout());
		setBackground(bkgcolor);		
		
		JPanel x = new JPanel(new GridLayout(2,1));
		x.setBackground(bkgcolor);

		JPanel p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);
		p.add(targets);

		x.add(p);

		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);
		p.add(updatePlayers = new JButton("Update players"));
		x.add(p);

		add("Center", x);

		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);

		p.add(cancel = new JButton("Cancel"));
		p.add(new JLabel("  "));
		p.add(OK = new JButton("OK"));

		add("South", p);

		p = new JPanel(new GridLayout(1,1));
		p.setBackground(bkgcolor);
		JPanel q = new JPanel(new GridLayout(4,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("Get Status of a War", JLabel.CENTER));
		q.add(new JLabel("Select a player that", JLabel.CENTER));
		q.add(new JLabel("is a combatant", JLabel.CENTER));
		p.add(q);

		add("North", p);

		updatePlayers.addActionListener(this);
		cancel.addActionListener(this);
		OK.addActionListener(this);

		(new PlayClip("war.wav")).start();
		
		setSize(320,220);
		setVisible(true);

		//targets.addItem("A_Player");         // for testing only
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

		i=0;
		while (assets[i][0] != null) {
			if (assets[i][0].equalsIgnoreCase(str[0])) {
				h[0].setText(assets[i][1]);
				break;
			}
			i++;
		}
	}	

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == updatePlayers) {
			project.ac.execute("GET_GAME_IDENTS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		} 
		out = "";
		if (evt.getSource() == cancel) {
			notify();
			return;
		} else if (evt.getSource() == OK) {
			String tmp = (String)targets.getSelectedItem();
			if (tmp == null || tmp.equals("")) {
				out = "";
				notify();
				return;
			}
			out = tmp;
			notify();
			return;
		}
	}
	
	synchronized public String value() {
		try { wait(); } catch (Exception e) { }
		return out;
	}
}

