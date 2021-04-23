import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;

// Rules:
//  1. Encryption can not be started if it is off and Active Client is running
//  2. All commands except: RSA key, ZKP key, and delete key are disabled if
//     Active Client is not running
//  3. If WAITING is received and player is logged in all commands are active
//     but encryption cannot be changed (since Active Client and Passive Server
//     must be on as WAITING)
//     cannot be received otherwise
//  4. All commands except IDENT are off if player is not logged in and IDENT is
//     received
//  5. All commands except PASSWORD are off if player is not logged in and PASSWORD
//     is received
//  6. All commands except HOST_PORT are off if player is not logged in and HOST_PORT
//     is received
//  7. All commands expect ALIVE are off if player is not logged in ALIVE is received

public class State {
	Project project;
	
	public State(Project proj) { project = proj; }
	
	public void initiate () {
		allOff();
		project.command.ind[0].setBackground(Color.yellow);
		project.command.ind[14].setBackground(Color.yellow);
		project.command.ind[16].setBackground(Color.yellow);
	}
	
	public void login (String cmd) {
		project.command.ind[1].setBackground(Color.black);
		project.command.ind[2].setBackground(Color.black);
		project.command.ind[5].setBackground(Color.black);
		project.command.ind[10].setBackground(Color.black);
		if (cmd.equalsIgnoreCase("ALIVE")) {
			if (project.command.ind[0].getBackground() == Color.yellow)
				project.command.ind[0].setBackground(Color.black);
			project.command.ind[10].setBackground(Color.yellow);
		} else if (cmd.equalsIgnoreCase("IDENT")) {
			project.command.ind[1].setBackground(Color.yellow);
		} else if (cmd.equalsIgnoreCase("PASSWORD")) {
			if (project.command.ind[0].getBackground() == Color.yellow)
				project.command.ind[0].setBackground(Color.black);
			project.command.ind[2].setBackground(Color.yellow);
		} else if (cmd.equalsIgnoreCase("HOST_PORT")) {
			selectedOff();
			project.command.ind[5].setBackground(Color.yellow);
		} else if (cmd.equalsIgnoreCase("LOGGED")) {
			loggedInFunc();
		}
	}
	
	private void selectedOff () {
		for (int i=1 ; i < 40 ; i++)
			if (project.command.ind[i] != null && i != 12 && i != 14 && i != 16)
				project.command.ind[i].setBackground(Color.black);
	}
	
	private void loggedInFunc () {
		project.command.ind[3].setBackground(Color.yellow);
		project.command.ind[4].setBackground(Color.yellow);
		project.command.ind[5].setBackground(Color.yellow);
		project.command.ind[6].setBackground(Color.yellow);
		project.command.ind[7].setBackground(Color.yellow);
		project.command.ind[8].setBackground(Color.yellow);
		project.command.ind[9].setBackground(Color.yellow);
		for (int i=11 ; i < 40 ; i++) {
			if (project.command.ind[i] != null)
				project.command.ind[i].setBackground(Color.yellow);
		}
		deletePublicKey();
	}
    
	public void deletePublicKey () {
		if (!project.command.arg[7].getText().equals("") &&
			 !project.command.arg[8].getText().equals("")) {
			project.command.ind[12].setBackground(Color.yellow);
			project.command.ind[15].setBackground(project.command.ind[17].getBackground());
		} else {
			project.command.ind[12].setBackground(Color.black);
			project.command.ind[15].setBackground(Color.black);
		}
	}
	
	private void allOff () {
		for (int i=0 ; i < 40 ; i++)
			if (project.command.ind[i] != null)
				project.command.ind[i].setBackground(Color.black);
	}
	
	private void allOn () {
		for (int i=0 ; i < 40 ; i++)
			if (project.command.ind[i] != null)
				project.command.ind[i].setBackground(Color.yellow);
	}
	
	boolean canBeInvoked (int index) {
		if (index < 40) {
			if (project.command.ind[index].getBackground() == Color.black) {
				project.status.setText(KeepTwo.rows(project.command.cmd[index].getText()+
																" is disabled at the moment",
																project.status.getText()));
				(new PlayClip("error-1.wav")).start();
				return false;
			} 
		}
		return true;
	}
}

