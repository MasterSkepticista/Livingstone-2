//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : Assignment.java
*
* CHANGELOG:
* 05/06/00      Mhs     Created.
*/

package livdll;

import java.awt.Color;

/** I haven't combined this with Assignment.java yet. */
public class CandidateAssignment {

  protected String attribute;
  protected String value;
  protected int weight;
  // for display in the Candidate Manager
  protected Color backgroundColor;
  protected String text; // filtered toString text 
  protected boolean selected;


  public CandidateAssignment(String attribute, String value, int weight) {
    this.attribute = attribute;
    this.value = value;
    this.weight = weight;
    this.backgroundColor = Color.white;
    this.text = "";
    this.selected = false;
  }

  public String getAttribute() { return attribute; }
  public String getValue() { return value; }
  public int getWeight() { return weight; }

  public Color getBackgroundColor() {
    return backgroundColor;
  }

  public void setBackgroundColor( Color color) {
    backgroundColor = color;
  }

  public String getText() {
    return text;
  }

  public void setText( String text) {
    this.text = text;
  }

  public boolean isSelected() {
    return selected;
  }

  public void setSelected( boolean selected) {
    this.selected = selected;
  }

  public String toString() {
    return "<CandidateAssignment "+attribute+"="+value+":"+weight+">";
  }
  
  public boolean equals(Object obj) {
    if(obj instanceof CandidateAssignment) {
      CandidateAssignment c = (CandidateAssignment)obj;
      if(c.getAttribute() == attribute &&
	 c.getValue() == value &&
	 c.getWeight() == weight) return true;
      else return false;
    } else return super.equals(obj);
  }
}
