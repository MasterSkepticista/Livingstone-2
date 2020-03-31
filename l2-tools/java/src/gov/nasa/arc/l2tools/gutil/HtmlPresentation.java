//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : HtmlPresentation.java
 *
 * CHANGELOG:
 * 11/29/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

/** Write as an HTML stream. */
public interface HtmlPresentation extends Presentation {

    public void write(HtmlWriter writer) throws java.io.IOException;

}
