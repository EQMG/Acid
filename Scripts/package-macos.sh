cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
APP=ExampleStarting
BINPATH=${SCRIPTPATH}/Build/Debug/bin
BUNDLE=${BINPATH}/${APP}.app

rm -rf ${BUNDLE}
mkdir ${BUNDLE}
mkdir ${BUNDLE}/Contents
mkdir ${BUNDLE}/Contents/MacOS
mkdir ${BUNDLE}/Contents/Resources
mkdir ${BUNDLE}/Contents/Frameworks

cp -r ${SCRIPTPATH}/Resources ${BUNDLE}/Contents/Resources
cp ${BINPATH}/${APP} ${BUNDLE}/Contents/MacOS/${APP}
touch ${BUNDLE}/Contents/MacOS/${APP}.sh
echo "#!/bin/bash" >> ${BUNDLE}/Contents/MacOS/${APP}.sh
echo "BASEDIR=`dirname $0`" >> ${BUNDLE}/Contents/MacOS/${APP}.sh
echo "open /Applications/Utilities/Terminal.app $BASEDIR/${APP}" >> ${BUNDLE}/Contents/MacOS/${APP}.sh
chmod +x ${BUNDLE}/Contents/MacOS/${APP}.sh