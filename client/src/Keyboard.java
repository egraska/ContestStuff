import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

// This thread just lights a key (yellow) for 1/2 second
class light extends Thread {
   JButton button;

   public light (JButton button) {  this.button = button; }

   public void run () {
		try {
			button.setBackground(Color.yellow);
			button.update(button.getGraphics());
			try { sleep(500); } catch (Exception e) { }
			button.setBackground(new JButton().getBackground());
			button.update(button.getGraphics());
		} catch (Exception e) { }
   }
}

// Main class
public class Keyboard extends JPanel implements ActionListener, KeyListener {
   int b[] = new int[1000], count = -1;               // plaintext
   JLabel lbl1, lbl2, lbl3;
   JTextField msg;
   JButton keys[][] = new JButton[4][30];             // keyboard
   JButton space;                                     // space key
	JButton clear;
	JButton bs;                                        // backspace
   Color bkgcolor = new Color(255,255,223);
	Project project;

   public Keyboard (Project pr) {
		project = pr;
		
      setLayout(new BorderLayout(10,10));
      setBackground(bkgcolor);

      JPanel p = new JPanel(new GridLayout(4,1,0,-8));
      p.setBackground(bkgcolor);
		p.add(new JLabel("enter target hostname and port, separated by space",
							  JLabel.CENTER));
		p.add(new JLabel("  examples: 10.8.0.50 20101,  CYBERCLASS.UAS.EDU 20101",
							  JLabel.CENTER));
		p.add(new JLabel(" "));
		JPanel np = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, -2));
		np.setBackground(bkgcolor);
      np.add(msg = new JTextField(40));
		p.add(np);
      add("North", p);

      JPanel q = new JPanel(new FlowLayout());
		p = new JPanel(new GridLayout(5,1));
      p.setBackground(bkgcolor);
      q.setBackground(bkgcolor);
		
      q.add(keys[0][0] = new JButton("0"));
      q.add(keys[0][1] = new JButton("1"));
      q.add(keys[0][2] = new JButton("2"));
      q.add(keys[0][3] = new JButton("3"));
      q.add(keys[0][4] = new JButton("4"));
      q.add(keys[0][5] = new JButton("5"));
      q.add(keys[0][6] = new JButton("6"));
      q.add(keys[0][7] = new JButton("7"));
      q.add(keys[0][8] = new JButton("8"));
      q.add(keys[0][9] = new JButton("9"));
      p.add(q);
		
      q = new JPanel(new FlowLayout());
      q.setBackground(bkgcolor);
      q.add(keys[1][0] = new JButton("Q"));
      q.add(keys[1][1] = new JButton("W"));
      q.add(keys[1][2] = new JButton("E"));
      q.add(keys[1][3] = new JButton("R"));
      q.add(keys[1][4] = new JButton("T"));
      q.add(keys[1][5] = new JButton("Y"));
      q.add(keys[1][6] = new JButton("U"));
      q.add(keys[1][7] = new JButton("I"));
      q.add(keys[1][8] = new JButton("O"));
      q.add(keys[1][9] = new JButton("P"));
      p.add(q);
		
      q = new JPanel(new FlowLayout());
      q.setBackground(bkgcolor);
      q.add(keys[2][0] = new JButton("A"));
      q.add(keys[2][1] = new JButton("S"));
      q.add(keys[2][2] = new JButton("D"));
      q.add(keys[2][3] = new JButton("F"));
      q.add(keys[2][4] = new JButton("G"));
      q.add(keys[2][5] = new JButton("H"));
      q.add(keys[2][6] = new JButton("J"));
      q.add(keys[2][7] = new JButton("K"));
      q.add(keys[2][8] = new JButton("L"));
      q.add(keys[2][9] = new JButton(""));
      p.add(q);
		
      q = new JPanel(new FlowLayout());
      q.setBackground(bkgcolor);
      q.add(keys[3][0] = new JButton("Z"));
      q.add(keys[3][1] = new JButton("X"));
      q.add(keys[3][2] = new JButton("C"));
      q.add(keys[3][3] = new JButton("V"));
      q.add(keys[3][4] = new JButton("B"));
      q.add(keys[3][5] = new JButton("N"));
      q.add(keys[3][6] = new JButton("M"));
      q.add(keys[3][7] = new JButton("."));
      q.add(keys[3][8] = new JButton(""));
      q.add(keys[3][9] = new JButton(""));
      q.add(new JLabel("        "));
      p.add(q);
		
      q = new JPanel(new FlowLayout());
      q.setBackground(bkgcolor);
		q.add(clear = new JButton("Clear"));
		q.add(new JLabel("             "));
      q.add(space = new JButton("          Space          "));
		q.add(new JLabel("             "));
		q.add(bs = new JButton("Backspace"));
      p.add(q);

      add("Center", p);

		for (int i=0 ; i < 10 ; i++) keys[0][i].addActionListener(this);			
      for (int i=0 ; i < 10 ; i++) keys[1][i].addActionListener(this);
      for (int i=0 ; i < 9 ; i++) keys[2][i].addActionListener(this);
      for (int i=0 ; i < 8 ; i++) keys[3][i].addActionListener(this);
      space.addActionListener(this);
      clear.addActionListener(this);
		bs.addActionListener(this);
      for (int i=8 ; i < 10 ; i++) keys[3][i].setVisible(false); 
      keys[2][9].setVisible(false);

      msg.setFont(new Font("Courier", Font.PLAIN, 18));
      msg.addKeyListener(this);
   }

   public JButton xlate (char ch) {
      switch (ch) {
		case '\b': return bs;
      case ' ':  return space;
		case '0': return keys[0][0];
		case '1': return keys[0][1];
		case '2': return keys[0][2];
		case '3': return keys[0][3];
		case '4': return keys[0][4];
		case '5': return keys[0][5];
		case '6': return keys[0][6];
		case '7': return keys[0][7];
		case '8': return keys[0][8];
		case '9': return keys[0][9];
      case 'Q': case 'q': return keys[1][0];
      case 'W': case 'w': return keys[1][1];
      case 'E': case 'e': return keys[1][2];
      case 'R': case 'r': return keys[1][3];
      case 'T': case 't': return keys[1][4];
      case 'Y': case 'y': return keys[1][5];
      case 'U': case 'u': return keys[1][6];
      case 'I': case 'i': return keys[1][7];
      case 'O': case 'o': return keys[1][8];
      case 'P': case 'p': return keys[1][9];
      case 'A': case 'a': return keys[2][0];
      case 'S': case 's': return keys[2][1];
      case 'D': case 'd': return keys[2][2];
      case 'F': case 'f': return keys[2][3];
      case 'G': case 'g': return keys[2][4];
      case 'H': case 'h': return keys[2][5];
      case 'J': case 'j': return keys[2][6];
      case 'K': case 'k': return keys[2][7];
      case 'L': case 'l': return keys[2][8];
      case 'Z': case 'z': return keys[3][0];
      case 'X': case 'x': return keys[3][1];
      case 'C': case 'c': return keys[3][2];
      case 'V': case 'v': return keys[3][3];
      case 'B': case 'b': return keys[3][4];
      case 'N': case 'n': return keys[3][5];
      case 'M': case 'm': return keys[3][6];
		case '.': return keys[3][7];
      }
      return null;
   }

   public void actionPerformed (ActionEvent evt) {
		if (evt.getSource() == clear) {
			msg.setText("");
			return;
		}
		for (int i=0 ; i < 10 ; i++) {
			if (evt.getSource() == keys[0][i]) {
				msg.removeActionListener(this);
				char ch = keys[0][i].getText().charAt(0);
				msg.setText(msg.getText()+String.valueOf(ch));
				(new light(keys[0][i])).start();
				msg.addActionListener(this);
				return;
			}
		}
		for (int i=0 ; i < 10 ; i++) {
			if (evt.getSource() == keys[1][i]) {
				msg.removeActionListener(this);
				char ch = keys[1][i].getText().charAt(0);
				msg.setText(msg.getText()+String.valueOf(ch));
				(new light(keys[1][i])).start();
				msg.addActionListener(this);
				return;
			}
		}
		for (int i=0 ; i < 9 ; i++) {
			if (evt.getSource() == keys[2][i]) {
				msg.removeActionListener(this);
				char ch = keys[2][i].getText().charAt(0);
				msg.setText(msg.getText()+String.valueOf(ch));
				(new light(keys[2][i])).start();
				msg.addActionListener(this);
				return;
			}
		}
		for (int i=0 ; i < 8 ; i++) {
			if (evt.getSource() == keys[3][i]) {
				msg.removeActionListener(this);
				char ch = keys[3][i].getText().charAt(0);
				msg.setText(msg.getText()+String.valueOf(ch));
				(new light(keys[3][i])).start();
				msg.addActionListener(this);
				return;
			}
		}
		if (evt.getSource() == space) {
			msg.removeActionListener(this);
			char ch = ' ';
			msg.setText(msg.getText()+String.valueOf(ch));
			(new light(space)).start();
			msg.addActionListener(this);
		} else if (evt.getSource() == bs) {
			if (msg.getText().length() > 0) {
				msg.setText(msg.getText().substring(0, msg.getText().length()-1));
			}
		}
	}

	public void keyPressed(KeyEvent evt) { }
	public void keyReleased(KeyEvent evt) { }
	
	public void keyTyped(KeyEvent evt) { 
		char ch = evt.getKeyChar();
		(new light(xlate(ch))).start();
	}
}
