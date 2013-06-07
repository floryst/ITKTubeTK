/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#include <itkImage.h>
#include <itkFilterWatcher.h>
#include <itkExceptionObject.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkTubeRidgeSeedSupervisedLinearBasisGenerator.h>

// Forward declaration
template< int Dimension > int
DoIt(int argc, char * argv [] );

// Pick the method to run based on testnum
int itkTubeRidgeSeedSupervisedLinearBasisGeneratorTest( int argc,
  char * argv [] )
{
  if( argc != 5 )
    {
    std::cerr << "Missing arguments." << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " testnum inputImage maskImage outputImage"
      << std::endl;
    return EXIT_FAILURE;
    }

  switch( std::atoi( argv[1] ) )
    {
    case 0:
      {
      std::cout << "Test Number = 0" << std::endl;
      return DoIt< 2 >( argc, argv );
      }
    case 1:
      {
      std::cout << "Test Number = 1" << std::endl;
      return DoIt< 3 >( argc, argv );
      }
    default:
      {
      std::cout << "Testnum not found" << std::endl;
      return EXIT_FAILURE;
      }
    };
}

template< int Dimension >
int DoIt(int argc, char * argv [] )
{
  int testNum = std::atoi( argv[1] );

  bool error = false;

  // Define the pixel type
  typedef float                                   PixelType;
  // Declare the type of the images
  typedef itk::Image< PixelType, Dimension >      ImageType;

  // Define the mask pixel type
  typedef unsigned char                           MaskPixelType;
  // Declare the type of the mask image
  typedef itk::Image< MaskPixelType, Dimension >  MaskImageType;

  // Declare the reader and writer
  typedef itk::ImageFileReader< ImageType >       ReaderType;

  typedef itk::ImageFileWriter< ImageType >       WriterType;

  // Declare the mask reader and writer
  typedef itk::ImageFileReader< MaskImageType >   MaskReaderType;

  // Declare the type for the Filter
  typedef itk::tube::RidgeSeedSupervisedLinearBasisGenerator< ImageType,
    MaskImageType >                               FunctionType;

  // Create the reader
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during read:" << std::endl << e;
    return EXIT_FAILURE;
    }
  typename ImageType::Pointer inputImage = reader->GetOutput();

  // Create the mask reader
  typename MaskReaderType::Pointer maskReader = MaskReaderType::New();
  maskReader->SetFileName( argv[3] );
  try
    {
    maskReader->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during mask read:" << std::endl << e;
    return EXIT_FAILURE;
    }
  typename MaskImageType::Pointer maskImage = maskReader->GetOutput();

  typename FunctionType::Pointer func = FunctionType::New();
  func->SetRidgeImage( inputImage );

  func->SetLabelmap( maskImage );
  func->SetObjectId( 255 );
  func->AddObjectId( 127 );

  typename FunctionType::RidgeScalesType scales(5);
  scales[0] = 0.3;
  scales[1] = 0.5;
  scales[2] = 0.75;
  scales[3] = 1.0;
  scales[4] = 1.5;
  func->SetScales( scales );

  func->Update();

  // Re-run without a labelmap to cause it to label the entire image
  func->SetLabelmap( NULL );
  func->UpdateBasisImages();

  char filename[4096];
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetUseCompression( true );
  std::cout << "Number of Basis = " << func->GetNumberOfBasis()
    << std::endl;
  for( unsigned int i=0; i<3; i++ )
    {
    std::sprintf( filename, "%s.basis%02d.mha", argv[4], i );
    writer->SetFileName( filename );
    writer->SetInput( func->GetBasisImage(i) );
    std::cout << "Basis" << i << " = " << func->GetBasisValue(i) << " : "
      << func->GetBasisVector(i) << std::endl;
    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject& e)
      {
      std::cerr << "Exception caught during write:" << std::endl << e;
      error = true;
      }
    }

  for( unsigned int i=0; i<func->GetNumberOfFeatures(); i++ )
    {
    std::sprintf( filename, "%s.f%02d.mha", argv[4], i );
    writer->SetFileName( filename );
    writer->SetInput( func->GetFeatureImage(i) );
    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject& e)
      {
      std::cerr << "Exception caught during write:" << std::endl << e;
      error = true;
      }
    }

  // Load RidgeSeedInfo

  // All objects should be automatically destroyed at this point
  if( !error )
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}
