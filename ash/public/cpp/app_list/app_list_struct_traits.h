// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_PUBLIC_CPP_APP_LIST_APP_LIST_STRUCT_TRAITS_H_
#define ASH_PUBLIC_CPP_APP_LIST_APP_LIST_STRUCT_TRAITS_H_

#include "ash/public/cpp/app_list/app_list_types.h"
#include "ash/public/cpp/ash_public_export.h"
#include "ash/public/interfaces/app_list.mojom-shared.h"
#include "base/strings/string16.h"

namespace mojo {

////////////////////////////////////////////////////////////////////////////////
// AppListState:

template <>
struct EnumTraits<ash::mojom::AppListState, ash::AppListState> {
  static ash::mojom::AppListState ToMojom(ash::AppListState input) {
    switch (input) {
      case ash::AppListState::kStateApps:
        return ash::mojom::AppListState::kStateApps;
      case ash::AppListState::kStateSearchResults:
        return ash::mojom::AppListState::kStateSearchResults;
      case ash::AppListState::kStateStart:
        return ash::mojom::AppListState::kStateStart;
      case ash::AppListState::kStateCustomLauncherPageDeprecated:
      case ash::AppListState::kInvalidState:
        break;
    }
    NOTREACHED();
    return ash::mojom::AppListState::kStateApps;
  }

  static bool FromMojom(ash::mojom::AppListState input,
                        ash::AppListState* out) {
    switch (input) {
      case ash::mojom::AppListState::kStateApps:
        *out = ash::AppListState::kStateApps;
        return true;
      case ash::mojom::AppListState::kStateSearchResults:
        *out = ash::AppListState::kStateSearchResults;
        return true;
      case ash::mojom::AppListState::kStateStart:
        *out = ash::AppListState::kStateStart;
        return true;
    }
    NOTREACHED();
    return false;
  }
};

////////////////////////////////////////////////////////////////////////////////
// AppListModelStatus:

template <>
struct EnumTraits<ash::mojom::AppListModelStatus, ash::AppListModelStatus> {
  static ash::mojom::AppListModelStatus ToMojom(ash::AppListModelStatus input) {
    switch (input) {
      case ash::AppListModelStatus::kStatusNormal:
        return ash::mojom::AppListModelStatus::kStatusNormal;
      case ash::AppListModelStatus::kStatusSyncing:
        return ash::mojom::AppListModelStatus::kStatusSyncing;
    }
    NOTREACHED();
    return ash::mojom::AppListModelStatus::kStatusNormal;
  }

  static bool FromMojom(ash::mojom::AppListModelStatus input,
                        ash::AppListModelStatus* out) {
    switch (input) {
      case ash::mojom::AppListModelStatus::kStatusNormal:
        *out = ash::AppListModelStatus::kStatusNormal;
        return true;
      case ash::mojom::AppListModelStatus::kStatusSyncing:
        *out = ash::AppListModelStatus::kStatusSyncing;
        return true;
    }
    NOTREACHED();
    return false;
  }
};

////////////////////////////////////////////////////////////////////////////////
// SearchResultType:

template <>
struct EnumTraits<ash::mojom::SearchResultType, ash::SearchResultType> {
  static ash::mojom::SearchResultType ToMojom(ash::SearchResultType input) {
    switch (input) {
      case ash::SearchResultType::kInstalledApp:
        return ash::mojom::SearchResultType::kInstalledApp;
      case ash::SearchResultType::kPlayStoreApp:
        return ash::mojom::SearchResultType::kPlayStoreApp;
      case ash::SearchResultType::kInstantApp:
        return ash::mojom::SearchResultType::kInstantApp;
      case ash::SearchResultType::kInternalApp:
        return ash::mojom::SearchResultType::kInternalApp;
      case ash::SearchResultType::kUnknown:
        break;
    }
    NOTREACHED();
    return ash::mojom::SearchResultType::kInstalledApp;
  }

  static bool FromMojom(ash::mojom::SearchResultType input,
                        ash::SearchResultType* out) {
    switch (input) {
      case ash::mojom::SearchResultType::kInstalledApp:
        *out = ash::SearchResultType::kInstalledApp;
        return true;
      case ash::mojom::SearchResultType::kPlayStoreApp:
        *out = ash::SearchResultType::kPlayStoreApp;
        return true;
      case ash::mojom::SearchResultType::kInstantApp:
        *out = ash::SearchResultType::kInstantApp;
        return true;
      case ash::mojom::SearchResultType::kInternalApp:
        *out = ash::SearchResultType::kInternalApp;
        return true;
    }
    NOTREACHED();
    return false;
  }
};

////////////////////////////////////////////////////////////////////////////////
// SearchResultDisplayType:

template <>
struct EnumTraits<ash::mojom::SearchResultDisplayType,
                  ash::SearchResultDisplayType> {
  static ash::mojom::SearchResultDisplayType ToMojom(
      ash::SearchResultDisplayType input) {
    switch (input) {
      case ash::SearchResultDisplayType::kNone:
        return ash::mojom::SearchResultDisplayType::kNone;
      case ash::SearchResultDisplayType::kList:
        return ash::mojom::SearchResultDisplayType::kList;
      case ash::SearchResultDisplayType::kTile:
        return ash::mojom::SearchResultDisplayType::kTile;
      case ash::SearchResultDisplayType::kRecommendation:
        return ash::mojom::SearchResultDisplayType::kRecommendation;
      case ash::SearchResultDisplayType::kCard:
        return ash::mojom::SearchResultDisplayType::kCard;
      case ash::SearchResultDisplayType::kLast:
        break;
    }
    NOTREACHED();
    return ash::mojom::SearchResultDisplayType::kNone;
  }

  static bool FromMojom(ash::mojom::SearchResultDisplayType input,
                        ash::SearchResultDisplayType* out) {
    switch (input) {
      case ash::mojom::SearchResultDisplayType::kNone:
        *out = ash::SearchResultDisplayType::kNone;
        return true;
      case ash::mojom::SearchResultDisplayType::kList:
        *out = ash::SearchResultDisplayType::kList;
        return true;
      case ash::mojom::SearchResultDisplayType::kTile:
        *out = ash::SearchResultDisplayType::kTile;
        return true;
      case ash::mojom::SearchResultDisplayType::kRecommendation:
        *out = ash::SearchResultDisplayType::kRecommendation;
        return true;
      case ash::mojom::SearchResultDisplayType::kCard:
        *out = ash::SearchResultDisplayType::kCard;
        return true;
    }
    NOTREACHED();
    return false;
  }
};

////////////////////////////////////////////////////////////////////////////////
// SearchResultTag:

template <>
struct StructTraits<ash::mojom::SearchResultTagDataView, ash::SearchResultTag> {
  static int styles(const ash::SearchResultTag& tag) { return tag.styles; }
  static const gfx::Range& range(const ash::SearchResultTag& tag) {
    return tag.range;
  }
  static bool Read(ash::mojom::SearchResultTagDataView data,
                   ash::SearchResultTag* out);
};

////////////////////////////////////////////////////////////////////////////////
// SearchResultActionLabel:

template <>
struct UnionTraits<ash::mojom::SearchResultActionLabelDataView,
                   ash::SearchResultAction> {
  static ash::mojom::SearchResultActionLabelDataView::Tag GetTag(
      const ash::SearchResultAction& action);

  static bool Read(ash::mojom::SearchResultActionLabelDataView data,
                   ash::SearchResultAction* out);
};

////////////////////////////////////////////////////////////////////////////////
// SearchResultAction:

template <>
struct StructTraits<ash::mojom::SearchResultActionDataView,
                    ash::SearchResultAction> {
  static bool Read(ash::mojom::SearchResultActionDataView data,
                   ash::SearchResultAction* out);

  static const base::string16& tooltip_text(
      const ash::SearchResultAction& action) {
    return action.tooltip_text;
  }

  static const ash::SearchResultAction& label(
      const ash::SearchResultAction& action) {
    return action;
  }
};

}  // namespace mojo

#endif  // ASH_PUBLIC_CPP_APP_LIST_APP_LIST_STRUCT_TRAITS_H_
