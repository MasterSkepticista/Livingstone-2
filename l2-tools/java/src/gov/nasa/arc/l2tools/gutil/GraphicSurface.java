//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : GraphicSurface.java
 *
 * An interface describing drawing surfaces that InputMode's can
 * interact with.
 *
 * CHANGELOG:
 * 08/31/98      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.*;
import java.util.Vector;

/** An interface describing drawing surfaces that InputMode's can interact
    with.  Unused. */
public interface GraphicSurface extends InputSurface {

  public GraphicElement pick(int x, int y);
  //public Vector pickAll(Rectangle r);

  //public Vector getSelection ();
  //public void setSelection(Vector elts);
  //public void clearSelection();  
  //public Graphics getGraphics();

	public GraphicElement getSelection();
	public void setSelection(GraphicElement e);
	public void clearSelection();
	
	public void repaint();

}
