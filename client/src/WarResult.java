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
public class WarResult extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];	
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	JButton OK;
	JLabel label;
	String out = null;

	public void setResponses (String response) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		String [] res = new String [20];
		StringTokenizer st = new StringTokenizer(response, " ");
		int i=0;
		try {
			while (st.hasMoreTokens()) {
				res[i++] = st.nextToken();
			}
		} catch (Exception e) { }
		for (int j=0 ; j < 7 ; j++) {
			for (int k=0 ; k < i ; k++) {
				if (str[j].trim().equalsIgnoreCase(res[k])) {
					if (j < 3)
						g[j].setText("  "+res[k+1]);
					else
						g[j].setText(res[k+1]);						
					break;
				}
			}
		}
	}

	public WarResult (String response) {
		JLabel label;
		
		setTitle("War Results");
		setLayout(new BorderLayout());
		setBackground(bkgcolor);		
		
		JPanel x = new JPanel(new GridLayout(1,2));
		x.setBackground(bkgcolor);

		JPanel p = new JPanel(new GridBagLayout());
		p.setBackground(bkgcolor);
		GridBagConstraints gc = new GridBagConstraints();

		str[0] = "Aggressor";
		str[1] = "Defender";
		str[2] = "Winner";
		str[3] = "Battles";
		str[4] = "Status";
		str[5] = " Weapons ";
		str[6] = "Vehicles";

		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 4;
		
		JPanel xx = new JPanel(new BorderLayout());
		xx.setBackground(bkgcolor);
		xx.add("North", new JLabel("War Results", JLabel.CENTER));
		xx.add("South", label = new JLabel(" "));
		label.setFont(new Font("Helvetica", Font.PLAIN, 6));
		p.add(xx, gc);
		
		for (int i=0; i < 7; i++) {
			JPanel xy = new JPanel(new BorderLayout());
			xy.setBackground(bkgcolor);
			xy.add("Center", b[i] = new JButton(str[i]));
			if (i < 3)
				xy.add("East", g[i] = new JTextField(16));
			else
				xy.add("East", g[i] = new JTextField(8));
			g[i].setEditable(false);
			g[i].setFont(new Font("Helvetica", Font.PLAIN, 18));
			if (i < 3)
				g[i].setHorizontalAlignment(JTextField.LEFT);
			else
				g[i].setHorizontalAlignment(JTextField.CENTER);
			
			gc.gridx = 0;
			gc.gridy = i+1;
			if (i < 3)
				gc.gridwidth = 3;
			else
				gc.gridwidth = 1;
			p.add(xy, gc);
		}
		x.add(p);

		add("Center", x);

		// Put the bottom row of two button into the applet
		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);

		p.add(OK = new JButton("OK"));

		add("South", p);
		// End bottom row

		OK.addActionListener(this);

		setResponses(response);

		(new PlayClip("war.wav")).start();
		
		setSize(470,320);
		setVisible(true);
	}

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == OK) {
			this.dispose();
		}
	}
}

