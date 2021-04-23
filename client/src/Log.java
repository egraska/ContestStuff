import javax.swing.*;
import java.util.*;
import java.awt.*;

public class Log extends JFrame {
	JTextArea log = new JTextArea(24,60);
	Project project;
	String title;

	public Log (Project prg, String t) {
		this.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		project = prg;
		title = t;
		setLayout(new BorderLayout());
		add("Center", new JScrollPane(log));
		add("North", new JLabel(title,JLabel.CENTER));
	}

	public void append (String str) {
		log.append(str);
		int pos = log.getDocument().getLength();
		log.setCaretPosition(pos);
		if (title.equals("Client Log")) {
			if (str.startsWith("[MON] RESULT:") ||
				 str.startsWith("[MON] REQUIRE:")) {
				String toAdd = str.substring(6,str.length()-1);
				String current = project.status.getText();
				project.status.setText(KeepTwo.rows(toAdd, current));
			}
		}
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

class KeepTwo {
	public static String rows (String added, String old) {
		String out = null;
		int len = Math.min(80,added.length());
		if (added.length() > 80) out = added.substring(0,len)+"...";
		else out = added.substring(0,len);
		if (old == null) return out;
		StringTokenizer t = new StringTokenizer(old, "\n");
		if (t.hasMoreTokens()) return out+"\n"+t.nextToken();
		return out;
	}
}

