// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * JmplWriter.java
 *
 * Created on July 16, 2002, 12:59 PM
 */

package oliver.xmpl;

/**
 *
 * @author  stephen
 */
public class JmplWriter {
    
    /**
     * Where to write the output to.
     */
    private java.io.PrintWriter writer = null;
    
    /**
     * Creates a new instance of JmplWriter.
     *
     * @param out where to write the output to.
     */
    public JmplWriter(java.io.OutputStream out) {
        this.writer = new java.io.PrintWriter(out);
    }
    
    /**
     * Cleans up the object when it's destroyed.
     */
    protected void finalize() throws Throwable {
        if(writer != null) {
            writer.flush();
            writer.close();
        }
        super.finalize();
    }
    
    
    /**
     * Writes an Xmpl element.
     *
     * @param xmpl the non-null Xmpl element to write.
     */
    public void write(Xmpl xmpl) throws java.io.IOException {
        write(xmpl, 0);
    }
    
    
    /**
     * Writes an Xmpl element.
     *
     * @param xmpl the non-null Xmpl element to write.
     * @param indent how many spaces to indent the element.
     */
    public void write(Xmpl xmpl, int indent) throws java.io.IOException {
        xmpl.writeJmpl(writer, true, indent);
    }
    
    public void flush() {
        writer.flush();
    }
    
    /**
     * Writes a string encoding reserved characters.
     *
     * @param str the string to write.
     *
   private void writeEncoded(String str)
   {
      for (int i = 0; i < str.length(); i++) {
         char c = str.charAt(i);
    
         switch (c) {
            case 0x0A:
               this.writer.print(c);
               break;
    
            case '<':
               this.writer.print("&lt;");
               break;
    
            case '>':
               this.writer.print("&gt;");
               break;
    
            case '&':
               this.writer.print("&amp;");
               break;
    
            case '\'':
               this.writer.print("&apos;");
               break;
    
            case '"':
               this.writer.print("&quot;");
               break;
    
            default:
               if ((c < ' ') || (c > 0x7E)) {
                  this.writer.print("&#x");
                  this.writer.print(Integer.toString(c, 16));
                  this.writer.print(';');
               } else {
                  this.writer.print(c);
               }
         }
      }
   }*/
    
}
