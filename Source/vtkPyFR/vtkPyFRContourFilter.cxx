#include "vtkPyFRContourFilter.h"

#include <vtkDataObject.h>
#include <vtkDataObjectTypes.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkInstantiator.h>
#include <vtkObjectFactory.h>

#include "PyFRContourFilter.h"

#include "vtkPyFRData.h"
#include "vtkPyFRContourData.h"

//----------------------------------------------------------------------------
int vtkPyFRContourFilter::RegisterPyFRDataTypes()
{
  vtkInstantiator::RegisterInstantiator("vtkPyFRData",
                                        &New_vtkPyFRData);
  vtkInstantiator::RegisterInstantiator("vtkPyFRContourData",
                                        &New_vtkPyFRContourData);

  return 1;
}

int vtkPyFRContourFilter::PyFRDataTypesRegistered =
  vtkPyFRContourFilter::RegisterPyFRDataTypes();

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPyFRContourFilter);

//----------------------------------------------------------------------------
vtkPyFRContourFilter::vtkPyFRContourFilter() : ContourField(0)
{
  this->ColorPalette = 1;
  this->ColorRange[0] = 0.;
  this->ColorRange[1] = 1.;
}

//----------------------------------------------------------------------------
vtkPyFRContourFilter::~vtkPyFRContourFilter()
{
}

//----------------------------------------------------------------------------
int vtkPyFRContourFilter::RequestData(
  vtkInformation*,
  vtkInformationVector** inputVector,
  vtkInformationVector* outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPyFRData *input = vtkPyFRData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPyFRContourData *output = vtkPyFRContourData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  PyFRContourFilter filter;
  for (unsigned i=0;i<this->ContourValues.size();i++)
    {
    filter.AddContourValue(this->ContourValues[i]);
    }

  filter.SetContourField(this->ContourField);
  filter(input->GetData(),output->GetData());
  filter.MapFieldOntoIsosurfaces(this->MappedField,input->GetData(),
                                 output->GetData());

  std::pair<float,float> crange = filter.ColorRange();
  this->ColorRange[0] = crange.first;
  this->ColorRange[1] = crange.second;

  output->SetColorPalette(this->ColorPalette,this->ColorRange);
  this->minmax = filter.DataRange();
  output->Modified();
  return 1;
}
//----------------------------------------------------------------------------

int vtkPyFRContourFilter::FillInputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPyFRData");
  return 1;
}
//----------------------------------------------------------------------------

void vtkPyFRContourFilter::SetNumberOfContours(int i)
{
  this->ContourValues.resize(i);
  this->Modified();
}
//----------------------------------------------------------------------------

void vtkPyFRContourFilter::SetContourValue(int i,double value)
{
  if (i < this->ContourValues.size() && this->ContourValues[i] != value)
    {
    this->ContourValues[i] = value;
    this->Modified();
    }
}
//----------------------------------------------------------------------------

void vtkPyFRContourFilter::SetContourField(int i)
{
  if (this->ContourField != i)
    {
    this->ContourField = i;
    this->Modified();
    }
}
//----------------------------------------------------------------------------

void vtkPyFRContourFilter::SetMappedField(int i)
{
  if (this->MappedField != i)
    {
    this->MappedField = i;
    this->Modified();
    }
}
//----------------------------------------------------------------------------

std::pair<float,float> vtkPyFRContourFilter::Range() const {
  return this->minmax;
}

void vtkPyFRContourFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "ContourField: " << this->ContourField << "\n";
  os << indent << "MappedField: " << this->MappedField << "\n";
  os << indent << "ContourValues: ";
  for (unsigned i=0;i<this->ContourValues.size();i++)
    os << this->ContourValues[i] << "\n";
}
