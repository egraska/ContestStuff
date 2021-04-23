import java.net.*;
import java.util.*;
import java.lang.*;
import javax.swing.*;
import java.io.*;
import java.awt.*;
import java.math.*;
import java.security.*;

public class Server implements Runnable{

   ServerSocket srvSocket = null;
   Socket incoming = null;
   Thread runner;
   String ident;
   Project parent;
   boolean going;
   ConnectionHandler conhan = null;

   public Server(Project gameFrame) {
		int port;
      going = true;
      parent = gameFrame;
      ident = parent.identity.getText();
		try {
			port = Integer.parseInt(parent.srvport.getText());
		} catch (Exception e) {
			Info info = new Info(" Number Format Exception:",
										"Server host port field is not a number");
			parent.status.setText(KeepTwo.rows("Server host port is not a number - please edit",
														  parent.status.getText()));
			return;
		}
      try {
         srvSocket =
            new ServerSocket(Integer.parseInt(parent.srvport.getText()));
      } catch (IOException e) {
			parent.server_log.append("ERROR: ServerSocket failure! class=Server "+
											 "method=constructor\n");
      }
   }
   
   public void start() {
      if (runner == null) {
         runner = new Thread(this);
         runner.start();
      } 
   }

   public void stop () {
      going = false;
      if (conhan != null) conhan.stop();
      try {
         if (incoming != null) incoming.close();
         if (srvSocket != null) srvSocket.close();
      } catch (Exception e) {
			parent.server_log.append("Sockets closed");
      }
      // runner.stop();          <------------- this may have to be uncommented
   }

   public void run() {
      try {
         for (int i=0 ; going ; ++i) {
            parent.server_log.append("[srv] listening on port " + 
												 srvSocket.getLocalPort() + "\n");
            incoming =  srvSocket.accept();
            parent.server_log.append("[srv] starting new ConnectionHandler...\n");
            
            //Spawn a new thread for each new connection
            (conhan = new ConnectionHandler(parent,incoming,i,ident)).start(); 
         } 
      } catch (Exception e) {
			parent.server_log.append("Server [run]: Error in Server: "  + e + "\n");
			Info info = new Info(" Server Socket Failure:",
										"Server location is not valid - please edit the server "+
										"location field");
			parent.status.setText(KeepTwo.rows("Server location is not valie - please "+
														  "edit the server location field",
														  parent.status.getText()));
			return;
      }
   }
}

