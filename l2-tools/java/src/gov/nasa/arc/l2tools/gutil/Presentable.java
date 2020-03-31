//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : Presentable.java
*
* CHANGELOG:
* 11/01/99      Mhs     Created
* 11/02/99      Mhs     Eliminated HtmlViewable.  Combined with Presentable.
* 01/02/99      Mhs     Made Serializable.  This was necessary to make them draggable.
*/

package gov.nasa.arc.l2tools.gutil;

import javax.swing.JPopupMenu;
import java.awt.datatransfer.DataFlavor;
import java.io.Serializable;

/**
 * We're using the model/view pattern heavily in skunkworks.  This
 * interface describes the protocol for a "model" object to return
 * different kinds of views.  We're calling them presentations following
 * lisp machine terminology.
 * 
 * There are three kinds of views currently supported by this interface:
 * 1. A skunkworks-specific view object that implements the
 *    Presentation interface.  These objects are used in views like
 *    lattices.
 * 2. An HTML view generated via calls to a Writer.
 * 3. A short string view used for the wholine and perhaps for
 *    printers. 
 */
public interface Presentable extends PopupMenuProvider, Serializable {

    /** Return a view object which stands for this in a skunkworks GUI. */
    public Presentation getPresentation(Class context);

    /** Return a short string to present this object in a wholine */
    public String stringForWholine();

    /** Return a string referencing an icon to use to represent this object in
        SurfacePresentation contexts.  Return null if you want to use the
        default icon.  For now, these are supposed to be filenames.  Maybe
        we'll change them to URL's later. */
    public String sourceForIcon();

    /** This object can write a description of itself in html. */
    public void write(HtmlWriter writer) throws java.io.IOException;

    /** DataFlavor for drag-and-drop. */
    public static final DataFlavor dataFlavor = new DataFlavor(Presentable.class,"Presentable");

}
