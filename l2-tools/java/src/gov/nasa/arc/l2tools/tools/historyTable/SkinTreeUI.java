//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Class.java
 *
 * Created on February 21, 2003, 3:48 PM
 */

package gov.nasa.arc.l2tools.tools.historyTable;

import gov.nasa.arc.l2tools.Exception;
import javax.swing.plaf.basic.*;
import javax.swing.*;
import java.awt.*;
import javax.swing.plaf.*;
import java.io.*;

public class SkinTreeUI extends BasicTreeUI {

  /**
   * Description of the Field
   */
  protected final static int HALF_SIZE = 4;
  /**
   * Description of the Field 
   */
  protected final static int SIZE = 15;
  
  protected void paintVerticalLine(Graphics g, JComponent c, int x, int top, int bottom) {
    
    Color oldColor = g.getColor();
    g.setColor(Color.black);
    super.paintVerticalLine(g, c, x, top, bottom);
    g.setColor(oldColor);
  }

  /**
   * Description of the Method
   *
   * @param g      Description of Parameter
   * @param c      Description of Parameter
   * @param y      Description of Parameter
   * @param left   Description of Parameter
   * @param right  Description of Parameter
   */
  protected void paintHorizontalLine(Graphics g, JComponent c, int y, int left, int right) {
    Color oldColor = g.getColor();
    g.setColor(Color.black);
    super.paintHorizontalLine(g, c, y, left, right);
    g.setColor(oldColor);
    
  }
 
  /**
   * Description of the Method
   *
   * @param c  Description of Parameter
   * @return   Description of the Returned Value
   */
  public static ComponentUI createUI(JComponent c) {
    return new SkinTreeUI();
  }


    
  public SkinTreeUI() {super();}
  
  
  protected void installDefaults(){
    super.installDefaults();
    setExpandedIcon((Icon)UIManager.get("MyJTree.expandedIcon"));  
    setCollapsedIcon((Icon)UIManager.get("MyJTree.collapsedIcon"));  
  }
  
    public static class ExpandedIcon extends Component implements Icon, Serializable {
    /**
     * Gets the IconWidth attribute of the ExpandedIcon object
     *
     * @return   The IconWidth value
     *
     */
     private int width =0;
     private int height =0;
     protected static ImageIcon collapsedIcon = null;
     protected static ImageIcon expandedIcon = null;
        
     public ExpandedIcon() throws Exception{
         
        ClassLoader cl = this.getClass().getClassLoader();
	  
        if(cl == null)  return;
	    
        java.net.URL url = cl.getResource("gov.nasa.arc.l2tools.icons/collapsed.gif");
	if(url != null){collapsedIcon = new ImageIcon(url);}
        else throw new Exception("Cannot load collapsed icon image"); 
          
        url = cl.getResource("gov.nasa.arc.l2tools.icons/expanded.gif");
	if(url != null){expandedIcon = new ImageIcon(url);}
        else throw new Exception("Cannot load expanded icon image"); 
        width = expandedIcon.getIconWidth();
        height =expandedIcon.getIconHeight();
      }
     
     
 
     
     public int getIconWidth() {
        return width;
     }

    /**
     * Gets the IconHeight attribute of the ExpandedIcon object
     *
     * @return   The IconHeight value
     */
    public int getIconHeight() {
      return height;
    }

    /**
     * Description of the Method
     *
     * @param c  Description of Parameter
     * @param g  Description of Parameter
     * @param x  Description of Parameter
     * @param y  Description of Parameter
     */
    public void paintIcon(Component c, Graphics g, int x, int y) {

        boolean loaded = g.drawImage(expandedIcon.getImage(), x, y, width-1, height-1,null);
    }
    
    

    public static Icon createExpandedIcon() throws Exception{
        return new ExpandedIcon();
    }
  }

  
  public static class CollapsedIcon extends ExpandedIcon {

    /**
     * Description of the Method
     *
     * @param c  Description of Parameter
     * @param g  Description of Parameter
     * @param x  Description of Parameter
     * @param y  Description of Parameter
     */
      
    private int width; 
    private int height; 
    
    public CollapsedIcon() throws Exception{
        width = collapsedIcon.getIconWidth();
        height = collapsedIcon.getIconHeight();
        
    }
    
    public void paintIcon(Component c, Graphics g, int x, int y) {
      
        boolean loaded = g.drawImage(collapsedIcon.getImage(), x, y, SIZE-1, SIZE-1,null);
     
    }

    /**
     * Description of the Method
     *
     * @return   Description of the Returned Value
     */
    public static Icon createCollapsedIcon() throws Exception{
      return new CollapsedIcon();
    }
  }

 
}
  

