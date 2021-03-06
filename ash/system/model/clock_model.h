// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_MODEL_CLOCK_MODEL_H_
#define ASH_SYSTEM_MODEL_CLOCK_MODEL_H_

#include "base/i18n/time_formatting.h"
#include "base/macros.h"
#include "base/observer_list.h"
#include "chromeos/dbus/system_clock_client.h"
#include "chromeos/settings/timezone_settings.h"

namespace ash {

class ClockObserver;

// Model to notify system clock and related configuration change.
class ClockModel : public chromeos::SystemClockClient::Observer,
                   public chromeos::system::TimezoneSettings::Observer {
 public:
  ClockModel();
  ~ClockModel() override;

  void AddObserver(ClockObserver* observer);
  void RemoveObserver(ClockObserver* observer);

  void SetUse24HourClock(bool use_24_hour);

  // Force observers to refresh clock views e.g. system is resumed or timezone
  // is changed.
  void NotifyRefreshClock();

  // chromeos::SystemClockClient::Observer:
  void SystemClockUpdated() override;
  void SystemClockCanSetTimeChanged(bool can_set_time) override;

  // chromeos::system::TimezoneSettings::Observer:
  void TimezoneChanged(const icu::TimeZone& timezone) override;

  base::HourClockType hour_clock_type() const { return hour_clock_type_; }

  bool can_set_time() { return can_set_time_; }

 private:
  void NotifyDateFormatChanged();
  void NotifySystemClockTimeUpdated();
  void NotifySystemClockCanSetTimeChanged(bool can_set_time);

  // The type of clock hour display: 12 or 24 hour.
  base::HourClockType hour_clock_type_;

  // If system clock can be configured by user through SetTimeDialog.
  bool can_set_time_;

  base::ObserverList<ClockObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(ClockModel);
};

}  // namespace ash

#endif  // ASH_SYSTEM_MODEL_CLOCK_MODEL_H_
