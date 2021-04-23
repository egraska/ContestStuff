import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class PublicKey extends JFrame implements ActionListener {
	JButton zkp = null;
	JButton rsa = null;
	JButton none = null;
	JLabel l = null;
	Color  bkgcolor = new Color(255,255,223);
	JTextField genuine;
	int val;

	public PublicKey () {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		JPanel p = new JPanel(new GridLayout(4,1));
		p.setBackground(bkgcolor);

		JPanel q = new JPanel(new BorderLayout());
		q.setBackground(bkgcolor);
		
		p.add(l = new JLabel("Choose One", JLabel.CENTER));
		l.setFont(new Font("Helvetica", Font.BOLD, 20));
		
 		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);		
		q.add(zkp = new JButton("ZKP Public Key"));
		p.add(q);
		
 		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);		
		q.add(rsa = new JButton("RSA Public Key"));
		p.add(q);

 		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);		
		q.add(none = new JButton("No Key"));
		p.add(q);

		add(p);

		zkp.addActionListener(this);
		rsa.addActionListener(this);
		none.addActionListener(this);

		(new PlayClip("trade.wav")).start();

		setSize(400,230);
		setVisible(true);
	}

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == rsa) { val = 1;	}
		else if (evt.getSource() == zkp) { val = 2; }
		else if (evt.getSource() == none) { val = 3; }		
		notify();
	}

	synchronized public int value () {
		try { wait(); } catch (Exception e) { }
		return val;
	}
}
