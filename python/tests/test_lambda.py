#!/usr/bin/env python
# Copyright (C) 2015 Swift Navigation Inc.
# Copyright (C) 2007-2008 by T.TAKASU, All rights reserved.
# Contact: Bhaskar Mookerji <mookerji@swiftnav.com>
#
# This source is subject to the license found in the file 'LICENSE' which must
# be be distributed together with this source. All other rights reserved.
#
# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
# EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

# Taken from the lambda/mlambda integer least square tests for rtklib

import numpy as np
import swiftnav.lambda_ as l

def test_lambda1():
  m = 2
  x = np.array([1585184.171,
                -6716599.430,
                3915742.905,
                7627233.455,
                9565990.879,
                989457273.200])
  sigma = np.matrix([[0.227134,   0.112202,   0.112202,   0.112202,   0.112202,   0.103473],
                     [0.112202,   0.227134,   0.112202,   0.112202,   0.112202,   0.103473],
                     [0.112202,   0.112202,   0.227134,   0.112202,   0.112202,   0.103473],
                     [0.112202,   0.112202,   0.112202,   0.227134,   0.112202,   0.103473],
                     [0.112202,   0.112202,   0.112202,   0.112202,   0.227134,   0.103473],
                     [0.103473,   0.103473,   0.103473,   0.103473,   0.103473,   0.434339]])
  F, s = l.lambda_solution_(x, np.asfortranarray(sigma), m)
  assert np.allclose(F,
                     np.array([[1.58518400e+06, -6.71659900e+06, 3.91574300e+06,
                                7.62723400e+06, 9.56599100e+06, 9.89457273e+08],
                               [1.58518400e+06, -6.71660000e+06, 3.91574300e+06,
                                7.62723300e+06, 9.56599100e+06, 9.89457273e+08]]))
  assert np.allclose(s, np.array([ 3.50798444,  3.70845619]))

def test_lambda2():
  m = 2
  x = np.array([-13324172.755747,
                -10668894.713608,
                -7157225.010770,
                -6149367.974367,
                -7454133.571066,
                -5969200.494550,
                8336734.058423,
                6186974.084502,
                -17549093.883655,
                -13970158.922370])
  sigma = np.matrix([[0.446320, 0.223160, 0.223160, 0.223160, 0.223160, 0.572775, 0.286388, 0.286388, 0.286388, 0.286388],
                     [0.223160, 0.446320, 0.223160, 0.223160, 0.223160, 0.286388, 0.572775, 0.286388, 0.286388, 0.286388],
                     [0.223160, 0.223160, 0.446320, 0.223160, 0.223160, 0.286388, 0.286388, 0.572775, 0.286388, 0.286388],
                     [0.223160, 0.223160, 0.223160, 0.446320, 0.223160, 0.286388, 0.286388, 0.286388, 0.572775, 0.286388],
                     [0.223160, 0.223160, 0.223160, 0.223160, 0.446320, 0.286388, 0.286388, 0.286388, 0.286388, 0.572775],
                     [0.572775, 0.286388, 0.286388, 0.286388, 0.286388, 0.735063, 0.367531, 0.367531, 0.367531, 0.367531],
                     [0.286388, 0.572775, 0.286388, 0.286388, 0.286388, 0.367531, 0.735063, 0.367531, 0.367531, 0.367531],
                     [0.286388, 0.286388, 0.572775, 0.286388, 0.286388, 0.367531, 0.367531, 0.735063, 0.367531, 0.367531],
                     [0.286388, 0.286388, 0.286388, 0.572775, 0.286388, 0.367531, 0.367531, 0.367531, 0.735063, 0.367531],
                     [0.286388, 0.286388, 0.286388, 0.286388, 0.572775, 0.367531, 0.367531, 0.367531, 0.367531, 0.735063]])
  F, s = l.lambda_solution_(x, np.asfortranarray(sigma), m)
  assert np.allclose(F,
                     np.array([[-13324188., -10668900.99999994, -7157236.00000014,
                                -6149379.00000041, -7454143.00000017, -5969220.,
                                 8336726.00000008, 6186959.99999982, -17549108.00000054,
                                -13970171.00000022],
                               [-13324187.99999997, -10668907.99999984, -7157236.00000013,
                                -6149379.00000041, -7454143.00000013, -5969219.99999996,
                                 8336717.00000021, 6186959.99999983, -17549108.00000053,
                                -13970171.00000017]]))

  assert np.allclose(s, np.array([ 1506.43579559,  1612.81177168]))

def test_lambda3():
  """
  Verify that passing in perfectly decorrelated integers results in the
  same integers being identified.
  """
  m = 1
  x = np.array([-22, 22, -55, 55, -33, 33, -66, 66, -44, 44, -77, 77])
  sigma = np.eye(x.size)
  F, s = l.lambda_solution_(x, np.asfortranarray(sigma), m)
  assert np.allclose(F[0], x)

def test_lambda4():
  """
  Verify that the relative scale of the covariance matrix doesn't affect
  the solution.
  """
  m = 1
  x = np.array([-22, 22, -55, 55, -33, 33, -66, 66, -44, 44, -77, 77])
  sigma = np.eye(x.size) * 1000
  F, s = l.lambda_solution_(x, np.asfortranarray(sigma), m)
  assert np.allclose(F[0], x)
