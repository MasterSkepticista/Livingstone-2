//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

public interface ToolFactory {

  /* The bean here is just the on selected or null so 
   * we know where to put the new one. */
  public void createDataModel(int index);

  public void destroyDataModel(int index) throws Throwable;

  public Tool newCommandLineTool(int index) throws ArrayIndexOutOfBoundsException;

  public Tool newCandidateManagerTool(int index) throws ArrayIndexOutOfBoundsException;

  public Tool newStanleyTool(int index, CommandLine commandLine) throws ArrayIndexOutOfBoundsException;

  public Tool newHistoryTableTool(int index) throws ArrayIndexOutOfBoundsException;

  public Tool newControlPanelTool(int index) throws ArrayIndexOutOfBoundsException;

  /* This could also be the model browser */
  public Tool newHarnessEditorTool(int index) throws ArrayIndexOutOfBoundsException;

  public Tool newTimeLineTool(int index) throws ArrayIndexOutOfBoundsException;

}
