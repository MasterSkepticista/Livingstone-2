#!/bin/sh

# Call the JMPL compiler to compile cbAndLeds.jmpl to test.xmpl
# This script illustrates how to use the JMPL compiler
# The JMPL files are "cb.jmpl" and "cbAndLeds.jmpl", in the CWD

# The common directory path for the Skunkworks files
# This will differ from user to user, system to system
SKUNKWORKS_ROOT=/home/taylor/L2Skunk/released_2.7.1/skunkworks

# The JAR file that contains the JMPL compiler
COMPILER_JAR=$SKUNKWORKS_ROOT/jars/skunkworksTools.jar

# The JAR file for the DOM classes (http://koala.ilog.fr/domlight/)
DOM_JAR=$SKUNKWORKS_ROOT/java/lib/domlight-1.0.jar:

# The JAR file for the ANTLR classes (http://www.antlr.org)
ANTLR_JAR=$SKUNKWORKS_ROOT/java/lib/antlr.jar

# The classpath
CLASS_PATH=$COMPILER_JAR:$DOM_JAR:$ANTLR_JAR

# The Java class that is the JMPL compiler's entry point
COMPILER_CLASS=gov.nasa.arc.skunkworks.io.jmpl.JmplCompiler

# The root component type, as defined in the JMPL file
COMPONENT_TYPE=cbAndLeds

# The name of the component in the XMPL; also the name of output .xmpl file
COMPONENT_NAME=test

# The JMPL source files
SOURCE_FILES="cb.jmpl cbAndLeds.jmpl"

# The command line
java -classpath $CLASS_PATH $COMPILER_CLASS $COMPONENT_TYPE $COMPONENT_NAME $SOURCE_FILES

# The output will be a file named $COMPONENT_NAME.xmpl
