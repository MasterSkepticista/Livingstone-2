//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : InputModeIdleOldStyle.java
 *

 * CHANGELOG:
 * 09/01/98      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.Frame;
import java.awt.Component;
import java.awt.event.*;

/** This mode breaks out the component and x,y instance variables of the event
 * and passes them in to the methods.  */
public class InputModeIdleOldStyle implements InputMode {
    // From MouseAdapter

    public void mousePressed(MouseEvent e) {
        mousePressed(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mousePressed(Component c, MouseEvent e, int x, int y) {}

    public void mouseReleased(MouseEvent e) {
        mouseReleased(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mouseReleased(Component c, MouseEvent e, int x, int y) {}

    public void mouseEntered(MouseEvent e) {
        mouseEntered(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mouseEntered(Component c, MouseEvent e, int x, int y) {}

    public void mouseExited(MouseEvent e) {
        mouseExited(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mouseExited(Component c, MouseEvent e, int x, int y) {}

    public void mouseClicked(MouseEvent e) {
        mouseClicked(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mouseClicked(Component c, MouseEvent e, int x, int y) {}

    public void mouseDragged(MouseEvent e) {
        mouseDragged(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mouseDragged(Component c, MouseEvent e, int x, int y) {}

    public void mouseMoved(MouseEvent e) {
        mouseMoved(e.getComponent(),e,e.getX(),e.getY());
    }
    public void mouseMoved(Component c, MouseEvent e, int x, int y) {}

    public void keyPressed(KeyEvent e) {
        keyPressed(e.getComponent(),e,e.getKeyCode());
    }
    public void keyPressed(Component c, KeyEvent e, int key) {}

    public void keyReleased(KeyEvent e) {
        keyReleased(e.getComponent(),e,e.getKeyCode());
    }
    public void keyReleased(Component c, KeyEvent e, int key) {}

    public void keyTyped(KeyEvent e) {
        keyTyped(e.getComponent(),e,e.getKeyCode());
    }
    public void keyTyped(Component c, KeyEvent e, int key) {}

    // These methods are called when the mode is first installed,
    // when another is pushed over it, when it's popped back to the
    // top of the mode stack, and when it is taken off the stack.


    public void mouseActivate(InputSurface s) {}

    public void mousePush(InputSurface s) {}

    public void mousePop(InputSurface s) {}

    public void mouseDeactivate(InputSurface s) {}

    // Return the kind of cursor
    public int mouseCursor() { return Frame.DEFAULT_CURSOR; }

    /* This won't work because processEvent() is a protected member. */
    public void delegateStroke(ComponentEvent e) throws Exception {
        Component c = e.getComponent();
        if (c instanceof JComponentWithInputMode) {
            JComponentWithInputMode c1 = (JComponentWithInputMode)c;
            c1.pushInputMode(this);
            c1.delegateEvent(e);
        } else {
            // This shouldn't be possible to reach
            throw new Exception("input stroke delegated to a component that does not handle input modes");
        }
    }
}

