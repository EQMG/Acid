#!/usr/bin/env bash
cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/Debug/bin/Resources
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/Release/bin/Resources
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/RelWithDebInfo/bin/Resources
