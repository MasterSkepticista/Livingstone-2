//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools.historyTable;

import javax.swing.*;
import java.awt.*;
import java.util.Hashtable;
import java.io.*;
import gov.nasa.arc.l2tools.util.EventLog;


/* Simple color database that maps strings to colors.  The strings are
   intended to represent attribute values, but you can probably also put
   attribute names and type strings in here, since they probably won't
   collide.  Later, we'll do a proper database.  <p>

   This persists the color database in the filesystem.
*/

public class ColorDB {

  /* Public for debugging purposes only */
  public static Hashtable colorTable = new Hashtable();
  static Color defaultColor = Color.white;

  public static void addColor(String value, Color color) {
    Color old = (Color)colorTable.get(value);
    if (color.equals(old)) return;
    colorTable.put(value,color);
    if (WriteThrough) writeToFile();
  }

  public static void removeColor(String value) {
    if (!colorTable.containsKey(value)) return;
    colorTable.remove(value);
    if (WriteThrough) writeToFile();
  }

  public static Color getColor(String value) {
    Color c = (Color)colorTable.get(value);
    if (c!=null) return c;
    return defaultColor;
  }

  /* Choose a color for the particular value.  Return true if the color
     changed, false if it didn't. */
  public static boolean chooseColorFor(Component c, String value) {
    Color initialColor = getColor(value);
    Color color = initialColor==null ? Color.white : initialColor;
    color = JColorChooser.showDialog(c,
				     "Choose background color for cells holding this value",
				     color);
    if (color==null) return false;
    if (color.equals(initialColor)) return false;
    if (color.equals(defaultColor)) {
      removeColor(value);
    } else {
      addColor(value,color);
    }
    return true;
  }

  //
  // Persistence methods
  //

  /* Write the database to the filesystem when its been modified */
  protected static boolean WriteThrough = true;

  /* Filename */
  protected static String filename = "SkunkworksColorDB.ser";

  /* Set the flag that determines whether the database is saved to the
     filesystem upon every modification. */
  public static void setWriteThrough(boolean s) {
    WriteThrough = s;
  }

  /* Set to null to inhibit writes */
  public static void setFilename(String file) {
    filename = file;
  }

  public static String getFilename() { return filename; }

  public static void writeToFile() {
    if (filename==null) return;
    try {
      FileOutputStream out = new FileOutputStream(filename);
      ObjectOutput s = new ObjectOutputStream(out);
      s.writeObject(colorTable);
      s.close();
    } catch (Exception e) {
      e.printStackTrace();
      return;
    }
  }

  public static void readFromFile(String filename) {
    try {
      FileInputStream in = new FileInputStream(filename);
      ObjectInput s = new ObjectInputStream(in);
      Object db = null;
      colorTable = (Hashtable)s.readObject();
      s.close();
    } catch (Exception e) {
      EventLog.report("Error reading History Table color db.");
      return;
    }
  }

  /* Initially load the database */
  static {
    File abstractFile = new File( filename);
    if ( abstractFile.exists() ) {
      readFromFile(filename);
    } else {
      EventLog.report("History Table color db not yet created by Mouse-R: Set value background color");
    }
  }

}
