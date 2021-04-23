import javax.swing.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.net.URL;

public class Help extends JFrame implements ActionListener {
	JTextArea log = new JTextArea(24,60);
	JLabel title;
	boolean frame_location;
	JButton kill;

	public Help (String ttl, boolean fl) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		setLayout(new BorderLayout());
		add("Center", log);
		log.setEditable(false);
		add("North", title = new JLabel("   ", JLabel.CENTER));
		log.setFont(new Font("Helvetica", Font.PLAIN, 16));
		frame_location = fl;
		setUndecorated(true);
		if (frame_location) setLocationRelativeTo(null);
		log.setText("");
		title.setText(ttl);
		title.setFont(new Font("Helvetica", Font.BOLD, 18));
	}

	public Help (String ttl, boolean fl, String img) {
		setLayout(new BorderLayout());
		setBackground(Color.white);

		JPanel p = new JPanel(new BorderLayout(10,10));
		p.setBackground(Color.white);
		
		URL imgURL = getClass().getClassLoader().getResource(img);
		ImageIcon icon = new ImageIcon(imgURL);
		JLabel label = new JLabel(icon);
		p.add("Center", label);
		p.add("North", title = new JLabel("   ", JLabel.CENTER));
		add("North", p);
		add("Center", log);
		log.setEditable(false);
		log.setFont(new Font("Helvetica", Font.PLAIN, 16));
		frame_location = fl;
		setUndecorated(true);
		if (frame_location) setLocationRelativeTo(null);
		log.setText("");
		title.setText(ttl);
		title.setFont(new Font("Helvetica", Font.BOLD, 18));		
	}

	public Help (String ttl, boolean fl, boolean button) {
		setLayout(new BorderLayout());
		add("Center", log);
		log.setEditable(false);
		add("North", title = new JLabel("   ", JLabel.CENTER));
		log.setFont(new Font("Helvetica", Font.PLAIN, 16));
		frame_location = fl;
		setUndecorated(true);
		if (frame_location) setLocationRelativeTo(null);
		log.setText("");
		title.setText(ttl);
		title.setFont(new Font("Helvetica", Font.BOLD, 18));		
		JPanel p = new JPanel(new FlowLayout());
		p.add(kill = new JButton("Close"));
		add("South", p);
		kill.addActionListener(this);
	}

	public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == kill) {
			dispose();
		}
	}

	public void erase () { log.setText(""); title.setText(""); }

	public void append (String str) {
		log.append(str);
	}

	public void fontUp () {
		log.setFont(new Font("Helvetica", Font.PLAIN, 20));
	}

	public void fontDn () {
		log.setFont(new Font("Helvetica", Font.PLAIN, 14));
	}
	
	public void flush () {
		log.append("\n");
	}
}
