set(PyFR_SRCS
  ColorTable.cu
  PyFRContourData.cu
  PyFRData.cu
  PyFRCrinkleClipFilter.cu
  PyFRContourFilter.cu
  PyFRConverter.cu
  PyFRGradientFilter.cu
  PyFRMergePointsFilter.cu
  PyFRParallelSliceFilter.cu
  PyFRWriter.cu
)

set( PyFRLibs )
foreach (fpType IN LISTS fpList)
  message(STATUS "setting up fpoint type '${fpType}'")
  set (pyfrLib "pyfr_${libSuffix_${fpType}}")
  set( fp_cxx_flags "-DFPType=${fpType} -DVTKM_DEVICE_ADAPTER=VTKM_DEVICE_ADAPTER_CUDA" )
  cuda_add_library(${pyfrLib} SHARED ${PyFR_SRCS} OPTIONS ${fp_cxx_flags})
  set_target_properties(${pyfrLib} PROPERTIES COMPILE_FLAGS ${fp_cxx_flags})

  target_link_libraries(${pyfrLib} ${MPI_LIBRARIES})
  if(MPI_COMPILE_FLAGS)
    set_target_properties(${pyfrLib} PROPERTIES
      COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
  endif()
  if(MPI_LINK_FLAGS)
    set_target_properties(${pyfrLib} PROPERTIES
      LINK_FLAGS "${MPI_LINK_FLAGS}")
  endif()
  list( APPEND PyFRLibs ${pyfrLib} )
endforeach()

install( TARGETS ${PyFRLibs} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
