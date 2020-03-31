//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : InputSurface.java
 *
 * An object that has input modes
 *
 * CHANGELOG:
 * 08/31/98      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.Cursor;

/** This interface describes objects that have input mode handling
 * capabilities.  */
public interface InputSurface {
    public void setInputMode(InputMode m);

    public void pushInputMode(InputMode m);

    public InputMode popInputMode();

    public InputMode getInputMode();

    public void setCursor(Cursor c);
}

