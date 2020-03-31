package LivingstoneCorba;


/**
* LivingstoneCorba/LivingstoneCommandLineHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from LivingstoneCorba.idl
* Thursday, April 20, 2006 4:53:49 PM PDT
*/

abstract public class LivingstoneCommandLineHelper
{
  private static String  _id = "IDL:LivingstoneCorba/LivingstoneCommandLine:1.0";

  public static void insert (org.omg.CORBA.Any a, LivingstoneCorba.LivingstoneCommandLine that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static LivingstoneCorba.LivingstoneCommandLine extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_interface_tc (LivingstoneCorba.LivingstoneCommandLineHelper.id (), "LivingstoneCommandLine");
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static LivingstoneCorba.LivingstoneCommandLine read (org.omg.CORBA.portable.InputStream istream)
  {
    return narrow (istream.read_Object (_LivingstoneCommandLineStub.class));
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, LivingstoneCorba.LivingstoneCommandLine value)
  {
    ostream.write_Object ((org.omg.CORBA.Object) value);
  }

  public static LivingstoneCorba.LivingstoneCommandLine narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof LivingstoneCorba.LivingstoneCommandLine)
      return (LivingstoneCorba.LivingstoneCommandLine)obj;
    else if (!obj._is_a (id ()))
      throw new org.omg.CORBA.BAD_PARAM ();
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      LivingstoneCorba._LivingstoneCommandLineStub stub = new LivingstoneCorba._LivingstoneCommandLineStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

  public static LivingstoneCorba.LivingstoneCommandLine unchecked_narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof LivingstoneCorba.LivingstoneCommandLine)
      return (LivingstoneCorba.LivingstoneCommandLine)obj;
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      LivingstoneCorba._LivingstoneCommandLineStub stub = new LivingstoneCorba._LivingstoneCommandLineStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

}
