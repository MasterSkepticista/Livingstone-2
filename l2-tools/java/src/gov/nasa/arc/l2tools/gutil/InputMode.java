//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : InputMode.java
 *
 * Adds mouse operations to the keyboard mode.  I defined my own interfaces
 *
 * Notes:
 * - I believe the mouseCursor() calls aren't used yet.  Wait for 1.1
 * - I removed delegateEvent() and friends because it was becomming a hassle.
 *   It would be better to have an abstraction for doing the delegation.
 *   I've got to remember to always push the mode when delegating now.
 *
 * CHANGELOG:
 * 07/17/97      Mhs     Created
 * 07/25/97      Mhs     Changed MouseMode -> InputMode
 * 08/31/98      Mhs     Converting to 1.1
 * 10/30/99      Mhs     Using for Skunk Works
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.Component;
import java.awt.Event;
import java.awt.event.*;
import java.io.Serializable;

/** This adds mouse operations to the KeyListener interface. This is the one
 * to use normally, because it collects all mouse and keyboard handling
 * together.  */
public interface InputMode extends MouseListener, MouseMotionListener, KeyListener, Serializable {

    // These methods are called when the mode is first installed,
    // when another is pushed over it, when it's popped back to the
    // top of the mode stack, and when it is taken off the stack.
    public void mouseActivate(InputSurface s);

    public void mousePush(InputSurface s);

    public void mousePop(InputSurface s);

    public void mouseDeactivate(InputSurface s);

    // Return the kind of cursor
    public int mouseCursor();

    // Important inherited methods
    // public void mousePressed(MouseEvent e);
    // public void mouseReleased(MouseEvent e);
    // public void mouseEntered(MouseEvent e);
    // public void mouseExited(MouseEvent e);
    // public void mouseClicked(MouseEvent e);
    // public void mouseDragged(MouseEvent e);
    // public void mouseMoved(MouseEvent e);
    // public void keyPressed(KeyEvent e);
    // public void keyReleased(KeyEvent e);
    // public void keyAction(KeyEvent e);

    // Handle this event using this mode
    public void delegateStroke(ComponentEvent e) throws Exception;

}

