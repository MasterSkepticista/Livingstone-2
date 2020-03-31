//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : PopupMenuProvider.java
 *
 * 
 *
 * CHANGELOG:
 * 12/09/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

/** This object can provide operations for a popup menu. */
public interface PopupMenuProvider {

    /** Add items to this object's right click menu.  Component will be the
        presentation context.*/
    public void addPopupMenuItems(java.awt.Component c, java.util.Vector items);

}
