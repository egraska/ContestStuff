import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;

/**
 * The Bank class monitors raw resources (glass, oil, steel, etc.) and
 * complex resources (weapons, computers, vehicles) for the I-Wars
 * player.
 *
 *@author Greg Larson
 *@version 
 */
public class Bank {
   public int computers, weapons, vehicles;
   public int oil, steel, plastic, copper, rubber, glass, rupyulars;
   public DefaultListModel <String> resourceVec = null;
   public String[] resourceStrings;
   
   public Bank() {
      /*** delete JF
           computers = 0;
           weapons = 0;
           vehicles = 0;
           oil = 0;
           steel = 0;
           plastic = 0;
           copper = 0;
           rubber = 0;
           glass = 0;
      **/

      /***  add JF ***/
      computers = 10;
      weapons = 2;
      vehicles = 5;
      oil = 100;
      steel = 34;
      plastic = 56;
      copper = 124;
      rubber = 67;
      glass = 23;
      /*** ***/

      resourceStrings = new String[Globals.NUM_RESOURCES];
      resourceStrings[Globals.COMPUTERS] = "computers";
      resourceStrings[Globals.WEAPONS] = "weapons";
      resourceStrings[Globals.VEHICLES] = "vehicles";
      resourceStrings[Globals.OIL] = "oil";
      resourceStrings[Globals.STEEL] = "steel";
      resourceStrings[Globals.PLASTIC] = "plastic";
      resourceStrings[Globals.COPPER] = "copper";
      resourceStrings[Globals.RUBBER] = "rubber";
      resourceStrings[Globals.GLASS] = "glass";
      resourceStrings[Globals.RUPYULARS] = "rupyulars";
      resourceVec = new DefaultListModel <String> ();
      resourceVec.setSize(20);
   }

   public void initialize () {
      setValue(Globals.COMPUTERS, 10);
      setValue(Globals.WEAPONS, 2);
      setValue(Globals.VEHICLES, 5);
      setValue(Globals.OIL, 100);
      setValue(Globals.STEEL, 34);
      setValue(Globals.PLASTIC, 56);
      setValue(Globals.COPPER, 124);
      setValue(Globals.RUBBER, 67);
      setValue(Globals.GLASS, 23);
   }
   
   public DefaultListModel getResourceVec() {
      if (resourceVec != null) resourceVec.clear();
      if (resourceVec == null || resourceVec.size() == 0) {
         resourceVec.addElement(report(Globals.COMPUTERS));
         resourceVec.addElement(report(Globals.WEAPONS));
         resourceVec.addElement(report(Globals.VEHICLES));
         resourceVec.addElement(report(Globals.OIL));
         resourceVec.addElement(report(Globals.STEEL));
         resourceVec.addElement(report(Globals.PLASTIC));
         resourceVec.addElement(report(Globals.COPPER));
         resourceVec.addElement(report(Globals.RUBBER));
         resourceVec.addElement(report(Globals.GLASS));
         resourceVec.addElement(report(Globals.RUPYULARS));
      }
      return resourceVec;
   }

   public String report(int resource) {
      boolean debug = false;

      Class cls;
      Field[] fields;
      int value = 0;
      boolean foundMatch = false;
      int i = 0;
      try {
         cls = Class.forName("Bank");
         fields = cls.getDeclaredFields();
         while (!foundMatch && i < fields.length) {
            if (debug)
					System.out.println("matching: "+fields[i].getName()+" "+
											 resourceStrings[resource]);
            if (fields[i].getName().equals(resourceStrings[resource])) {
               if (debug)
						System.out.println("\tvalue="+fields[i].getInt(this));
               value = fields[i].getInt(this);
               foundMatch = true;
            }
            i++;
         } // end while
      } catch(Exception e) {
         System.err.println("Error: class=Bank method=report");
      }
      if (foundMatch) {
         if (debug)
				System.out.println("foundMatch res="+resource+" value="+value);
         return insertDotLeader(resourceStrings[resource]) + value;
      } else 
         return null;
   } // end method report

   public void setValue(int resource, int val) {
      Class cls;
      Field[] fields;
      boolean foundMatch = false;
      int i = 0;
      try {
         cls = Class.forName("Bank");
         fields = cls.getDeclaredFields();
         while (!foundMatch && i < fields.length) {
            if (fields[i].getName().equals(resourceStrings[resource])) {
               fields[i].setInt(this, val);
               resourceVec.set(resource, report(resource));
               foundMatch = true;
            }
            i++;
         } // end while
      } catch(Exception e) {
         System.err.println("Error: class=Bank method=setValue");
      }   
   } // end method setValue

   public void setValue(String resource, int val) {
      for (int i = 0; i < resourceStrings.length; ++i) {
         if (resource.toLowerCase().equals(resourceStrings[i])) {
            setValue(i, val);
            return;
         }
      } // end for
   } // end method setValue

   public String insertDotLeader(String str) {
      StringBuffer buffer = new StringBuffer(str);
      for (int i = buffer.length(); i < 28; ++i) {
         buffer.append(".");
      }
      return buffer.toString();
   } // end method insertDotLeader
   
} // end class Bank
