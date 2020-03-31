//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : InputModeIdle.java
 *
 * Adds mouse operations to the keyboard mode.  I defined my own interfaces
 *
 * Notes:
 * - I believe the mouseCursor() calls aren't used yet.  Wait for 1.1
 *
 * CHANGELOG:
 * 09/01/98      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import gov.nasa.arc.l2tools.util.EventLog;
import java.awt.Component;
import java.awt.Frame;
import java.awt.event.*;

/**
 * A do-nothing input mode typically used as a superclass. 
 */
public class InputModeIdle extends MouseAdapter implements InputMode {
    // From MouseAdapter
    // public boolean mousePressed(MouseEvent e);
    // public boolean mouseReleased(MouseEvent e);
    // public boolean mouseEntered(MouseEvent e);
    // public boolean mouseExited(MouseEvent e);
    // public boolean mouseClicked(MouseEvent e);
    public void mouseDragged(MouseEvent e) {}

    public void mouseMoved(MouseEvent e) {}

    public void keyPressed(KeyEvent e) {};

    public void keyReleased(KeyEvent e) {};

    public void keyTyped(KeyEvent e) {};

    // These methods are called when the mode is first installed,
    // when another is pushed over it, when it's popped back to the
    // top of the mode stack, and when it is taken off the stack.
    public void mouseActivate(InputSurface s) {}

    public void mousePush(InputSurface s) {}

    public void mousePop(InputSurface s) {}

    public void mouseDeactivate(InputSurface s) {}

    // Return the kind of cursor
    public int mouseCursor() { return Frame.DEFAULT_CURSOR; }

    public void delegateStroke(ComponentEvent e) {
        Component c = e.getComponent();
        if (c instanceof JComponentWithInputMode) {
            ((JComponentWithInputMode)c).pushInputMode(this);
            c.dispatchEvent(e);
        } else {
            // This shouldn't be possible to reach
            EventLog.report(new Exception("input stroke delegated to a component that does not handle input modes"));
        }
    }
}

