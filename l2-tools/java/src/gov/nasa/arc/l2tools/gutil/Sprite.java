//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
// Sprite.java

package gov.nasa.arc.l2tools.gutil;

import java.awt.*;

public abstract interface Sprite {

  public abstract void paint(Graphics g);
  public abstract void markDirtyRects(SpriteCanvas canvas);

}
