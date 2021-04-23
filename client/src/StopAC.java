public class StopAC implements ActOnButton {
   Project gui;
   public StopAC (Project gui) {  this.gui = gui;  }
   public boolean doit() { gui.ac.stop(); return true;  }
}
