import java.util.*;

public class LivMain {

    public static void main(String[] args) {
        test4();
        System.exit(0);
    }

    static void test1() {
        LivingstoneEngine e = LivingstoneEngine.cbfsFactory("hello",1);
        e.helloWorldJNI();
    }

    /* This doesn't do the right thing.  The same "address" comes up. */
    static void test2() {
        LivingstoneEngine e = LivingstoneEngine.cbfsFactory("hello",1);
        e.helloWorldJNI();

        LivingstoneEngine e1 = LivingstoneEngine.cbfsFactory("hello1",1);
        e1.helloWorldJNI();
    }

    /*
    static void test2(LivingstoneEngine e) {
        System.out.println("before");
        e.loadModel("test-input");
        e.printPropositions();
        System.out.println("after");
    }
    */

    // Test allocation
    static void test3() {
        System.out.println("before allocation");
        LivingstoneEngine e = LivingstoneEngine.cbfsFactory("test",1);
        System.out.println("  getName()="+e.getName());
        e.loadModel("test-input");
        LivingstoneEngine.deallocate(e);
        System.out.println("after");
    }

    // Test proposition callback
    static void test4() {
	System.out.println("before engine allocation");
	System.out.flush();
        LivingstoneEngine e = LivingstoneEngine.cbfsFactory("test",1);
	System.out.println("after engine allocation");
	System.out.flush();
        e.loadModel("cb");
	System.out.println("after loadModel");
	System.out.flush();
        e.mapOverPropositionStrings(new Observer() {
            public void update(Observable o, Object propname) {
                System.out.println("---> "+propname);
            }
        });
        System.out.println("after");
    }

    static void scenario1() {
        System.out.println("BEGIN scenario1");
        LivingstoneEngine e = LivingstoneEngine.cbfsFactory("test",1);
        e.loadModel("test-input");
        e.progress("vdecu.driver1CmdIn=on");
        e.progress("vdecu.driver1CmdIn=open");
        e.assign("valve1.reportedPosition","closed");
        e.conflict();
        e.findCandidates();
        e.cprops("valve1");
        e.assign("valve2.reportedPosition","closed");
        e.findCandidates();
        System.out.println("END scenario1");
    }

}
