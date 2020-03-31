//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

/* Holds a copy of the key data model state variables.  This is used to
   snapshot a particular state so that, for instance, it can be
   regenerated later, even after the engine has moved on.  I'm using it
   initially to stamp presentations with a particular engine state so
   that they have access to the engine, but if the engine has moved on,
   they'll fail. */
public class DataModelState {

  protected DataModel dataModel = null;
  protected Candidate candidate = null;
  protected int startTimestep = -1;
  protected int startTimeStep = -1;
  protected int endTimeStep = -1;
  protected String modelName = null;

  public DataModelState(DataModel dataModel) {
    this.dataModel = dataModel;
    this.modelName = dataModel.getModel();
    this.startTimeStep = dataModel.getStartTimeStep();
    this.endTimeStep = dataModel.getEndTimeStep();
    this.candidate = dataModel.getSelectedCandidate();
  }

  public ScenarioInterface getPipeline() throws DataModelStateError {
    if ( dataModel==null
	 || modelName != dataModel.getModel() 
	 || endTimeStep != dataModel.getEndTimeStep()
	 || startTimeStep != dataModel.getStartTimeStep()
	 || modelName != dataModel.getModel())
      throw new DataModelStateError("Can't access the engine through this saved state.  The engine has moved on.");
    return dataModel.getPipeline();
  }



}
