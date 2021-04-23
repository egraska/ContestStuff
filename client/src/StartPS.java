public class StartPS implements ActOnButton {
   Project gui;

   public StartPS (Project gui) { this.gui = gui; }

   public boolean doit () {
      if (((String)gui.identity.getText()).equals("")) {
         gui.status.setText(KeepTwo.rows("User name missing",
													  gui.status.getText()));
         return false;
      } else if (gui.srvport.getText().equals("")) {
         gui.status.setText(KeepTwo.rows("Missing host port",
													  gui.status.getText()));
         return false; 
      }
      gui.srv = new Server(gui);
      gui.srv.start();
      return true;
   }
}

