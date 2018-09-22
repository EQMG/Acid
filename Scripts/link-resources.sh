#!/usr/bin/env bash
cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

if [ -d "${SCRIPTPATH}/Build/Debug/Bin/Resources" ]; then
  mkdir "${SCRIPTPATH}/Build/Debug/Bin/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/Debug/Bin/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/Build/Debug/Bin32/Resources" ]; then
  mkdir "${SCRIPTPATH}/Build/Debug/Bin32/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/Debug/Bin32/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/Build/Release/Bin/Resources" ]; then
  mkdir "${SCRIPTPATH}/Build/Release/Bin/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/Release/Bin/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/Build/Release/Bin32/Resources" ]; then
  mkdir "${SCRIPTPATH}/Build/Release/Bin32/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/Release/Bin32/Resources/Engine"
fi

if [ -d "${SCRIPTPATH}/Build/RelWithDebInfo/Bin/Resources" ]; then
  mkdir "${SCRIPTPATH}/Build/RelWithDebInfo/Bin/Resources"
  ln -s "${SCRIPTPATH}/Resources" "${SCRIPTPATH}/Build/RelWithDebInfo/Bin/Resources/Engine"
fi
