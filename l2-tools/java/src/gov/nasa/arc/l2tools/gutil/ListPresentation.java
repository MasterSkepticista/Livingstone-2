//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : ListPresentation.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

/** Presentation for use in lists and trees, e.g., todo lists */
public interface ListPresentation extends Presentation {

    /** This will usually call toString() on the object this is
        representing. */
    public String stringForPresentation();

}
