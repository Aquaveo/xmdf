\tableofcontents
# Xmdf Tutorial {#Xmdf_Tutorial}

## Introduction {#Intro_Xmdf}
The purpose of this tutorial is to provide explanation on how to use the classes defined in xmsng to perform feature stamping. The examples provided in this tutorial refer to test cases that are in the xmsng/tutorial/TutStamping.cpp source file.

The xmsng library contains classes for performing feature stamping. The basic idea is to associate cross sections with a center line to produce a new feature (a "fill" embankment, a "cut" channel). The output from the feature stamp is a TIN and a set of break lines defining distinct characteristics of the new feature, such as the center line, shoulder, toe, and end cap. 

The xms::XmdfIo class is used to set up the inputs to the stamp operation and it also contains the outputs of the operation in the member variables m_outTin and m_outBreakLines;

## Example - Fill Embankment Feature Stamp {#Example_FillEmbankment}
The following example shows how to use the xmdf to make a simple fill embankment. The testing code for this example is TutStampingTests::test_StampFillEmbankment.

\snippet xmdf/xmdf/TutStamping.cpp snip_testStampFillEmbankment

The output from this test looks like the following image.

![Stamp fill embankment](doxygen/html/images/tutStamp_FillEmbankment.png)

## Example - Cut Embankment Feature Stamp {#Example_CutEmbankment}
The following example shows how to use the xmdf to make a simple cut embankment. This is just like the fill embankment but we have flipped the specified cross sections. The testing code for this example is TutStampingTests::test_StampCutEmbankment.

\snippet xmdf/xmdf/TutStamping.cpp snip_testStampCutEmbankment

The output from this test looks like the following image.

![Stamp cut embankment](doxygen/html/images/tutStamp_CutEmbankment.png)

## Example - Wing Wall with Fill Embankment Feature Stamp {#Example_WingWall}
The following example shows how to use the xmdf to make a fill embankment with a wing wall end cap with a specified angle. An angle can be specified with all end caps and it will affect the orientation of the end cap from the center line to the shoulder. The wing wall has an additional angle that can be specified that changes the orientation from the shoulder to the toe. The testing code for this example is TutStampingTests::test_StampWingWall.

\snippet xmdf/xmdf/TutStamping.cpp snip_testStampWingWall

The output from this test looks like the following image.

![Stamp fill embankment with a wing wall end cap](doxygen/html/images/tutStamp_WingWall.png)

## Example - Sloped Abutment with Fill Embankment Feature Stamp {#Example_SlopedAbutment}
The following example shows how to use the xmdf to make a fill embankment with a sloped abutment end cap.  The testing code for this example is TutStampingTests::test_StampSlopedAbutment.

\snippet xmdf/xmdf/TutStamping.cpp snip_testStampSlopedAbutment

The output from this test looks like the following image.

![Stamp fill embankment with a sloped abutment end cap](doxygen/html/images/tutStamp_SlopedAbutment.png)

## Example - Guidebank with Fill Embankment Feature Stamp {#Example_Guidebank}
The following example shows how to use the xmdf to make a fill embankment with a guidebank end cap.  The testing code for this example is TutStampingTests::test_StampGuidebank.

\snippet xmdf/xmdf/TutStamping.cpp snip_testStampGuidebank

The output from this test looks like the following image.

![Stamp fill embankment with a guidebank end cap](doxygen/html/images/tutStamp_Guidebank.png)

## Example - Fill Embankment Feature Stamp Intersecting Bathymetry {#Example_Bathymetry}
The following example shows how to use the xmdf to make a fill embankment with an underlying bathymetry. The stamp is intersected and potentially cut off by the bathymetry. The testing code for this example is TutStampingTests::test_StampIntersectBathymetry.

\snippet xmdf/xmdf/TutStamping.cpp snip_testStampIntersectBathymetry

The output from this test looks like the following image. The red triangles are the bathymetry.

![Stamp fill embankment cut off by bathymetry](doxygen/html/images/tutStamp_Intersect.png)

