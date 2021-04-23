import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.io.*;
import java.net.*;
import java.math.*;
import java.security.*;

// Opens a box that lists all resources.  On either side of a resource that
// can be synthesized is a TextField.  On the right side of all other resources
// is a TextField.  All right side TextFields will display current resource
// holdings if the player is logged in and presses the 'Update assets' button.
// The left side TextFields display the quantity of resources that the player
// wishes to synthesize.  Only one resource can be synthesized at a time.  To
// choose a resource to synthesize click on the resource's button.  Multiple
// clicks increase the quantity.  Click another resource to reset.  Click
// OK when done.  Unless no numbers are in the 'want' column or the Cancel button
// is pressed the output generated is a string consisting of the selected
// resource, followed by a space, followed by the quantity in the selected
// resource's 'want' TextField.
//
// Use like this in the code: 
//   sp = new SynthesizeParms("box title goes here", 10);
//   String out = sp.value();
//   sp.dispose();
//   sp = null;
//
// The 1st line opens the box and control proceeds to the 2nd line which is
// stopped by the 'wait()' in the value function of SynthesizeParms.  When
// OK or cancel is clicked a notify causes execution past the wait and a
// string is returned - the string is assembled before the notify.  Variable
// out gets that string for use later.  The 3rd line closes the box and the
// 4th line clears the reference to the box.
//
// The variable sp is declared in class Project and initialized to null.
// In class ActiveClient, handleResult, section PLAYER_STATUS, there needs to
// be a line
//    if (parent.sp != null) parent.sp.updateAssets(resultString);
// which activates the 'Update assets' button in SynthesizeParms
class DefendParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JButton a[] = new JButton[10];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];
	JTextField h[] = new JTextField[10];
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	JButton cancel, OK, updateResources, reset, help;
	String out="";
	int type;
	Project project;
	JTextField target = new JTextField();
	JTextField txt;
	JLabel label;

	public DefendParms (String ttl, String aggressor, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		project = pr;

		setTitle(ttl);
		setLayout(new BorderLayout());
		setBackground(bkgcolor);		
		
		JPanel x = new JPanel(new GridLayout(1,3));
		x.setBackground(bkgcolor);

		JPanel p = new JPanel(new GridBagLayout());
		p.setBackground(bkgcolor);
		GridBagConstraints gc = new GridBagConstraints();

		str[0] = "Vehicles";
		str[1] = "Weapons";
		str[2] = "";
		
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

		for (int i=0; i < 2 ; i++) {
			t[i] = new JTextField(2);
			t[i].setText("0");
			t[i].setEditable(false);
			t[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			t[i].setHorizontalAlignment(JTextField.RIGHT);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 0;
			gc.gridy = i+1;
			gc.gridwidth = 1;
			p.add(t[i], gc);

			b[i] = new JButton(str[i]);
			if (i == 2) b[i].setVisible(false);
			b[i].addActionListener(this);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 1;
			gc.gridy = i+1;
			gc.gridwidth = 3;
			p.add(b[i], gc);

			h[i] = new JTextField(2);
			if (i == 2) h[i].setVisible(false);
			h[i].setEditable(false);
			h[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			h[i].setHorizontalAlignment(JTextField.RIGHT);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 4;
			gc.gridy = i+1;
			gc.gridwidth = 1;
			p.add(h[i], gc);
		}

		t[2] = new JTextField(2);
		t[2].setText("");
		t[2].setBackground(bkgcolor);
		t[2].setBorder(BorderFactory.createEmptyBorder());
		t[2].setEditable(false);
		t[2].setFont(new Font("Helvetica", Font.PLAIN, 20));
		t[2].setHorizontalAlignment(JTextField.RIGHT);
		t[2].setBackground(bkgcolor);
		t[2].setBorder(BorderFactory.createEmptyBorder());
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 3;
		gc.gridwidth = 1;
		p.add(t[2], gc);
		
		target.setFont(new Font("Helvetica", Font.PLAIN, 18));
		target.setEditable(false);
		target.setHorizontalAlignment(JTextField.CENTER);
      if (aggressor != null) target.setText(aggressor);
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 1;
		gc.gridy = 4;
		gc.gridwidth = 3;
		p.add(target, gc);

		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 5;
		gc.gridwidth = 5;
		p.add(new JLabel("Aggressor", JLabel.CENTER), gc);

		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 6;
		gc.gridwidth = 5;
		p.add(txt = new JTextField(), gc);
		txt.setBackground(bkgcolor);
		txt.setBorder(BorderFactory.createEmptyBorder());
		txt.setFont(new Font("Helvetica", Font.PLAIN, 22));

		JPanel s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		s.add(new JLabel("    "));
		s.add(reset = new JButton("Reset Commits"));
		s.add(new JLabel("  "));
		s.add(help = new JButton("Help"));
		
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 7;
		gc.gridwidth = 4;
		p.add(s, gc);

		x.add(p);

		add("Center", x);

		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);

		p.add(updateResources = new JButton("Update assets"));
		p.add(new JLabel("  "));
		p.add(cancel = new JButton("Cancel"));
		p.add(new JLabel("  "));
		p.add(OK = new JButton("OK"));

		add("South", p);

		p = new JPanel(new GridLayout(1,1));
		p.setBackground(bkgcolor);
		JPanel q = new JPanel(new GridLayout(5,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("Click asset multiple times", JLabel.CENTER));
		q.add(new JLabel("to set desired quantity", JLabel.CENTER));
		q.add(new JLabel("Click 'Reset Commits' to", JLabel.CENTER));
		q.add(new JLabel("zero out both quantities", JLabel.CENTER));		
		p.add(q);

		add("North", p);

		reset.addActionListener(this);
		updateResources.addActionListener(this);
		cancel.addActionListener(this);
		OK.addActionListener(this);
		help.addActionListener(this);

		(new PlayClip("win.wav")).start();
		
		setSize(340,400);
		setVisible(true);

		//for (int i=0 ; i < 2 ; i++) h[i].setText("10");  // <- for testing only
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
					h[j].setText(assets[i][1]);
					break;
				}
				i++;
			}
		}
	}	

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == help) {
			if (project.mousehelp != null) project.mousehelp.defend_window();
		} else if (evt.getSource() == updateResources) {
			project.ac.execute("PLAYER_STATUS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		} else if (evt.getSource() == reset) {
			t[0].setText("0");
			t[1].setText("0");
			return;
		}
		out = "";
		if (evt.getSource() == cancel) {
			notify();
			return;
		} else if (evt.getSource() == OK) {
			if (t[0].getText().equals("")) t[0].setText("0");
			if (t[1].getText().equals("")) t[1].setText("0");
			out = "WAR_DEFEND "+str[0]+" "+t[0].getText()+" "+str[1]+ " "+t[1].getText();
			notify();
			return;
		}
		if (evt.getSource() == b[0]) {
			int m = -1;
			try {
				m = Integer.parseInt(h[0].getText());
			} catch (Exception e) { }
			int n = -1;
			try {
				n = Integer.parseInt(t[0].getText());
			} catch (Exception e) {	}
			n++;
			if (m != -1 && n <= m && n <= 40) t[0].setText(String.valueOf(n));
			return;
		} else if (evt.getSource() == b[1]) {
			int m = -1;
			try {
				m = Integer.parseInt(h[1].getText());
			} catch (Exception e) { }
			int n = -1;
			try {
				n = Integer.parseInt(t[1].getText());
			} catch (Exception e) {
				n = 0;
			}
			n++;
			if (m != -1 && n <= m && n <= 40) t[1].setText(String.valueOf(n));
			return;
		}
	}
	
	synchronized public String value() {
		try { wait(); } catch (Exception e) { }
		return out;
	}
}

