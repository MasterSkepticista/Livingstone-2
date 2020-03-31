//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : Presentation.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

import java.awt.datatransfer.DataFlavor;

/** A presentation is a GUI element that represents another object.  It is
    usually a *very* lightweight thing, although it can also be a subclass of
    JComponent. */
public interface Presentation extends PopupMenuProvider {

    /** Return the object this presentation represents. */
    public Object getObject();

    /** Return a short string to present this object in a wholine */
    public String stringForWholine();

    /** DataFlavor for drag-and-drop. */
    public static final DataFlavor dataFlavor = new DataFlavor(Presentation.class,"Presentation");

}
