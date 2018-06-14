#!/usr/bin/env bash
cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
mkdir ${SCRIPTPATH}/Build/Debug/bin/Resources
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/Debug/bin/Resources/Engine
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/Release/bin/Resources/Engine
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/RelWithDebInfo/bin/Resources/Engine
