//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : PopupMenuItem.java
*
* CHANGELOG:
* 12/02/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.gutil;

public class PopupMenuItem {
    String label;
    PopupMenuCallback callback;

    // Needed by PopupMenuItemSeparator
    public PopupMenuItem() {
        super();
    }

    public PopupMenuItem(String label, PopupMenuCallback callback) {
        super();
        this.label = label;
        this.callback = callback;
    }
}
