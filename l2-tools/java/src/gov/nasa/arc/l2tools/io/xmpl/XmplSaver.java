//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.io.xmpl;

/**
 * Insert the type's description here.
 * Creation date: (3/20/2000 11:21:10 AM)
 * @author: Andrew Bachmann
 */
public class XmplSaver extends gov.nasa.arc.l2tools.io.Saver {
  /**
   * XmplSaver constructor comment.
   */
  public XmplSaver() {
    super();
  }
  /**
   * XmplSaver constructor comment.
   * @param writer gov.nasa.arc.l2tools.io.Writer
   */
  public XmplSaver(gov.nasa.arc.l2tools.io.Writer writer) {
    super(writer);
  }
  /**
   * XmplSaver constructor comment.
   * @param file java.io.File
   * @exception java.io.IOException The exception description.
   */
  public XmplSaver(java.io.File file) throws java.io.IOException {
    super(file);
  }
  /**
   * XmplSaver constructor comment.
   * @param fd java.io.FileDescriptor
   */
  public XmplSaver(java.io.FileDescriptor fd) {
    super(fd);
  }
  /**
   * XmplSaver constructor comment.
   * @param out java.io.OutputStream
   */
  public XmplSaver(java.io.OutputStream out) {
    super(out);
  }
  /**
   * XmplSaver constructor comment.
   * @param out java.io.OutputStream
   * @param enc java.lang.String
   * @exception java.io.UnsupportedEncodingException The exception description.
   */
  public XmplSaver(java.io.OutputStream out, String enc) throws java.io.UnsupportedEncodingException {
    super(out, enc);
  }
  /**
   * XmplSaver constructor comment.
   * @param name java.lang.String
   * @exception java.io.IOException The exception description.
   */
  public XmplSaver(String name) throws java.io.IOException {
    super(name);
  }
  /**
   * XmplSaver constructor comment.
   * @param url java.net.URL
   * @exception java.io.IOException The exception description.
   * @exception java.net.MalformedURLException The exception description.
   */
  public XmplSaver(java.net.URL url) throws java.io.IOException, java.net.MalformedURLException {
    super(url);
  }
  /**
   * save method comment.
   */
  public void save(final gov.nasa.arc.l2tools.DataObject object) throws java.io.IOException {
    class DummyXmplClass extends fr.dyade.koala.xml.domlight.XMLElement {
      public String getName() { return "xmpl"; }
      public int getChildCount() { return 3; }
      public fr.dyade.koala.xml.domlight.XMLNode getChild(int which) {
        class StartContent extends gov.nasa.arc.l2tools.Content {
          public String getContent() {
            return "\n";
          }
        }
        class EndContent extends gov.nasa.arc.l2tools.Content {
          public String getContent() {
            return "\n";
          }
        }
        switch (which) {
          case 0: return new StartContent();
          case 1: return object.getXmlElement();
          case 2: return new EndContent();
        }
        return null;
      }
    }
    gov.nasa.arc.l2tools.io.xml.XMLElementSerializer serializer
    = new gov.nasa.arc.l2tools.io.xml.XMLElementSerializer(getWriter());
    serializer.writeMetaData("<?xml version=\"1.0\"?>\n");
    fr.dyade.koala.xml.domlight.XMLElement xmpl = new DummyXmplClass();
    xmpl.addAttribute("xmlns:ci","http://www.w3.org/1999/XSL/Format");
    serializer.writeXMLElement(xmpl);
    serializer.flush();
  }
  /**
   * save method comment.
   */
  public void save(gov.nasa.arc.l2tools.Project project) throws java.io.IOException {
    save((gov.nasa.arc.l2tools.DataObject)project);
  }
}
