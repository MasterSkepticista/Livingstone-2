// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package livdll;

import java.util.Comparator;

public class MyComparator implements Comparator {

  static int MAX_DIFF = Integer.MAX_VALUE - Character.MAX_VALUE;

    public int compare(Object o1, Object o2) {
        char[] v1 = o1.toString().toCharArray();
	char[] v2 = o2.toString().toCharArray();
	int len1 = v1.length;
	int len2 = v2.length;
	int i1 = 0;
	int i2 = 0;
	int d1 = 0;
	int d2 = 0;
	int n1 = 0;
	int n2 = 0;

	while (i1<len1 && i2<len2) {
	    while(i1+d1<len1 && Character.isDigit(v1[i1+d1])) {
	      d1++;
	    }
	    
	    if(d1==0) {
	      n1 = Character.getNumericValue(v1[i1++]) + MAX_DIFF;
	    } else {
	      String num = new String(v1, i1, d1);
	      try {
	       n1 = Integer.parseInt(num);
	      } catch(NumberFormatException e) {
		System.out.println(e);
	      }
	      i1 = i1 + d1 - 1;
	      d1 = 0;
	    }



	    while(i2+d2<len2 && Character.isDigit(v2[i2+d2])) {
	      d2++;
	    }
	    
	    if(d2==0) {
	      n2 = Character.getNumericValue(v2[i2++]) + MAX_DIFF;
	    } else {
	      String num = new String(v2, i2, d2);
	      try {
	       n2 = Integer.parseInt(num);
	      } catch(NumberFormatException e) {
		System.out.println(e);
	      }
	      i2 = i2 + d2;
	      d2 = 0;
	    }


	    if(n1 != n2) return n1 - n2;
	}

	return len1 - len2;
    }

}
