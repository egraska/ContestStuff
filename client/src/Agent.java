import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Agent extends JPanel implements ActionListener, MouseListener {
   JButton on, off;
   JPanel ind1, ind2;
   boolean state;
   ActOnButton bon, boff;
   Project pr;
   String name;
	Help a1;	

   public Agent(String nm, Project pr) {
      this.bon = bon;
      this.boff = boff;
      this.name = nm;
      this.pr = pr;

      if (name.equals("Server")) {
         bon = new StartPS(pr);
         boff = new StopPS(pr);
      } else {
         bon = new StartAC(pr);
         boff = new StopAC(pr);
      }
      setLayout(new BorderLayout());
		
      JPanel p = new JPanel(new GridLayout(2,1));
		
      p.add(ind1 = new JPanel());
      p.add(ind2 = new JPanel());
      add("West", p);
		
      p = new JPanel(new GridLayout(2,1));
		
      p.add(on = new JButton("Connect"));
      p.add(off = new JButton("Disconnect"));
      add("Center", p);
		
      add("South", new JLabel(nm, JLabel.CENTER));
		add("North", new JLabel(" "));
		
      on.addActionListener(this);
		on.addMouseListener(this);
      off.addActionListener(this);
		off.addMouseListener(this);
      state = false;
      ind1.setSize(60,60);
      ind1.setVisible(true);
      ind1.setBackground(Color.red);
      ind2.setSize(60,60);
      ind2.setVisible(true);
      ind2.setBackground(Color.red);
      if (name.equals("Server")) pr.srvport.setEditable(false);
   }

   public void actionPerformed (ActionEvent evt) {
      pr.command.arg[4].setText(pr.srvport.getText());      
      if (evt.getSource() == on) {
         if (state) return;
         if (!bon.doit()) return;
         state = true;
         ind1.setBackground(Color.green);
         ind2.setBackground(Color.green);
         if (name.equals("Server")) {
            pr.srvport.setEditable(false);
            pr.srvlocation.setEditable(false);
         }
         pr.monlocation.setEditable(false);
			pr.monport.setEditable(false);
         pr.identity.setEditable(false);
         pr.passwd.setEditable(false);
      } else if (evt.getSource() == off) {
         if (!state) return;
         state = false;
         ind1.setBackground(Color.red);
         ind2.setBackground(Color.red);
         if (name.equals("Server")) {
            if (pr.sport.equals("")) pr.srvport.setEditable(true);
            if (pr.sloc.equals("")) pr.srvlocation.setEditable(true);
				if (!boff.doit()) return;
         } else {
				pr.state.initiate();
				pr.ac.execute("QUIT");
				pr.ac.getMonitorMessage(null);
				pr.ac.handleResult();
				if (!boff.doit()) return;   /***** JVF *****/
			}

         if (!pr.client.state && !pr.server.state) {
            if (pr.mloc.equals("")) {
					pr.monlocation.setEditable(true);
					pr.monport.setEditable(true);
				}
            if (pr.name.equals("")) {
					pr.identity.setEditable(true);
					pr.passwd.setEditable(true);
				}
         }
      }
   }

	public void mouseEntered (MouseEvent evt) {
		if (!pr.helpstate) return;
		if (evt.getSource() == on) {
			if (name.equals("Server"))
				pr.mousehelp.server_connect();
			else
				pr.mousehelp.client_connect();
		} else if (evt.getSource() == off) {
			if (name.equals("Server")) {
				pr.mousehelp.server_disconnect();
			} else {
				pr.mousehelp.client_disconnect();
			}
		}
	}

	public void exit () {
		if (a1 != null) a1.dispose();
		a1 = null;
	}
	
	public void mouseExited (MouseEvent evt) { }
	public void mouseClicked (MouseEvent evt) { exit(); }
	public void mouseReleased (MouseEvent evt) { }
	public void mousePressed (MouseEvent evt) { }
}

