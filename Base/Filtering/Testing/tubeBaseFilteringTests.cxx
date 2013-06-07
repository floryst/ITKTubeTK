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

#include <iostream>
#include "tubeTestMain.h"

void RegisterTests( void )
{
  REGISTER_TEST( tubeBaseFilteringPrintTest );
  REGISTER_TEST( itkSubSampleTubeSpatialObjectFilterTest );
  REGISTER_TEST( itkSubSampleTubeTreeSpatialObjectFilterTest );
  REGISTER_TEST( itkStructureTensorRecursiveGaussianImageFilterTest );
  REGISTER_TEST( itkStructureTensorRecursiveGaussianImageFilterTestNew );
  REGISTER_TEST( itkTubeEnhancingDiffusion2DImageFilterTest );
  REGISTER_TEST( itkSheetnessMeasureImageFilterTest );
  REGISTER_TEST( itkSheetnessMeasureImageFilterTest2 );
  REGISTER_TEST( itkAnisotropicHybridDiffusionImageFilterTest );
  REGISTER_TEST( itkAnisotropicCoherenceEnhancingDiffusionImageFilterTest );
  REGISTER_TEST( itkAnisotropicEdgeEnhancementDiffusionImageFilterTest );
}
