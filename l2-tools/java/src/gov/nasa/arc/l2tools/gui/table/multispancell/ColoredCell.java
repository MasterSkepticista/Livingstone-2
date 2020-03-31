/*
 * (swing1.1beta3)
 * 
 */

package gov.nasa.arc.l2tools.gui.table.multispancell;

import java.awt.*;


/**
 * @version 1.0 11/22/98
 */

public interface ColoredCell {
  
  public Color getForeground(int row, int column);
  public void setForeground(Color color, int row, int column);
  public void setForeground(Color color, int[] rows, int[] columns);

  public Color getBackground(int row, int column);
  public void setBackground(Color color, int row, int column);
  public void setBackground(Color color, int[] rows, int[] columns);


}
