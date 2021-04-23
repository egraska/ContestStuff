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
class CrackParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JButton a[] = new JButton[10];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];
	JTextField h[] = new JTextField[10];
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	JButton cancel, OK, updatePlayers, updateResources, undo, help;
	String out="";
	int type;
	Project project;
	JComboBox <String> targets = null;
	JLabel label;

	public CrackParms (String ttl, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		project = pr;
		targets = project.copyBox(project.command.cmd12_arg1, false);		
		
		setTitle(ttl);
		setLayout(new BorderLayout());
		setBackground(bkgcolor);		
		
		JPanel x = new JPanel(new GridLayout(1,1));  // //////////////
		x.setBackground(bkgcolor);

		JPanel p = new JPanel(new GridBagLayout());
		p.setBackground(bkgcolor);
		GridBagConstraints gc = new GridBagConstraints();

		str[0] = "Computers";

		JPanel xx = new JPanel(new BorderLayout());
		xx.setBackground(bkgcolor);
		xx.add("North", new JLabel("Assets", JLabel.CENTER));

		JPanel xz = new JPanel(new GridLayout(1,2));
		xz.setBackground(bkgcolor);
		xz.add(new JLabel("commit", JLabel.LEFT));
		xz.add(new JLabel("have", JLabel.RIGHT));
		xx.add("Center", xz);
		xx.add("South", label = new JLabel(" "));
		label.setFont(new Font("Helvetica", Font.PLAIN, 6));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 5;
		p.add(xx, gc);
		
		t[0] = new JTextField(2);
		t[0].setText("");
		t[0].setEditable(false);
		t[0].setFont(new Font("Helvetica", Font.PLAIN, 20));
		t[0].setHorizontalAlignment(JTextField.RIGHT);
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 1;
		gc.gridwidth = 1;
		p.add(t[0], gc);

		b[0] = new JButton(str[0]);
		b[0].addActionListener(this);
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 1;
		gc.gridy = 1;
		gc.gridwidth = 3;
		p.add(b[0], gc);

		h[0] = new JTextField(2);
		h[0].setEditable(false);
		h[0].setFont(new Font("Helvetica", Font.PLAIN, 20));
		h[0].setHorizontalAlignment(JTextField.RIGHT);
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 4;
		gc.gridy = 1;
		gc.gridwidth = 1;
		p.add(h[0], gc);

		t[1] = new JTextField(2);
		t[1].setBackground(bkgcolor);
		t[1].setBorder(BorderFactory.createEmptyBorder());		
		gc.fill = GridBagConstraints.HORIZONTAL;		
		gc.gridx = 0;
		gc.gridy = 2;
		gc.gridwidth = 1;
		p.add(t[1], gc);

		JPanel s = new JPanel(new FlowLayout(FlowLayout.CENTER,0,10));
		s.setBackground(bkgcolor);
		s.add(undo = new JButton("Undo Computers"));
		gc.fill = GridBagConstraints.HORIZONTAL;		
		gc.gridx = 0;
		gc.gridy = 3;
		gc.gridwidth = 5;
		p.add(s, gc);		

		s = new JPanel(new FlowLayout(FlowLayout.CENTER,0,10));
		s.setBackground(bkgcolor);
		s.add(updatePlayers = new JButton("Update players"));
		gc.fill = GridBagConstraints.HORIZONTAL;		
		gc.gridx = 0;
		gc.gridy = 4;
		gc.gridwidth = 5;
		p.add(s, gc);		

		s = new JPanel(new FlowLayout(FlowLayout.CENTER,0,10));
		s.setBackground(bkgcolor);
		s.add(updateResources = new JButton("Update assets"));
		gc.fill = GridBagConstraints.HORIZONTAL;		
		gc.gridx = 0;
		gc.gridy = 5;
		gc.gridwidth = 5;
		p.add(s, gc);		
		
		x.add(p);

		add("Center", p);

		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);

		p.add(cancel = new JButton("Cancel"));
		p.add(new JLabel("  "));
		p.add(OK = new JButton("OK"));
		p.add(new JLabel("  "));
		p.add(help = new JButton("Help"));

		add("South", p);

		p = new JPanel(new BorderLayout());
		p.setBackground(bkgcolor);
		JPanel q = new JPanel(new GridLayout(4,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("Click Computers multiple times", JLabel.CENTER));
		q.add(new JLabel("to select the number to submit", JLabel.CENTER));
		q.add(new JLabel("and select a target to crack", JLabel.CENTER));
		p.add("North", q);

		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);
		q.add(targets);
		p.add("Center", q);

		add("North", p);

		undo.addActionListener(this);
		updatePlayers.addActionListener(this);
		updateResources.addActionListener(this);
		cancel.addActionListener(this);
		OK.addActionListener(this);
		help.addActionListener(this);

		(new PlayClip("win.wav")).start();
		
		setSize(320,420);
		setVisible(true);

		//h[0].setText("10");             // <- for testing only
		//targets.addItem("A_Player");    // <- for testing only
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
		if (evt.getSource() == help) {
			if (project.mousehelp != null) project.mousehelp.player_status_crack_window();
			return;
		} else if (evt.getSource() == updatePlayers) {
			project.ac.execute("GET_GAME_IDENTS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		} else if (evt.getSource() == updateResources) {
			project.ac.execute("PLAYER_STATUS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		} else if (evt.getSource() == undo) {
			t[0].setText("");
			return;
		} else if (evt.getSource() == b[0]) {
			int m = -1;
			try {
				m = Integer.parseInt(h[0].getText());
			} catch (Exception e) { }
			int num = 0;
			try {
				num = Integer.parseInt(t[0].getText());
			} catch (Exception e) { }
			if (m >= 0 && num < m && num < 99) 
				t[0].setText(String.valueOf(num+1));
			return;
		} 
		out = "";
		if (evt.getSource() == cancel) {
			notify();
			return;
		} else if (evt.getSource() == OK) {
			out = (String)targets.getSelectedItem();
			if (out == null || out.equals("")) {
				notify();
				return;
			}
			if (t[0].getText().equals("")) {
				notify();
				return;
			}
			out += " " + t[0].getText();
			notify();
			return;
		}
	}
	
	synchronized public String value() {
		try { wait(); } catch (Exception e) { }
		return out;
	}
}

