import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;
import java.io.*;
import java.net.*;
import java.math.*;
import java.security.*;

// Opens a box that lists some or all resources under 'I will trade' and some
// or all resources under 'for'.  Next to each resource in the 'I will trade'
// list is a TextField on the right and a TextField on the left.  When a player
// is logged in pressing the 'Update assets' button will populate TextField on
// the right with resources quantities currently held by the player.  A player
// assembles a set of resources and quantities to be offered for trade by clicking
// a resource in the 'I will trade' list and then using the center numeric pad
// to enter a value which is displayed in the left TextField adjacent to the
// resource clicked.  A player does the same on the 'for' list resources to
// assemble a set of resources and quantities that the player wishes to receive
// in the trade.  The target of the trade is selected in the 'with' menu.  To
// populate the 'with' menu a player presses the 'Update players' button.
//
// Use like this in the Project class:
//   tp = new TradeParms("box title goes here", 10, this);
//   String out = tp.value();
//   tp.dispose();
//   tp = null;
//
// The 1st line opens the box and control proceeds to the 2nd line which is
// stopped by the 'wait()' in the value function of TradeParms.  When OK or
// cancel are clicked a notify causes execution past the wait and a string is
// returned - the string was assembled before the notify.  Variable out gets
// that string for use later.  The 3rd line closes the box and the 4th line
// clears the reference to the box.
//
// The variable tp is declared in class Project and initialized to null.
// In class ActiveClient, handleResult, section PLAYER_STATUS, there needs to
// be a line
//    if (parent.tp != null) parent.tp.updateAssets(resultString);
// which activates the 'Update assets' button in TradeParms
// In the GET_GAME_IDENTS section of the ActiveClient's handleResult there needs
// to be a line
//    if (parent.tp != null)
//       parent.updateBox(parent.tp.targets, parent.command.cmd12_arg1, false);
// which activates the 'Update players' button in TradeParms.
class TradeParms extends JFrame implements ActionListener {
	JButton b[] = new JButton[10];
	JButton n[] = new JButton[11];
	JButton a[] = new JButton[10];
	JTextField t[] = new JTextField[10];
	JTextField g[] = new JTextField[10];
	JTextField h[] = new JTextField[10];
	String str[] = new String[10];
	Color bkgcolor = new Color(255,255,223);
	int which = -1; 
	JButton cancel, OK, updatePlayers, updateResources, help;
	String out="";
	int type;
	Project project;
	JComboBox <String> targets = null;
	JLabel label;

	public TradeParms (String ttl, int type, Project pr) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		project = pr;
		targets = project.copyBox(project.command.cmd12_arg1, false);		
		
		if (type >= 10) this.type = 10;
		else if (type < 1) this.type = 1;
		else this.type = type;
		
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

		JPanel s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		s.add(new JLabel("offer"));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 1;
		p.add(s, gc);

		s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		s.add(new JLabel("assets"));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 1;
		gc.gridy = 0;
		gc.gridwidth = 3;
		p.add(s, gc);

		s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		s.add(new JLabel("have"));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 4;
		gc.gridy = 0;
		gc.gridwidth = 1;
		p.add(s, gc);

		for (int i=0; i < type ; i++) {
			t[i] = new JTextField(2);
			t[i].setEditable(false);
			t[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			t[i].setHorizontalAlignment(JTextField.RIGHT);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 0;
			gc.gridy = i+1;
			gc.gridwidth = 1;
			p.add(t[i], gc);

			b[i] = new JButton(str[i]);
			b[i].addActionListener(this);
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

		p = new JPanel(new BorderLayout());
		p.setBackground(bkgcolor);

		// The number pad start
		JPanel q = new JPanel(new GridLayout(4,3));
		q.setBackground(bkgcolor);
		
		for (int i=0 ; i < 10 ; i++) {
			n[i] = new JButton(String.valueOf(i));
			n[i].addActionListener(this);
			q.add(n[i]);
		}
		q.add(new JLabel(" "));
		q.add(n[10] = new JButton("\u2936"));         // backspace
		n[10].setFont(new Font("Helvetica", Font.PLAIN, 18));
		n[10].addActionListener(this);
		// The number pad end

		// The text above the number pad start
		JPanel r = new JPanel(new GridLayout(5,1));
		r.setBackground(bkgcolor);
		r.add(new JLabel(" "));
		r.add(new JLabel("    Click an asset then use", JLabel.CENTER));
		r.add(new JLabel("    the pad to enter quantity", JLabel.CENTER));
		r.add(new JLabel("    or click asset repeatedly", JLabel.CENTER));
		r.add(new JLabel(" "));		
		// The text above the number pad end

		// The text and number pad start
		JPanel xs = new JPanel(new BorderLayout());
		xs.setBackground(bkgcolor);
		xs.add("North", r);
		xs.add("Center", q);
		xs.add("West", new JLabel("          "));
		xs.add("East", new JLabel("     "));
		xs.add("South", label = new JLabel("  "));
		label.setFont(new Font("Helvetica", Font.PLAIN, 40));
		// The text and number pad end

		// The Players list start
		JPanel xp = new JPanel(new FlowLayout());
		xp.setBackground(bkgcolor);		
		JPanel xr = new JPanel(new BorderLayout());
		xr.setBackground(bkgcolor);
		xr.add("Center", targets);
		xp.add(xr);
		// The Players list end
		
		p.add("North", xp);
		p.add("Center", xs);

		x.add(p);

		p = new JPanel(new GridBagLayout());
		p.setBackground(bkgcolor);
		gc = new GridBagConstraints();

		s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		s.add(new JLabel("want"));
		gc.fill = GridBagConstraints.HORIZONTAL;
		gc.gridx = 0;
		gc.gridy = 0;
		gc.gridwidth = 1;
		p.add(s, gc);

		for (int i=0; i < type ; i++) {
			g[i] = new JTextField(2);
			g[i].setEditable(false);
			g[i].setFont(new Font("Helvetica", Font.PLAIN, 20));
			g[i].setHorizontalAlignment(JTextField.RIGHT);
			gc.fill = GridBagConstraints.HORIZONTAL;
			gc.gridx = 0;
			gc.gridy = i+1;
			gc.gridwidth = 1;
			p.add(g[i], gc);

			a[i] = new JButton(str[i]);
			a[i].addActionListener(this);
			gc.gridx = 1;
			gc.gridy = i+1;
			gc.gridwidth = 3;
			p.add(a[i], gc);
		}

		x.add(p);

		add("Center", x);

		p = new JPanel(new FlowLayout());
		p.setBackground(bkgcolor);

		p.add(updateResources = new JButton("Update assets"));
		p.add(new JLabel("  "));
		p.add(updatePlayers = new JButton("Update players"));
		p.add(new JLabel("             "));
		p.add(cancel = new JButton("Cancel"));
		p.add(new JLabel("  "));
		p.add(OK = new JButton("OK"));
		p.add(new JLabel("  "));
		p.add(help = new JButton("Help"));

		add("South", p);

		p = new JPanel(new GridLayout(1,3));
		p.setBackground(bkgcolor);
		q = new JPanel(new GridLayout(2,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("I offer to trade these", JLabel.CENTER));
		p.add(q);
		q = new JPanel(new GridLayout(2,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("with", JLabel.CENTER));
		p.add(q);
		q = new JPanel(new GridLayout(2,1));
		q.setBackground(bkgcolor);
		q.add(new JLabel(" "));
		q.add(new JLabel("for these", JLabel.CENTER));
		p.add(q);
		add("North", p);

		updateResources.addActionListener(this);
		updatePlayers.addActionListener(this);
		cancel.addActionListener(this);
		OK.addActionListener(this);
		help.addActionListener(this);

		(new PlayClip("trade.wav")).start();
		
		setSize(650,420);
		setVisible(true);

		// targets.addItem("A_Player");                        // <- for testing only
		// for (int i=0 ; i < type ; i++) h[i].setText("20");  // <- for testing only
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

		for (int j=0 ; j < type ; j++) {
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
			if (project.mousehelp != null) project.mousehelp.trade_request_window();
		} else if (evt.getSource() == updatePlayers) {
			project.ac.execute("GET_GAME_IDENTS");
			project.ac.getMonitorMessage(null);
			project.ac.handleResult();
			targets.addItem("MONITOR");
			return;
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
			out = project.identity.getText();
			if (out.equals("")) {
				notify();
				return;
			}
			String tmp = "";
			for (int i=0 ; i < type ; i++) {
				if (t[i].getText().length() > 0) {
					tmp += " " + str[i] + " " + t[i].getText();
				}
			}
			if (tmp.equals("")) {
				out = "";
				notify();
				return;
			}
			out += tmp+" for ";

			tmp = (String)targets.getSelectedItem();
			if (tmp == null || tmp.equals("")) {
				out = "";
				notify();
				return;
			}

			out += tmp;

			tmp = "";
			for (int i=0 ; i < type ; i++) {
				if (g[i].getText().length() > 0) {
					tmp += " " + str[i] + " " +g[i].getText();
				}
			}
			if (tmp.equals("")) out = "";
			else out += tmp;
			notify();
			return;
		}
		for (int i=0 ; i < type ; i++) {
			if (evt.getSource() == b[i]) {
				which = i;
				int m = -1;
				try {
					m = Integer.parseInt(h[i].getText());
				} catch (Exception e) { }
				int num = 0;
				try {
					num = Integer.parseInt(t[i].getText());
				} catch (Exception e) { }
				if (m >= 0 && num < m && num < 99) t[i].setText(String.valueOf(num+1));
				return;
			} else if (evt.getSource() == a[i]) {
				which = i+20;
				int num = 0;
				try {
					num = Integer.parseInt(g[i].getText());
				} catch (Exception e) { }
				if (num < 99) g[i].setText(String.valueOf(num+1));
				return;
			}
		}
		if (evt.getSource() == n[10]) {
			if (which >= 20) {
				if (g[which-20].getText().length() > 0) {
					int len = g[which-20].getText().length();
					g[which-20].setText(g[which-20].getText().substring(0,len-1));
				}
			} else {
				if (t[which].getText().length() > 0) {
					int m = -1;
					try {
						m = Integer.parseInt(h[which].getText());
					} catch (Exception e) { }
					int num = 0;
					try {
						num = Integer.parseInt(t[which].getText());
					} catch (Exception e) { }
					if (m >= 0 && num <= m && num < 99) {
						int len = t[which].getText().length();
						t[which].setText(t[which].getText().substring(0,len-1));
					}
				}
			}
			return;
		}
		if (which < 0 || which > 40) return;
		for (int i=0 ; i < 10 ; i++) {
			if (evt.getSource() == n[i]) {
				if (which >= 20) {
					if (i > 0 || (i == 0 && g[which-20].getText().length() > 0)) {
						if (g[which-20].getText().length() < 2) {
							int num = -1;
							try {
								num = Integer.parseInt(g[which-20].getText()+String.valueOf(i));
							} catch (Exception e) { }
							if (num >= 0 && num < 99)
								g[which-20].setText(String.valueOf(num));
							else
								g[which-20].setText("0");
						}
					}
				} else {
					if (i > 0 || (i == 0 && t[which].getText().length() > 0)) {
						if (t[which].getText().length() < 2) {
							int m = -1;
							try {
								m = Integer.parseInt(h[which].getText());
							} catch (Exception e) { }
							int num = 0;
							try {
								num = Integer.parseInt(t[which].getText()+String.valueOf(i));
							} catch (Exception e) { }
							if (m >=0 && num <= m && num < 99)
								t[which].setText(String.valueOf(num));
						}
					}
				}
			}
		}
	}
	
	synchronized public String value() {
		try { wait(); } catch (Exception e) { }
		return out;
	}
}

