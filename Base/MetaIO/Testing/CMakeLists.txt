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

find_package( SlicerExecutionModel REQUIRED )
include( ${SlicerExecutionModel_USE_FILE} )

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

find_package( PythonInterp )
if( PYTHON_EXECUTABLE )
  add_test( PrintSelf-BaseMetaIO ${PYTHON_EXECUTABLE}
    ${TubeTK_SOURCE_DIR}/CMake/PrintSelfCheck.py
    ${TubeTK_SOURCE_DIR}/Base/MetaIO )
endif( PYTHON_EXECUTABLE )

set( CompareImages_EXE
  ${TubeTK_LAUNCHER} $<TARGET_FILE:CompareImages> )

set( CompareTextFiles_EXE
  ${TubeTK_LAUNCHER} $<TARGET_FILE:CompareTextFiles> )

set( BASE_MetaIO_TESTS
  ${TubeTK_LAUNCHER} $<TARGET_FILE:tubeBaseMetaIOTests> )

set( BASE_MetaIO_HEADER_TEST
  ${TubeTK_LAUNCHER} $<TARGET_FILE:tubeBaseMetaIOHeaderTest> )

set( TEMP ${TubeTK_BINARY_DIR}/Temporary )

set( tubeBaseMetaIOTests_SRCS
  tubeBaseMetaIOPrintTest.cxx
  itktubeMetaLDATest.cxx
  itktubeMetaNJetLDATest.cxx
  itktubeMetaClassPDFTest.cxx
  itktubeMetaRidgeSeedTest.cxx
  itktubeMetaTubeExtractorTest.cxx )


add_executable( tubeBaseMetaIOHeaderTest
  tubeBaseMetaIOHeaderTest.cxx )
target_link_libraries( tubeBaseMetaIOHeaderTest
  TubeTKMetaIO ${ITK_LIBRARIES} )

set( tubeBaseMetaIO_External_Libraries )

set( no_install_option )
if( NOT TubeTK_INSTALL_DEVELOPMENT )
  set( no_install_option NO_INSTALL )
endif()

SEMMacroBuildCLI(
  NAME tubeBaseMetaIOTests
  ADDITIONAL_SRCS
    ${tubeBaseMetaIOTests_SRCS}
  LOGO_HEADER ${TubeTK_SOURCE_DIR}/Base/CLI/TubeTKLogo.h
  INCLUDE_DIRECTORIES
    ${TubeTK_SOURCE_DIR}/Base/Filtering
  TARGET_LIBRARIES
    ${tubeBaseMetaIO_External_Libraries}
    TubeTKMetaIO
    ${ITK_LIBRARIES}
    ITKIOMeta
  EXECUTABLE_ONLY
  ${no_install_option}
  )

add_test( NAME tubeBaseMetaIOHeaderTest
  COMMAND ${BASE_MetaIO_HEADER_TEST} )

add_test( NAME tubeBaseMetaIOPrintTest
  COMMAND ${BASE_MetaIO_TESTS}
    tubeBaseMetaIOPrintTest )

add_test( NAME itktubeMetaLDATest
  COMMAND ${BASE_MetaIO_TESTS}
    itktubeMetaLDATest
      ${TEMP}/itktubeMetaLDATest.mlda )

add_test( NAME itktubeMetaNJetLDATest
  COMMAND ${BASE_MetaIO_TESTS}
    itktubeMetaNJetLDATest
      ${TEMP}/itktubeMetaNJetLDATest.mnda )

add_test( NAME itktubeMetaClassPDFTest
  COMMAND ${BASE_MetaIO_TESTS}
    itktubeMetaClassPDFTest
      ${TEMP}/itktubeMetaClassPDFTest.mha )

add_test( NAME itktubeMetaRidgeSeedTest
  COMMAND ${BASE_MetaIO_TESTS}
    itktubeMetaRidgeSeedTest
      ${TEMP}/itktubeMetaRidgeSeedTest.mrs )

add_test( NAME itktubeMetaTubeExtractorTest
  COMMAND ${BASE_MetaIO_TESTS}
    itktubeMetaTubeExtractorTest
      ${TEMP}/itktubeMetaTubeExtractorTest.mtp )
