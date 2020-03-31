//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : JComponentWithMouseMode.java
*
* A component that handles mouse actions through a mode class instance.
*
* CHANGELOG:
* 10/30/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

import javax.swing.JComponent;
import java.util.Stack;
import java.awt.event.ComponentEvent;

/**
 * Extension of JComponent with Shirley's input mode handling. 
 */
public class JComponentWithInputMode extends JComponent implements InputSurface {

    // The top of the mode stack is also kept as a listener
    protected Stack modeStack = new Stack();

    // Setting this to true prints messages about mode changes to System.out 
    private boolean inputModeDebug = true;

    public JComponentWithInputMode() {
        super();
        setInputMode(defaultInputMode());
    }

    protected InputMode defaultInputMode() {
        return new InputModeIdle();
    }

    //
    // Mouse Handlers
    //  

    public void setInputMode(InputMode m) {
        if (inputModeDebug) System.out.println("setting input mode...");
        popInputMode();
        pushInputMode(m);
    }

    public void pushInputMode(InputMode m) {
        if (inputModeDebug) System.out.println("Push input mode "+m);
        if (!modeStack.empty()) {
            InputMode current = ((InputMode)modeStack.peek());
            current.mouseDeactivate(this);
            removeKeyListener(current);
            removeMouseListener(current);
            removeMouseMotionListener(current);
        }	  
        modeStack.push(m);
        addKeyListener(m);
        addMouseListener(m);
        addMouseMotionListener(m);
        m.mouseActivate(this);
    }

    public InputMode popInputMode() {
        if (modeStack.empty())
            return null;
        InputMode old = (InputMode)modeStack.pop();
        if (inputModeDebug) System.out.println("Pop input mode "+old);
        old.mouseDeactivate(this);
        removeKeyListener(old);
        removeMouseListener(old);
        removeMouseMotionListener(old);
        if (!modeStack.empty()) {
            InputMode next = ((InputMode)modeStack.peek());
            addKeyListener(next);
            addMouseListener(next);
            addMouseMotionListener(next);
            next.mouseActivate(this);
        }
        if (inputModeDebug) System.out.println("  mode is now "+getInputMode());
        return old;
    }

    public InputMode getInputMode() {
        if (modeStack.empty()) {
            InputMode m = new InputModeIdle();
            pushInputMode(m);
            return m;
        };
        return (InputMode)modeStack.peek();
    }

    //
    // Delegation support
    //

    /* This makes it possible for another piece of code to push events into
       this component.  We use this to support input mode delegation. */
    public void delegateEvent(ComponentEvent e) {
        processEvent(e);
    }

    // Properties
    void setInputModeDebug(boolean v) { inputModeDebug = v; }
    boolean getInputModeDebug() { return inputModeDebug; }

}
