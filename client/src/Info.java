import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Info extends JFrame implements ActionListener {
	JLabel l = null;
	Color  bkgcolor = new Color(255,255,223);
	JButton ok;
	Thread runner;
	String type;

	public Info (String type, String info) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		this.type = type;
		JPanel p = new JPanel(new GridLayout(3,1));
		p.setBackground(bkgcolor);

		JPanel q = new JPanel(new GridLayout(1,1));
		q.setBackground(bkgcolor);
		q.add(l = new JLabel(type, JLabel.LEFT));
		l.setFont(new Font("Helvetica", Font.BOLD, 16));
		l.setForeground(Color.red);
		p.add(q);

		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);
		q.add("North", l = new JLabel(info, JLabel.CENTER));
		l.setFont(new Font("Helvetica", Font.BOLD, 16));
		p.add(q);

 		q = new JPanel(new FlowLayout());
		q.setBackground(bkgcolor);		
		q.add(ok = new JButton("OK"));
		p.add(q);
		
		add(p);

		ok.addActionListener(this);

		(new PlayClip("info-alert.wav")).start();

		setSize(900,130);
		setVisible(true);
	}

	synchronized public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == ok) {
			dispose();
		}
	}
}
