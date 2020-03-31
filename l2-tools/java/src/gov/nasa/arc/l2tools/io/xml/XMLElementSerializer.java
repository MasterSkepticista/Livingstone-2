//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.xml;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 11:30:14 AM)
 * @author: Andrew Bachmann
 */
public class XMLElementSerializer extends fr.dyade.koala.xml.domlight.XMLElementSerializer {
/**
 * XMLElementSerializer constructor comment.
 * @param ostream java.io.OutputStream
 */
public XMLElementSerializer(java.io.OutputStream ostream) {
	super(ostream);
}
/**
 *
 * Creation date: (3/20/2000 11:30:35 AM)
 * @author: Andrew Bachmann
 * 
 * @param writer java.io.Writer
 */
public XMLElementSerializer(java.io.Writer writer) {
	this(new gov.nasa.arc.l2tools.io.WriterOutputStream(writer));
}
}
