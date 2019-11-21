/*
 *  recording_device.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef RECORDING_DEVICE_H
#define RECORDING_DEVICE_H

// C++ includes:
#include <fstream>
#include <vector>

// Includes from nestkernel:
#include "node.h"
#include "device.h"
#include "device_node.h"
#include "recording_backend.h"
#include "nest_types.h"
#include "kernel_manager.h"

// Includes from sli:
#include "dictdatum.h"
#include "dictutils.h"

namespace nest
{

/**
 * Base class for all recording devices.
 *
 * Recording devices collect or sample data and output it to one or
 * more recording backends selected by setting the device property
 * `record_to` to the name of the backend.
 *
 * Class RecordingDevice is merely a shallow interface class from
 * which concrete recording devices can inherit in order to use the
 * recording backend infrastructure more easily and provide a
 * consistent handling of activity windows by means of start/stop and
 * origin.
 *
 * If the device is configured to record from start to stop, this
 * is interpreted as (start, stop], i.e., the earliest recorded
 * event will have time stamp start+1, as it was generated during
 * the update step (start, start+1].
 *
 * If the device node is not an actual instance used by the user, but
 * rather a prototype node in a Model class, it will cache
 * device-specific properties of the recording backend and use them
 * for enrollment of the device with the backend as the last step
 * during the creation of instances. This mechanism is implemented in
 * set_status() and set_initialized_().
 *
 * @ingroup Devices
 *
 * @author HEP 2002-07-22, 2008-03-21, 2011-02-11
 */

class RecordingDevice : public DeviceNode, public Device
{
public:
  RecordingDevice();
  RecordingDevice( const RecordingDevice& );

  void calibrate( const std::vector< Name >&, const std::vector< Name >& );

  bool is_active( Time const& T ) const;

  enum Type
  {
    MULTIMETER,
    SPIKE_DETECTOR,
    SPIN_DETECTOR,
    WEIGHT_RECORDER
  };

  virtual Type get_type() const = 0;

  const std::string& get_label() const;

  void set_status( const DictionaryDatum& );
  void get_status( DictionaryDatum& ) const;

protected:
  void write( const Event&, const std::vector< double >&, const std::vector< long >& );
  void set_initialized_() override;

private:
  struct Parameters_
  {
    std::string label_; //!< A user-defined label for symbolic device names.
    Name record_to_;    //!< The name of the recording backend to use

    Parameters_();
    Parameters_( const Parameters_& );
    void get( DictionaryDatum& ) const;
    void set( const DictionaryDatum& );
  } P_;

  struct State_
  {
    size_t n_events_; //!< The number of events recorded by the device.

    State_();
    void get( DictionaryDatum& ) const;
    void set( const DictionaryDatum& );
  } S_;

  DictionaryDatum backend_params_;
};

} // namespace

#endif // RECORDING_DEVICE_H
