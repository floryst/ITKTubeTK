/*=========================================================================

Library:   TubeTK

Copyright 2017 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 ( the "License" );
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __tubeComputeTubeTortuosityMeasures_h
#define __tubeComputeTubeTortuosityMeasures_h

// ITK includes
#include <itkProcessObject.h>
#include <itkVesselTubeSpatialObject.h>

// TubeTK includes
#include "tubeWrappingMacros.h"
#include "tubeTubeMath.h"

#include "itktubeTortuositySpatialObjectFilter.h"

namespace tube
{
/** \class ComputeTubeTortuosityMeasures
 *
 *  \ingroup TubeTKITK
 */

template< unsigned int Dimension >
class ComputeTubeTortuosityMeasures:
  public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ComputeTubeTortuosityMeasures              Self;
  typedef itk::ProcessObject                         Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer< const Self >            ConstPointer;
  typedef itk::VesselTubeSpatialObject< Dimension >  TubeType;

  typedef itk::tube::TortuositySpatialObjectFilter< TubeType >
    FilterType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( ComputeTubeTortuosityMeasures, ProcessObject );

  /** Set/Get measure flag */
  tubeWrapSetMacro( MeasureFlag, int, Filter );
  tubeWrapGetMacro( MeasureFlag, int, Filter );

  /* Setter for input tube */
  tubeWrapSetConstObjectMacro( Input, TubeType, Filter );

  /* Getters for metrics */
  tubeWrapGetMacro( AverageRadiusMetric, double, Filter );
  tubeWrapGetMacro( DistanceMetric, double, Filter );
  tubeWrapGetMacro( InflectionCountMetric, double, Filter );
  tubeWrapGetMacro( InflectionCount1Metric, double, Filter );
  tubeWrapGetMacro( InflectionCount2Metric, double, Filter );
  tubeWrapGetMacro( ChordLengthMetric, double, Filter );
  tubeWrapGetMacro( PathLengthMetric, double, Filter );
  tubeWrapGetMacro( Percentile95Metric, double, Filter );
  tubeWrapGetMacro( SumOfAnglesMetric, double, Filter );
  tubeWrapGetMacro( SumOfTorsionMetric, double, Filter );
  tubeWrapGetMacro( TotalCurvatureMetric, double, Filter );
  tubeWrapGetMacro( TotalSquaredCurvatureMetric, double, Filter );

  /* Setters for parameters */
  tubeWrapSetMacro( SmoothingScale, double, Filter );
  tubeWrapSetMacro( SmoothingMethod, ::tube::SmoothTubeFunctionEnum, Filter );
  tubeWrapSetMacro( NumberOfBins, size_t, Filter );
  tubeWrapSetMacro( HistogramMin, double, Filter );
  tubeWrapSetMacro( HistogramMax, double, Filter );

  /* Runs the application */
  tubeWrapUpdateMacro( Filter );

  int GetCurvatureHistogramMetric( unsigned int bin ) const
  {
    return m_Filter->GetCurvatureHistogramMetric( bin );
  }

protected:
  ComputeTubeTortuosityMeasures( void );
  ~ComputeTubeTortuosityMeasures() {}
  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  ComputeTubeTortuosityMeasures( const Self & );

  void operator=( const Self & );

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const DataObjectIdentifierType &, itk::DataObject * ) {};

  typename FilterType::Pointer m_Filter;

};

} // End namespace tube


#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeComputeTubeTortuosityMeasures.hxx"
#endif

#endif // End !defined( __tubeComputeTubeTortuosityMeasures_h )
