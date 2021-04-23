import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class CommandPanel extends JPanel
	implements ActionListener, MouseListener {

	JPanel ind[];
	JButton cmd[];
	JTextField arg[];
   JComboBox <String> cmd12_arg1;
   JLabel title, label;
   String result;
   Project gui;
	String privKey = null;

   public CommandPanel (String name, Project gui) {
		ind = new JPanel[40];
		cmd = new JButton[40];
		arg = new JTextField[40];
		for (int i=0 ; i < 40 ; i++) {
			ind[i] = new JPanel();
			arg[i] = null;
			cmd[i] = null;
		}

      this.gui = gui;
      setLayout(new BorderLayout());

      // The Command and Argument buttons
      JPanel p = new JPanel(new GridLayout(10,4,5,2));

      // row 1
		p.add(makeButton(ind[0], cmd[0] = new JButton("Start Encryption")));
      p.add(new JLabel("  "));
      p.add(new JLabel("  "));
      p.add(new JLabel("  "));

      // row 2
		p.add(makeButton(ind[1], cmd[1] = new JButton("IDENT")));
		p.add(arg[0] = new JTextField(15));
		p.add(arg[1] = new JTextField(15));
      p.add(new JLabel("  "));
		arg[0].setEditable(false);
		arg[0].setHorizontalAlignment(JTextField.CENTER);
		arg[1].setEditable(false);

      // row 3
		p.add(makeButton(ind[2], cmd[2] = new JButton("PASSWORD")));
		p.add(arg[2] = new JTextField(15));
		p.add(makeButton(ind[3], cmd[3] = new JButton("RANDOM_PLAYER_HOST_PORT")));
		p.add(makeButton(ind[4], cmd[4] = new JButton("WAR_DECLARE")));
		arg[2].setEditable(false);
		arg[2].setHorizontalAlignment(JTextField.CENTER);

      // row 4
		p.add(makeButton(ind[5], cmd[5] = new JButton("HOST_PORT")));
		p.add(arg[3] = new JTextField(15));
		p.add(arg[4] = new JTextField(15));
		p.add(makeButton(ind[6], cmd[6] = new JButton("WAR_STATUS")));
		arg[3].setHorizontalAlignment(JTextField.CENTER);
		arg[4].setHorizontalAlignment(JTextField.CENTER);

      // row 5
		p.add(makeButton(ind[7], cmd[7] = new JButton("CHANGE_PASSWORD")));
		p.add(arg[5] = new JTextField(15));
		p.add(makeButton(ind[8], cmd[8] = new JButton("QUIT")));
		p.add(makeButton(ind[9], cmd[9] = new JButton("WAR_TRUCE_OFFER")));

      // row 6
		p.add(makeButton(ind[10], cmd[10] = new JButton("ALIVE")));
      p.add(new JLabel(" "));
		p.add(makeButton(ind[11], cmd[11] = new JButton("SIGN_OFF")));
		p.add(new JLabel(" "));

      // row 7
		p.add(makeButton(ind[12], cmd[12] = new JButton("Delete public key")));
		p.add(arg[6] = new JTextField(""));
		p.add(makeButton(ind[13], cmd[13] = new JButton("SYNTHESIZE")));
		p.add(makeButton(ind[14], cmd[14] = new JButton("Public Key ZKP")));
		arg[6].setBackground(new JButton().getBackground());
      arg[6].setForeground(Color.black);
      arg[6].setHorizontalAlignment(JTextField.CENTER);
      arg[6].setFont(new Font("Helvetica", Font.BOLD, 13));
      arg[6].setEditable(false);

      // row 8
		p.add(makeButton(ind[15], cmd[15] = new JButton("MAKE_CERTIFICATE")));
		p.add(arg[7] = new JTextField(15));
		p.add(arg[8] = new JTextField(15));
		p.add(makeButton(ind[16], cmd[16] = new JButton("Public Key RSA")));
		arg[7].setEditable(false);
      arg[8].setEditable(false);

      // row 9
		p.add(makeButton(ind[17], cmd[17] = new JButton("GET_CERTIFICATE")));
		p.add(makeButton(ind[18], cmd[18] = new JButton("GET_GAME_IDENTS")));
		p.add(makeButton(ind[19], cmd[19] = new JButton("PLAYER_STATUS_CRACK")));
		p.add(new JLabel(" "));
		
      // row 10
		p.add(makeButton(ind[20], cmd[20] = new JButton("PLAYER_HOST_PORT")));
      p.add(cmd12_arg1 = new JComboBox <String> ());
		p.add(makeButton(ind[21], cmd[21] = new JButton("PLAYER_STATUS")));
		p.add(makeButton(ind[22], cmd[22] = new JButton("TRADE_REQUEST")));
		
      add("Center", p);
      add("North",title = new JLabel(name, JLabel.CENTER));
      title.setFont(new Font("TimesRoman", Font.BOLD, 20));

		for (int i=0 ; i < 40 ; i++) {
			if (cmd[i] != null) {
				cmd[i].addActionListener(this);
				cmd[i].addMouseListener(this);
			}
			if (arg[i] != null) {
				arg[i].addMouseListener(this);
			}
		}
		cmd12_arg1.addMouseListener(this);
   }

	public JPanel makeButton (JPanel ind, JButton button) {
		JPanel xr = new JPanel(new BorderLayout());
		xr.add("West", ind);
		ind.setBackground(Color.black);
		ind.setSize(60,60);
		ind.setVisible(true);
		xr.add("Center", button);
		return xr;
	}
   
   synchronized public String command () {
      try {  wait(); } catch (Exception e) { }
      return result;
   }
   
   public void mouseEntered (MouseEvent evt) {
      if (!gui.helpstate) return;
		for (int i=0 ; i < 40 ; i++) {
			if (cmd[i] != null && evt.getSource() == cmd[i]) {
				gui.mousehelp.xlate(i);
				return;
			}
		}
		
		for (int i=0 ; i < 40 ; i++) {
			if (arg[i] != null && evt.getSource() == arg[i]) {
				gui.mousehelp.xlate(i+100);
				return;
			}
		}
		if (evt.getSource() == cmd12_arg1) {
			gui.mousehelp.xlate(200);
			return;
		}
   }
   
   public void mouseExited (MouseEvent evt) { }
   public void mouseClicked (MouseEvent evt) { gui.mousehelp.exit(); }
   public void mousePressed (MouseEvent evt) { }
   public void mouseReleased (MouseEvent evt) { }	
   
   synchronized public void actionPerformed (ActionEvent evt) {
		result = new String("Done");
		if (evt.getSource() == cmd[0]) {
			if (!gui.state.canBeInvoked(0)) return;
         gui.encryption_on = true;
         ind[0].setBackground(Color.green);
         return;
      } else if (evt.getSource() == cmd[1]) {
			if (!gui.state.canBeInvoked(1)) return;
         result = new String("IDENT");
      } else if (evt.getSource() == cmd[2]) {
			if (!gui.state.canBeInvoked(2)) return;
         result = new String("PASSWORD");
      } else if (evt.getSource() == cmd[3]) {
			if (!gui.state.canBeInvoked(3)) return;
         result = new String("RANDOM_PLAYER_HOST_PORT");
      } else if (evt.getSource() == cmd[4]) {
			if (!gui.state.canBeInvoked(4)) return;
         result = new String("WAR_DECLARE");
      } else if (evt.getSource() == cmd[5]) {
			if (!gui.state.canBeInvoked(5)) return;
         result = new String("HOST_PORT");
      } else if (evt.getSource() == cmd[6]) {
			if (!gui.state.canBeInvoked(6)) return;
         result = new String("WAR_STATUS");
      } else if (evt.getSource() == cmd[7]) {
			if (!gui.state.canBeInvoked(7)) return;
         result = new String("CHANGE_PASSWORD");
      } else if (evt.getSource() == cmd[8]) {
			if (!gui.state.canBeInvoked(8)) return;
         result = new String("QUIT");
      } else if (evt.getSource() == cmd[9]) {
			if (!gui.state.canBeInvoked(9)) return;
         result = new String("WAR_TRUCE_OFFER");
      } else if (evt.getSource() == cmd[10]) {
			if (!gui.state.canBeInvoked(10)) return;
         result = new String("ALIVE");
      } else if (evt.getSource() == cmd[11]) {
			if (!gui.state.canBeInvoked(11)) return;
         result = new String("SIGN_OFF");
      } else if (evt.getSource() == cmd[12]) {
			if (!gui.state.canBeInvoked(12)) return;
			arg[7].setText("");
			arg[8].setText("");
			gui.state.deletePublicKey();
			return;
      } else if (evt.getSource() == cmd[13]) {
			if (!gui.state.canBeInvoked(13)) return;
         result = new String("SYNTHESIZE");
		} else if (evt.getSource() == cmd[14]) {
			if (!gui.state.canBeInvoked(14)) return;
			Pitbull.initializeUserZKPKey(gui.identity.getText(), false);
			gui.state.deletePublicKey();
			return;
      } else if (evt.getSource() == cmd[15]) {
			if (!gui.state.canBeInvoked(15)) return;
         result = new String("MAKE_CERTIFICATE");
      } else if (evt.getSource() == cmd[16]) {
			if (!gui.state.canBeInvoked(16)) return;
         Pitbull.initializeUserRSAKey(gui.identity.getText(), false);
			gui.state.deletePublicKey();			
			return;
      } else if (evt.getSource() == cmd[17]) {
			if (!gui.state.canBeInvoked(17)) return;
         result = new String("GET_CERTIFICATE");
      } else if (evt.getSource() == cmd[18]) {
			if (!gui.state.canBeInvoked(18)) return;
         result = new String("GET_GAME_IDENTS"); 
      } else if (evt.getSource() == cmd[19]) {
			if (!gui.state.canBeInvoked(19)) return;
			result = new String("PLAYER_STATUS_CRACK");
      } else if (evt.getSource() == cmd[20]) {
			if (!gui.state.canBeInvoked(20)) return;
         result = new String("PLAYER_HOST_PORT");
      } else if (evt.getSource() == cmd[21]) {
			if (!gui.state.canBeInvoked(21)) return;
         result = new String("PLAYER_STATUS");
      } else if (evt.getSource() == cmd[22]) {
			if (!gui.state.canBeInvoked(22)) return;
         result = new String("TRADE_REQUEST");
      } else
         result = new String("Done");
      notify();
   }
}
