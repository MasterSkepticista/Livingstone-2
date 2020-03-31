/*
 * (swing1.1beta3)
 * 
 */

package gov.nasa.arc.l2tools.gui.table.multispancell;

import java.awt.*;


/**
 * @version 1.0 11/22/98
 */

public interface CellFont {
  
  public Font getFont(int row, int column);
  public void setFont(Font font, int row, int column);
  public void setFont(Font font, int[] rows, int[] columns);


}
