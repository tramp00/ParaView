/******************************************************************************
 * Copyright 2019 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/
/// \file
/// \brief  Interfaces for error feedback from the NVIDIA IndeX library runtime.

#ifndef NVIDIA_INDEX_IERROR_H
#define NVIDIA_INDEX_IERROR_H

#include <mi/base/interface_declare.h>
#include <mi/base/types.h>
#include <mi/dice.h>

namespace nv
{
namespace index
{

/// @ingroup nv_index_error
/// The base class of all errors generated by NVIDIA IndeX. All errors contain
/// information about the host and potentially the GPU device that caused the
/// error. Additionally, each error provides a descriptive error string detailing
/// the error type and possible causes for the failure.
///
class IError : public mi::base::Interface_declare<0x27d2eff4, 0xcb29, 0x4452, 0x84, 0x6e, 0x4f,
                 0xa7, 0x9, 0xa3, 0x12, 0x6c>
{
public:
  /// Returns the interface ID corresponding to the actual derived error class.
  /// This information allows for a straightforward conversion of a general
  /// \c IError interface instance to the specialized derived interface type.
  ///
  /// \return         The interface ID of the derived error class.
  ///
  virtual mi::base::Uuid get_error_uuid() const = 0;

  /// Returns a descriptive string detailing the error type and possible causes
  /// for the failure. The string summarizes all available information including
  /// the host ID and possibly the GPU device ID causing the failure.
  ///
  /// \return         The error string describing the runtime failure.
  ///
  virtual const char* get_error_string() const = 0;

  /// Returns the host ID of the cluster machine causing the generation of the
  /// runtime error.
  ///
  /// \return         The host ID of the failing NVIDIA IndeX call or process.
  ///
  virtual mi::Uint32 get_host_id() const = 0;

  /// Returns the device ID of the GPU device on a specific cluster machine causing
  /// the generation of the runtime error. If the error is not related to a
  /// GPU-processing failure an invalid ID is returned with the value \c 0xffffffffu (or. \c ~0u);
  ///
  /// \return         The device ID of GPU-device causing the failure of a
  ///                 NVIDIA IndeX call or process (\c 0xffffffffu marks an
  ///                 invalid device ID).
  ///
  virtual mi::Uint32 get_device_id() const = 0;
};

/// @ingroup nv_index_error
/// The base class for out-of-memory errors generated by NVIDIA IndeX. This class of
/// errors contains additional information regarding the available memory resources
/// and the size of the memory allocation request causing the runtime failure. The
/// specializations of this class \c IHost_out_of_memory_error and \c IDevice_out_of_memory_error
/// indicate the host or GPU-device memory system as the source of the resource
/// allocation failure.
///
class IOut_of_memory_error : public mi::base::Interface_declare<0x565cc068, 0xd7c9, 0x4c3e, 0x85,
                               0x38, 0x77, 0xca, 0x44, 0x6b, 0x23, 0x65, nv::index::IError>
{
public:
  enum Failure_hint
  {
    OOM_FAIL_HINT_NONE = 0x00u,
    OOM_FAIL_INVALID_ALLOC_REQUEST = 0x01u,
    OOM_FAIL_FRAGMENTED_MEMORY = 0x02u,
    OOM_FAIL_INSUFFICIENT_DYNAMIC_RESOURCES = 0x04u
  };

  /// Returns the amount of memory available on the specific memory system (i.e. host
  /// GPU device) on which the resource allocation failure was detected.
  ///
  /// \return         Returns the amount of memory available on the specific memory system.
  ///
  virtual mi::Size get_memory_available() const = 0;

  /// Returns the amount of memory requested from the specific memory system (i.e. host
  /// GPU device) which caused the resource allocation failure.
  ///
  /// \return         Returns the amount of memory requested from the specific memory system.
  ///
  virtual mi::Size get_memory_requested() const = 0;

  /// Returns a bitmask describing potential reasons of the memory allocation failure.
  ///
  /// \return         Returns a bitmask containing one or more of the \c Failure_hint constants.
  ///
  virtual mi::Uint32 get_failure_hints() const = 0;
};

/// @ingroup nv_index_error
/// This class specializes the generic out-of-memory error interface (\c IOut_of_memory_error)
/// to indicate the host memory system as the source of the resource allocation failure.
///
class IHost_out_of_memory_error
  : public mi::base::Interface_declare<0x573349e3, 0x5f5c, 0x4951, 0xab, 0x97, 0xc3, 0xd1, 0x13,
      0xcd, 0xe7, 0x2c, nv::index::IOut_of_memory_error>
{
};

/// @ingroup nv_index_error
/// This class specializes the generic out-of-memory error interface (\c IOut_of_memory_error)
/// to indicate the GPU-device memory system as the source of the resource allocation failure.
///
class IDevice_out_of_memory_error
  : public mi::base::Interface_declare<0x5662d3ad, 0x3311, 0x49ca, 0xb2, 0xce, 0x9e, 0x93, 0x67,
      0xad, 0x68, 0x86, nv::index::IOut_of_memory_error>
{
};

/// @ingroup nv_index_error
/// This error interface specialization indicates a failure to execute a GPU-device kernel at
/// runtime. The causes for such errors suggest invalid memory accesses and will invalidate
/// all device memory preventing further use of the associated GPU device of the current
/// rendering process.
///
/// \note The use of the \c IConfig_settings option \c set_cuda_debug_checks_enabled() to enable
///       additional runtime checks for failing device executions can offer more detailed
///       information to the actual cause of the runtime failure. Such information can prove
///       vital for the NVIDIA IndeX development team for reproducing and fixing potential
///       runtime issues.
///
class IDevice_execution_error : public mi::base::Interface_declare<0x9db4beec, 0x1d8c, 0x4a5f, 0x86,
                                  0x14, 0xd6, 0x7c, 0x9d, 0xa1, 0x98, 0x5d, nv::index::IError>
{
};

/// @ingroup nv_index_error
/// This error interface specialization indicates general runtime failures. The provided
/// error string will give more details about the runtime failure.
///
class IRuntime_error : public mi::base::Interface_declare<0x6565399f, 0x99ea, 0x480f, 0xb4, 0x44,
                         0x49, 0xdc, 0xc0, 0x8f, 0x96, 0x87, nv::index::IError>
{
};

/// @ingroup nv_index_error
/// This class serves as a collection of runtime errors potentially generated during
/// the execution of distributed NVIDIA IndeX processes such as multi-GPU or cluster-based
/// rendering.
///
class IError_set : public mi::base::Interface_declare<0x5edf4263, 0x4f89, 0x42de, 0xa3, 0xf8, 0xae,
                     0x4f, 0x1e, 0xf0, 0x75, 0x58>
{
public:
  /// This method allows for an quick and easy check if the returning NVIDIA IndeX call
  /// was successful or failed with the error set containing generated errors.
  ///
  /// \return         Returns \c true if the error set is containing errors, \c false otherwise.
  ///
  virtual bool any_errors() const = 0;

  /// Returns the number of error interface instances contained in the error set.
  ///
  /// \return         Returns the number of the contained runtime error instances.
  ///
  virtual mi::Uint32 get_nb_errors() const = 0;

  /// Allows access to single instances of \c IError interfaces contained in the error set.
  ///
  /// \note Use the \c mi::base::Handle template to store the returned interface pointer in order
  ///       to guarantee the correct destruction of the returned interface.
  ///
  /// \param[in] idx  Index of the requested \c IError interface instance.
  ///
  /// \return         Returns interface pointer to the requested error instance or a null-pointer
  ///                 to indicate an invalid access index.
  ///
  virtual IError* get_error(mi::Uint32 idx) const = 0;
};
}
} // namespace index / nv

#endif // NVIDIA_INDEX_IERROR_H
