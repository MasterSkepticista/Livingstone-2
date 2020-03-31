//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : InputModeMouseWholine.java
 *
 * Simple wholine.
 *
 * Notes:
 * - I believe the mouseCursor() calls aren't used yet.  Wait for 1.1
 *
 * CHANGELOG:
 * 09/01/98      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.event.*;
import java.awt.Label;

/**
 * This provides default handling for mouseMoved that displays the Presentation under the mouse via the wholine.  The way this
 * interacts with the GraphicSurface needs to be reworked.
 */
public class InputModeMouseWholine extends InputModeIdle {
    Label target;
    Object lastObject;

    public InputModeMouseWholine(Label l) {
        target = l;
    }

    public void mouseMoved(MouseEvent e) {
        if (target == null) return;
        GraphicSurface s = (GraphicSurface)(e.getComponent());
        Object o = s.pick(e.getX(), e.getY());
        //if (o==lastObject) return;
        if (o == lastObject) {
            //System.out.println("same as last time:" +target.getText());
            return;
        }
        if (o == null) {
            target.setText("");
        } else {
            target.setText(o.toString());
        }
        lastObject = o;
        //System.out.println(o);
        //target.repaint(400);
    }
}

