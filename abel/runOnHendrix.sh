#!/bin/bash

set -e # error as soon as possible
# Uploads files to hendrix, executes them, downloads results

SERVER=a544125@hendrix.cps.unizar.es
REMOTE_FOLDER='~/remoteCompiler/'
UPLOAD_FILES="Makefile *.cpp"
DOWNLOAD_FILES="*.csv"
RUNNABLE="./main"

echo ">>> CREATING REMOTE FOLDER >>>"
ssh $SERVER <<< "[ -d $REMOTE_FOLDER ] || mkdir $REMOTE_FOLDER"
echo "<<< DONE <<<"
echo

echo ">>> UPLOADING FILES >>>"
scp $UPLOAD_FILES $SERVER:$REMOTE_FOLDER
echo "<<< DONE <<<"
echo

echo ">>> EXECUTING >>>"
ssh -tt $SERVER << EOF

  set -e
  cd $REMOTE_FOLDER
  make
  $RUNNABLE
  exit 0
EOF
echo "<<< DONE <<<"
echo

echo ">>> DOWNLOADING FILES >>>"
scp $SERVER:$REMOTE_FOLDER"$DOWNLOAD_FILES" .
echo "<<< DONE <<<"
echo

sleep 5