//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
 * Author : Mark Shirley
 * Description : ResizableSurfacePresentation.java
 *
 * CHANGELOG:
 * 11/27/99      Mhs     Created
 */

package gov.nasa.arc.l2tools.gutil;

public interface ResizableSurfacePresentation extends SurfacePresentation {

    /** Modify the current bounds */
    public void setBounds(java.awt.Rectangle r);

}
