public class StopPS implements ActOnButton {
   Project gui;
   public StopPS (Project gui) {  this.gui = gui;  }
   public boolean doit() {  gui.srv.stop();  return true;  }
}

