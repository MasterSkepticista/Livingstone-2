//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : PopupMenuCallback.java
*
* CHANGELOG:
* 12/02/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

/** Part of the PopupMenu convenience class.  */
public interface PopupMenuCallback {
    /** Object will be the presentation's context. */
    public void callback(java.awt.Component context);
}
