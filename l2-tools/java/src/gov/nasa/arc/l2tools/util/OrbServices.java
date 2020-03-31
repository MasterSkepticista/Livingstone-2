//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : OrbServices.java
*
* Wrapper that holds onto the ORB instance (making it globally accessible).
*
* CHANGELOG:
* 11/24/99      Mhs     Created
* 11/24/99      Mhs     Modified to explicitly reference the port of the nameserver
*/

package gov.nasa.arc.l2tools.util;

import org.omg.CORBA.ORB;
import java.util.Properties;

public class OrbServices {

    static org.omg.CORBA.ORB orb;

    // Call the ORB's init method
    public static void init(String[] args) {
        Properties props = new Properties();
        //props.put("org.omg.CORBA.ORBInitialPort","1040");
        //props.put("org.omg.CORBA.ORBInitialHost","128.102.112.76");
        //System.out.println("props="+props);
        orb = ORB.init(args, props);
    }

    public static ORB getOrb() {
        if (orb==null) {
            String[] temp = {};
            init(temp);
        }
        return orb;
    }

}
