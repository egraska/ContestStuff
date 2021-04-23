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
class SynthesizeParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JButton a[] = new JButton[10];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];
	JTextField h[] = new JTextField[10];
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	JButton cancel, OK, updateResources, help;
	String out="";
	int type;
	Project project;
	JComboBox <String> targets = null;
	JLabel label;

	public SynthesizeParms (String ttl, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		project = pr;
		targets = project.copyBox(project.command.cmd12_arg1, false);		
		
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
		str[2] = "Computers";
		str[3] = "Oil";
		str[4] = "Steel";
		str[5] = "Plastic";
		str[6] = "Copper";
		str[7] = "Rubber";
		str[8] = "Glass";
		str[9] = "Rupyulars";
		
		JPanel xx = new JPanel(new BorderLayout());
		xx.setBackground(bkgcolor);
		xx.add("North", new JLabel("Assets", JLabel.CENTER));

		JPanel xz = new JPanel(new GridLayout(1,2));
		xz.setBackground(bkgcolor);
		xz.add(new JLabel("want", JLabel.LEFT));
		xz.add(new JLabel("have", JLabel.RIGHT));
		xx.add("Center", xz);
		xx.add("South", label = new JLabel(" "));
		label.setFont(new Font("Helvetica", Font.PLAIN, 6));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 5;
		p.add(xx, gc);

		for (int i=0; i < 10 ; i++) {
			t[i] = new JTextField(2);
			t[i].setText("");
			t[i].setEditable(false);
			t[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			t[i].setHorizontalAlignment(JTextField.RIGHT);
			if (i >= 3) {
				t[i].setBackground(bkgcolor);
				t[i].setBorder(BorderFactory.createEmptyBorder());
			}
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 0;
			gc.gridy = i+1;
			gc.gridwidth = 1;
			p.add(t[i], gc);

			b[i] = new JButton(str[i]);
			b[i].addActionListener(this);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 1;
			gc.gridy = i+1;
			gc.gridwidth = 3;
			p.add(b[i], gc);

			h[i] = new JTextField(2);
			h[i].setEditable(false);
			h[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			h[i].setHorizontalAlignment(JTextField.RIGHT);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 4;
			gc.gridy = i+1;
			gc.gridwidth = 1;
			p.add(h[i], gc);
		}

		x.add(p);

		add("Center", x);

		p = new JPanel(new FlowLayout(FlowLayout.CENTER,0,10));
		p.setBackground(bkgcolor);

		p.add(updateResources = new JButton("Update assets"));
		p.add(new JLabel("  "));
		p.add(cancel = new JButton("Cancel"));
		p.add(new JLabel("  "));
		p.add(OK = new JButton("OK"));
		p.add(new JLabel("  "));
		p.add(help = new JButton("Help"));

		add("South", p);

		p = new JPanel(new GridLayout(1,1));
		p.setBackground(bkgcolor);
		JPanel q = new JPanel(new GridLayout(4,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("Click Asset to Synthesize", JLabel.CENTER));
		q.add(new JLabel("Click more than once for", JLabel.CENTER));
		q.add(new JLabel("quantities larger than 1", JLabel.CENTER));
		p.add(q);

		add("North", p);

		updateResources.addActionListener(this);
		cancel.addActionListener(this);
		OK.addActionListener(this);
		help.addActionListener(this);

		(new PlayClip("win.wav")).start();
		
		setSize(400,470);
		setVisible(true);

		//for (int i=0 ; i < 10 ; i++) h[i].setText("10");  // <- for testing only
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
			if (project.mousehelp != null) project.mousehelp.synthesize_parms();
		} else if (evt.getSource() == updateResources) {
			project.ac.execute("PLAYER_STATUS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			return;
		}
		out = "";
		if (evt.getSource() == cancel) {
			notify();
			return;
		} else if (evt.getSource() == OK) {
			for (int i=0 ; i < 3 ; i++) {
				if (b[i].getBackground().equals(Color.red)) {
					if (t[i].getText().equals("")) {
						out = "";
						notify();
						return;
					}
					out += str[i] + " " + t[i].getText();
					notify();
					return;
				}
			}
		}
		for (int i=0 ; i < 3 ; i++) {
			if (evt.getSource() == b[i]) {
				if (b[i].getBackground().equals(Color.red)) {
					int n;
					try {
						n = Integer.parseInt(t[i].getText());
					} catch (Exception e) {
						n = 1;
					}
					n++;
					t[i].setText(String.valueOf(n));
				} else {
					for (int j =0 ; j < 3 ; j++) {
						b[j].setBackground(b[9].getBackground());
						t[j].setText("");
					}
					b[i].setBackground(Color.red);
					t[i].setText("1");
				}
				return;
			} 
		}
	}
	
	synchronized public String value() {
		try { wait(); } catch (Exception e) { }
		return out;
	}
}

