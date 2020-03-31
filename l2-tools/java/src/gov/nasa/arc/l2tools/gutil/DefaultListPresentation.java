//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : DefaultListPresentation.java
 *
 * Notes:
 * 
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

/** Default presentation for use in lists and trees, e.g., todo lists */

public class DefaultListPresentation extends PresentationBase {
    
    public DefaultListPresentation(Object object) {
        super(object);
    }

    /** This will usually call toString() on the object this is
        representing. */
    public String stringForPresentation() {
        if (object==null)
            return "No object";
        else
            return object.toString();
    }

    public void open() {
    }

}
