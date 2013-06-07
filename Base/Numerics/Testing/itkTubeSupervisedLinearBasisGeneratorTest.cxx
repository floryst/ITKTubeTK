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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <itkImage.h>
#include <itkFilterWatcher.h>
#include <itkExceptionObject.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "itkRecursiveGaussianImageFilter.h"

#include "itkTubeSupervisedLinearBasisGenerator.h"

int itkTubeSupervisedLinearBasisGeneratorTest(int argc, char* argv [] )
{
  if( argc != 5 )
    {
    std::cerr << "Missing arguments." << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
      << " inputImage inputMask outputBasis0Image outputBasis1Image"
      << std::endl;
    return EXIT_FAILURE;
    }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  // Define the pixel type
  typedef float PixelType;

  // Declare the types of the images
  typedef itk::Image<PixelType, Dimension>  ImageType;

  // Declare the reader and writer
  typedef itk::ImageFileReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter< ImageType > WriterType;


  // Declare the type for the filter
  typedef itk::tube::SupervisedLinearBasisGenerator< ImageType,
    ImageType > FilterType;

  // Create the reader and writer
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject& e )
    {
    std::cerr << "Exception caught during input read:" << std::endl << e;
    return EXIT_FAILURE;
    }
  ImageType::Pointer inputImage = reader->GetOutput();

  // Create the reader and writer
  ReaderType::Pointer maskReader = ReaderType::New();
  maskReader->SetFileName( argv[2] );
  try
    {
    maskReader->Update();
    }
  catch( itk::ExceptionObject& e )
    {
    std::cerr << "Exception caught during input read:" << std::endl << e;
    return EXIT_FAILURE;
    }
  ImageType::Pointer maskImage = maskReader->GetOutput();

  typedef itk::RecursiveGaussianImageFilter< ImageType > BlurFilterType;

  BlurFilterType::Pointer blur0 = BlurFilterType::New();
  blur0->SetInput( inputImage );
  blur0->SetSigma( 2.0 );
  blur0->SetOrder(
    itk::RecursiveGaussianImageFilter<ImageType>::FirstOrder );
  blur0->SetDirection( 0 );
  blur0->Update();

  BlurFilterType::Pointer blur1 = BlurFilterType::New();
  blur1->SetInput( inputImage );
  blur1->SetSigma( 2.0 );
  blur1->SetOrder(
    itk::RecursiveGaussianImageFilter<ImageType>::FirstOrder );
  blur1->SetDirection( 1 );
  blur1->Update();

  BlurFilterType::Pointer blur2 = BlurFilterType::New();
  blur2->SetInput( inputImage );
  blur2->SetSigma( 4.0 );
  blur2->SetOrder(
    itk::RecursiveGaussianImageFilter<ImageType>::ZeroOrder );
  blur2->SetDirection( 0 );
  blur2->Update();

  BlurFilterType::Pointer blur3 = BlurFilterType::New();
  blur3->SetInput( inputImage );
  blur3->SetSigma( 4.0 );
  blur3->SetOrder(
    itk::RecursiveGaussianImageFilter<ImageType>::ZeroOrder );
  blur3->SetDirection( 1 );
  blur3->Update();

  FilterType::Pointer filter = FilterType::New();
  filter->SetFeatureImage( inputImage );
  filter->AddFeatureImage( blur0->GetOutput() );
  filter->AddFeatureImage( blur1->GetOutput() );
  filter->AddFeatureImage( blur2->GetOutput() );
  filter->AddFeatureImage( blur3->GetOutput() );
  filter->UpdateWhitenFeatureImageStats( 0 );
  filter->UpdateWhitenFeatureImageStats( 1 );
  filter->UpdateWhitenFeatureImageStats( 2 );
  filter->UpdateWhitenFeatureImageStats( 3 );
  filter->UpdateWhitenFeatureImageStats( 4 );
  filter->WhitenFeatureImage( 0 );
  filter->WhitenFeatureImage( 1 );
  filter->WhitenFeatureImage( 2 );
  filter->WhitenFeatureImage( 3 );
  filter->WhitenFeatureImage( 4 );
  filter->SetLabelmap( maskImage );
  filter->SetObjectId( 255 );
  filter->AddObjectId( 127 );
  filter->Update();
  filter->SetLabelmap( NULL );
  filter->UpdateBasisImages();

  std::cout << "Number of Basis = " << filter->GetNumberOfBasis()
    << std::endl;
  for( unsigned int i=0; i<filter->GetNumberOfBasis(); i++ )
    {
    std::cout << "Basis" << i << " = " << filter->GetBasisValue(i) << " : "
      << filter->GetBasisVector(i) << std::endl;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetUseCompression( true );
  writer->SetInput( filter->GetBasisImage(0) );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject& e )
    {
    std::cerr << "Exception caught during write:" << std::endl << e;
    return EXIT_FAILURE;
    }

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName( argv[4] );
  writer2->SetUseCompression( true );
  writer2->SetInput( filter->GetBasisImage(1) );
  try
    {
    writer2->Update();
    }
  catch( itk::ExceptionObject& e )
    {
    std::cerr << "Exception caught during write:" << std::endl << e;
    return EXIT_FAILURE;
    }

  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;
}
