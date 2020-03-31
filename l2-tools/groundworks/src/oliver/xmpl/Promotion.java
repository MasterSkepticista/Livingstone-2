// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Promotion extends Xmpl {
        
    public static int UNKNOWN = 0;
    public static int ATTRIBUTE = 1;
    public static int RELATION = 2;
    public static int COMPONENT = 3;
    
    public Promotion() {
        super();
        setName("gr:promotion");
    }
    
    public Promotion(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    /** Getter for property instanceName.
     * @return Value of property instanceName.
     */
    public String getInstanceName() {
        return getAttribute("name","######");
    }
    
    /** Setter for property instanceName.
     * @param name New value of property instanceName.
     */
    public void setInstanceName(String instanceName) {
        setAttribute("name", instanceName);
    }

    /** Getter for property angle.
     * @return Value of property angle.
     */
    public int getAngle() {
        return Integer.parseInt(getAttribute("angle","0"));
    }
    
    /** Setter for property angle.
     * @param angle New value of property angle.
     */
    public void setAngle(int angle) {
        setAttribute("angle", String.valueOf(angle));
    }
    
    /** Getter for property type.
     * @return Value of property type.
     */
    public int getType() {
        return Integer.parseInt(getAttribute("type","0"));
    }
    
    /** Setter for property type.
     * @param type New value of property type.
     */
    public void setType(int type) {
        setAttribute("type", String.valueOf(type));
    }

    
}
