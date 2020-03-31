// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * LivingstoneCommand.java
 *
 * Created on May 6, 2003, 4:32 PM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public class LivingstoneCommand {
    
    private boolean breakpoint = false;
    private int type = UNKNOWN;
    private String cmd = "";
    private String args = "";
    
    public static int UNKNOWN = 0;
    public static int FIND_CANDIDATES = 1;
    public static int ASSIGNMENT = 2;
    public static int PROGRESS = 3;
    public static int PROPAGATE = 4;
    public static int COMMENT = 5;
    public static int RESTART = 6;
    
    /** Creates a new instance of LivingstoneCommand */
    public LivingstoneCommand(String line) {
        line = line.trim();
        if(line.startsWith("//") || line.trim().equals("")) {
            type = COMMENT;
            cmd = line;
            return;
        }
        if(line.toLowerCase().startsWith("bp ")) {
            breakpoint = true;
            line = line.substring(3).trim();
        } else {
            breakpoint = false;
        }
        int i = line.indexOf(" ");
        if(i > 0) {
            cmd = line.substring(0,i);
            args = line.substring(i+1);
        } else {
            cmd = line;
        }
        if(cmd.equalsIgnoreCase("progress")) {
            type = PROGRESS;
        } else if(cmd.equalsIgnoreCase("min-progress")) {
            type = PROGRESS;
        } else if(cmd.equalsIgnoreCase("full-progress")) {
            type = PROGRESS;
        } else if(cmd.equalsIgnoreCase("assign")) {
            type = ASSIGNMENT;
        } else if(cmd.equalsIgnoreCase("propagate")) {
            type = PROPAGATE;
        } else if(cmd.equalsIgnoreCase("fc")) {
            type = FIND_CANDIDATES;
        } else if(cmd.equalsIgnoreCase("prune-and-search")) {
            type = FIND_CANDIDATES;
        } else if(cmd.equalsIgnoreCase("prune-search")) {
            type = FIND_CANDIDATES;
        } else if(cmd.equalsIgnoreCase("find-fresh")) {
            type = FIND_CANDIDATES;
        } else if(cmd.equalsIgnoreCase("extend")) {
            type = FIND_CANDIDATES;
        } else if(cmd.equalsIgnoreCase("restart")) {
            type = RESTART;
        } else {
            type = UNKNOWN;
        }
    }
    
    public boolean isBreakpoint() { return breakpoint; }
    
    public int getType() { return type; }
    
    public String getCommand() { return cmd; }
    
    public String getArguments() { return args; }
    
    public String toString() { return cmd+" "+args; }
    
}
