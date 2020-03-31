//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : EventLog.java
*
* This will eventually be an event logging utility.  Right now, it just prints
* events to standard output.
*
* CHANGELOG:
* 11/10/99      Mhs     Created
*/

package gov.nasa.arc.l2tools.util;

import java.util.Vector;

/**
 * This will eventually be an event logging utility.  Right now, it just prints
 * events to standard output.
 */
public class EventLog {

    /** Record the exception in an event log.  For now, it just prints it. */
    public static void report(Throwable e) {
        e.printStackTrace();
    }

    /** Record a message in an event log.  For now, it just prints it. */
    public static void report(String msg) {
        System.out.println(msg);
    }

    /** Record a message in an event log.  For now, it just prints it. */
    public static void report(String[] msg) {
        for (int i=0;i<msg.length;i++)
            System.out.println(msg[i]);
    }

    /** Record a message in an event log.  This one accepts a vector of strings. */
    public static void report(Vector vec) {
        try {
            int max = vec.size();
            for (int i=0;i<max;i++)
                System.out.println((String)vec.elementAt(i));
        } catch (Exception e) {
            EventLog.report(e); // in case the cast fails
        }
    }

}
