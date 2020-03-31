//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import javax.swing.DefaultListSelectionModel;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.ListSelectionEvent;

  public class MyListSelectionModel extends DefaultListSelectionModel implements ListSelectionListener {

    private CandidateManager cm;
    private boolean imadelastselection = false;

    public MyListSelectionModel(CandidateManager cm) {
      super();
      this.cm = cm;
      setSelectionMode(SINGLE_INTERVAL_SELECTION);
      addListSelectionListener(this);
    }

    public void valueChanged(ListSelectionEvent e) {
      if(e.getValueIsAdjusting() || imadelastselection) return;
      else {
	int[] sel = cm.getCurrentSelectionInterval();
	setSelectionInterval(sel[0], sel[sel.length-1]);
	imadelastselection = true;
      }
    }
  }
