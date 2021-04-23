public class StartAC implements ActOnButton {
   Project gui;

   public StartAC (Project gui) {  this.gui = gui; }
   
   public boolean doit() {
      if (((String)gui.identity.getText()).equals("")) {
			gui.status.setText(KeepTwo.rows("User name missing",
													  gui.status.getText()));
         return false;
      } else if (gui.srvport.getText().equals("")) {
         gui.status.setText(KeepTwo.rows("Host port missing",
													  gui.status.getText()));
         return false;
      }
		gui.command.arg[0].setText(gui.identity.getText());
		gui.command.arg[2].setText(gui.passwd.getText());
		gui.ac = new ActiveClient(gui);
		gui.ac.setIdent(gui.identity.getText());
		gui.ac.start();
      return true;
   }
}

