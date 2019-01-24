#!/usr/bin/env bash
cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

if [ -d "${SCRIPTPATH}/cmake-build-debug/bin" ]; then
  mkdir "${SCRIPTPATH}/cmake-build-debug/bin/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/cmake-build-debug/bin/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/cmake-build-release/bin" ]; then
  mkdir "${SCRIPTPATH}/cmake-build-release/bin/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/cmake-build-debug/bin/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/Build/bin" ]; then
  mkdir "${SCRIPTPATH}/Build/bin/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/bin32/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/Build/bin32" ]; then
  mkdir "${SCRIPTPATH}/Build/bin32/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/bin32/Resources/Engine"
fi
