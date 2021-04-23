import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TradeResponse extends JFrame implements ActionListener {
   ConnectionHandler ch;
   JButton trade_yes = null;
   JButton trade_no = null;
   JLabel l = null;
   Color  bkgcolor = new Color(255,255,223);
   JTextField text, genuine;
   int val;
   String response;
   Color color;

   public TradeResponse (ConnectionHandler c, String res, Color col, int show_trade_string) {
      this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
      ch = c;
      response = res;
      color = col;
      
      JPanel p = new JPanel(new GridLayout(4,1));
      p.setBackground(bkgcolor);

      JPanel q = new JPanel(new BorderLayout());
      q.setBackground(bkgcolor);
      JPanel s = new JPanel(new FlowLayout());
      s.setBackground(bkgcolor);
      s = new JPanel(new FlowLayout());
      s.setBackground(bkgcolor);
      if (show_trade_string == 1) {
         q.add("North", new JLabel("      trade request from: "+ch.trade.getName()));
         s.add(genuine = new JTextField(20));
         q.add("Center", s);
         genuine.setText("  "+ch.trade.getTrade()+"  ");                    
         //s.add(genuine = new JTextField("  "+ch.trade.getTrade()+"  "));
      } else {
         q.add("North", new JLabel(" "));
         s.add(genuine = new JTextField("  "+response+"  "));
      }
      genuine.setBackground(color);
      genuine.setFont(new Font("Helvetica", Font.PLAIN, 17));
      q.add("Center", s);
      p.add(q);
      
      p.add(l = new JLabel("Choose One", JLabel.CENTER));
      l.setFont(new Font("Helvetica", Font.BOLD, 20));
      
      q = new JPanel(new FlowLayout());
      q.setBackground(bkgcolor);      
      q.add(trade_yes = new JButton("Trade Response Accept"));
      p.add(q);
      
      q = new JPanel(new FlowLayout());
      q.setBackground(bkgcolor);      
      q.add(trade_no = new JButton("Trade Response Decline"));
      p.add(q);

      add(p);

      trade_yes.addActionListener(this);
      trade_no.addActionListener(this);

		(new PlayClip("trade.wav")).start();

      setSize(400,200);
      setVisible(true);
   }

   synchronized public void actionPerformed (ActionEvent evt) {
      if (evt.getSource() == trade_yes) { val = 1; }
      else if (evt.getSource() == trade_no) { val = 2; }
      notify();
   }

   synchronized public int value () {
      try { wait(); } catch (Exception e) { }
      return val;
   }
}
