//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.xml;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 11:04:28 AM)
 * @author: Andrew Bachmann
 */
public class XMLElementDeserializer extends fr.dyade.koala.xml.domlight.XMLElementDeserializer {
/**
 * XMLElementDeserializer constructor comment.
 * @param istream java.io.InputStream
 */
public XMLElementDeserializer(java.io.InputStream istream) {
	super(istream);
}
/**
 * XMLElementDeserializer constructor comment.
 * @param istream java.io.InputStream
 * @param factory fr.dyade.koala.xml.domlight.XMLElementFactory
 */
public XMLElementDeserializer(java.io.InputStream istream, fr.dyade.koala.xml.domlight.XMLElementFactory factory) {
	super(istream, factory);
}
/**
 * XMLElementDeserializer constructor comment.
 * @param istream java.io.InputStream
 */
public XMLElementDeserializer(java.io.Reader reader) {
	this(new gov.nasa.arc.l2tools.io.ReaderInputStream(reader));
}
/**
 * XMLElementDeserializer constructor comment.
 * @param istream java.io.Reader
 * @param factory fr.dyade.koala.xml.domlight.XMLElementFactory
 */
public XMLElementDeserializer(java.io.Reader reader, fr.dyade.koala.xml.domlight.XMLElementFactory factory) {
	this(new gov.nasa.arc.l2tools.io.ReaderInputStream(reader), factory);
}
}
