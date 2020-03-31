//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import java.awt.Color;

import livdll.CandidateAssignment;

public class Candidate {

  protected int pointer;
  protected CandidateAssignment[] candidateAssignments;
  
  public Candidate(int pointer) {
    this.pointer = pointer;
    this.candidateAssignments = new CandidateAssignment[0];
  }

  public Candidate(int pointer, CandidateAssignment[] candidateAssignments) {
    this.pointer = pointer;
    this.candidateAssignments = candidateAssignments;
  }

  public int getPointer() { return pointer; }
  
  public CandidateAssignment[] getCandidateAssignments() { return candidateAssignments; }

  public void setCandidateAssignments(CandidateAssignment[] c) { candidateAssignments = c; }

  public int getWeight() {
      int ret = 0;
      if(candidateAssignments == null && candidateAssignments.length == 0) return ret;
      for(int i = 0; i < candidateAssignments.length; i++) 
	  ret = ret + candidateAssignments[i].getWeight();
      return ret;
  }

}
