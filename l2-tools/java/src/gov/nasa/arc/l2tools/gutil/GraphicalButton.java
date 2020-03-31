//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : From 1001 Java Programmer's Tips
* Description : image button with two states
*
* CHANGELOG:
* 03/04/99      Mhs     Created
*
*/

package gov.nasa.arc.l2tools.gutil;

import java.awt.*;
import java.awt.image.*;

public class GraphicalButton extends Canvas implements ImageObserver {

  Image  image_up;
  Image  image_down;
  String  name;
  static boolean DOWN = false;
  static boolean UP = true;
  boolean is_up = true;

  public GraphicalButton(String n, Image up, Image down)
  {
    image_up = up;
    image_down = down;
    name = n;
  }
 
  public Dimension getMinimumSize()
  {
    return(new Dimension(image_up.getWidth(this),
                         image_up.getHeight(this)));
  }

  public Dimension getPreferredSize()
  {
    return(getMinimumSize());
  }

  public boolean mouseDown(Event evt, int x, int y)
  {  
    is_up = GraphicalButton.DOWN;
    repaint();
    return(true);
  }

  public boolean mouseUp(Event evt, int x, int y)
  {
    if (contains(x,y))
      {
        deliverEvent(new Event(this, evt.when,
                               Event.ACTION_EVENT, x, y, evt.key, 
                               evt.modifiers, name));
      } 
    else
      {
        is_up = GraphicalButton.UP;
        repaint();
        System.out.println(name + " Abort");
      }

    return(true);
  }

  public void setButton(boolean state)
  {
    is_up = state;
    repaint();
  }

  public void update(Graphics g)
  {
    paint(g);
  }

  public void paint(Graphics g)
  {
    if (is_up)
      g.drawImage(image_up, 0, 0, this);
    else
      g.drawImage(image_down, 0, 0, this);
  }
}

