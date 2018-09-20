"""Test XmStamperIo_py.cpp"""
import unittest
import xmsstamper_py
from xmsstamper_py import stamper
import numpy as np

class TestXmStamper(unittest.TestCase):
  """XmStamper tests"""
  def test_cross_section_tutorial(self):
    """A test to serve as a tutorial"""

    #//------------------------------------------------------------------------------
    #/// \brief Test tutorial for symmetric cross section interpolation
    #//------------------------------------------------------------------------------
    io = stamper.XmStamperIo()

    io.center_line = ( ( 0, 0 ),( 5, 0 ),( 10, 0 ),( 15, 0 ),( 20, 0 ),( 25, 0 ) )
    io.cross_section_size = 6
    io.stamping_type = 0

    cross_section = stamper.XmStampCrossSection()

    cross_section.left = ( ( 0, 10 ),( 1, 11 ),( 2, 12 ),( 4, 11 ),( 5, 10 ),( 10, 5 ),( 15, 0 ) )
    cross_section.idx_left_shoulder = 2
    cross_section.right = cross_section.left
    cross_section.idx_right_shoulder = cross_section.idx_left_shoulder
    io.set_cross_section(1, cross_section)

    cross_section.left = ( ( 0, 20 ),( 4, 19 ),( 6, 18 ),( 8, 18 ),( 10, 20 ),( 15, 15 ),( 20, 10 ) )
    cross_section.idx_left_shoulder = 3
    cross_section.right = cross_section.left
    cross_section.idx_right_shoulder = cross_section.idx_left_shoulder
    io.set_cross_section(4, cross_section)

    #interp_section = XmStampInterpCrossSection()
    #interp_section.InterpMissingCrossSections(io)

    expected = (2, 2, 3, 3, 3, 3)
    #np.testing.assert_array_equal(
