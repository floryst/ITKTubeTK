##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

include_regular_expression( "^.*$" )

set( TEMP ${TubeTK_BINARY_DIR}/Temporary )

set( PROJ_EXE
     ${TubeTK_LAUNCHER} $<TARGET_FILE:${MODULE_NAME}> )

set( CompareTextFiles_EXE
     ${TubeTK_LAUNCHER} $<TARGET_FILE:CompareTextFiles> )

set( CompareImages_EXE
     ${TubeTK_LAUNCHER} $<TARGET_FILE:CompareImages> )

# Test1
ExternalData_Add_Test( TubeTKData
  NAME ${MODULE_NAME}-Test1
  COMMAND ${PROJ_EXE}
    DATA{${TubeTK_DATA_ROOT}/Branch-truth-new.tre}
    ${TEMP}/${MODULE_NAME}-Test1.vtk
    )

# Test2
ExternalData_Add_Test( TubeTKData
  NAME ${MODULE_NAME}-Test2
  COMMAND ${PROJ_EXE}
    DATA{${TubeTK_DATA_ROOT}/Branch-truth-new.tre}
    ${TEMP}/${MODULE_NAME}-Test2.stl
    )

# Test3
ExternalData_Add_Test( TubeTKData
  NAME ${MODULE_NAME}-Test3
  COMMAND ${PROJ_EXE}
    DATA{${TubeTK_DATA_ROOT}/Branch-truth-new.tre}
    ${TEMP}/${MODULE_NAME}-Test3.stt
    )
set_tests_properties( ${MODULE_NAME}-Test3
  PROPERTIES WILL_FAIL true )

# Test4
ExternalData_Add_Test( TubeTKData
  NAME ${MODULE_NAME}-Test4
  COMMAND ${PROJ_EXE}
    DATA{${TubeTK_DATA_ROOT}/ConvertTubesToSurfaceTestFile.tre}
    ${TEMP}/${MODULE_NAME}-Test4_Surface.vtk
    --centerline ${TEMP}/${MODULE_NAME}-Test4_Centerline.vtp )

# Test5 - Compare - Surface
ExternalData_Add_Test( TubeTKData
  NAME ${MODULE_NAME}-Test4-Compare-Surface
  COMMAND ${CompareTextFiles_EXE}
    -t ${TEMP}/${MODULE_NAME}-Test4_Surface.vtk
    -b DATA{${TubeTK_DATA_ROOT}/ConvertTubesToSurfaceTestFile_Surface.vtk}
    -d 0.01 )

set_tests_properties( ${MODULE_NAME}-Test4-Compare-Surface PROPERTIES DEPENDS
  ${MODULE_NAME}-Test4 )
