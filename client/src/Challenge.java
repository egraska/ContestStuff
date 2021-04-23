import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Challenge extends JFrame implements ActionListener {
	ConnectionHandler ch;
	JButton challenge = null;
	JButton trade_yes = null;
	JButton trade_no = null;
	JLabel l = null;
	Color  bkgcolor = new Color(255,255,223);
	JTextField text, genuine;
	int val;

	public Challenge (ConnectionHandler c) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		ch = c;
		
		JPanel p = new JPanel(new GridLayout(6,1));
		p.setBackground(bkgcolor);

		JPanel q = new JPanel(new BorderLayout());
		q.setBackground(bkgcolor);
		JPanel s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		q.add("North", new JLabel("      trade request from: "+ch.trade.getName()));
		s.add(text = new JTextField(40));
		q.add("Center", s);
		text.setText(ch.trade.getTrade());
		p.add(q);

		q = new JPanel(new BorderLayout());
		q.setBackground(bkgcolor);
		s = new JPanel(new FlowLayout());
		s.setBackground(bkgcolor);
		q.add("North", new JLabel(" "));
		s.add(genuine = new JTextField("Public Key Genuine?"));
		genuine.setBackground(ch.genuineColor);
		q.add("Center", s);
		p.add(q);
		
		p.add(l = new JLabel("Choose One", JLabel.CENTER));
		l.setFont(new Font("Helvetica", Font.BOLD, 20));
		
		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);
		q.add(challenge = new JButton("           Challenge           "));
		p.add(q);

 		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);		
		q.add(trade_yes = new JButton("Trade Response Accept"));
		p.add(q);
		
 		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);		
		q.add(trade_no = new JButton("Trade Response Decline"));
		p.add(q);

		add(p);

		challenge.addActionListener(this);
		trade_yes.addActionListener(this);
		trade_no.addActionListener(this);

		(new PlayClip("trade.wav")).run();

		setSize(600,300);
		setVisible(true);
	}

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == challenge) {	val = 1;	}
		else if (evt.getSource() == trade_yes) { val = 2;	}
		else if (evt.getSource() == trade_no) { val = 3; }
		notify();
	}

	synchronized public int value () {
		try { wait(); } catch (Exception e) { }
		return val;
	}
}
