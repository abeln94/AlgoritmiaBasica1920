#!/bin/bash
##############################################################
# Uploads files to hendrix, executes them, downloads results #
##############################################################

# settings
SERVER=a544125@hendrix.cps.unizar.es
REMOTE_FOLDER='~/remoteCompiler/'
UPLOAD_FILES="Makefile *.cpp"
DOWNLOAD_FILES="*.csv"
RUNNABLE="./main"

# error as soon as possible
set -e
# but say what failed
err_report() {
    echo "Error on line $1 due to error code $2"
}
trap 'err_report $LINENO $?' ERR

# create remote folder
echo ">>> CREATING REMOTE FOLDER >>>"
ssh $SERVER <<< "[ -d $REMOTE_FOLDER ] || mkdir $REMOTE_FOLDER"
echo "<<< DONE <<<"
echo

# upload files
echo ">>> UPLOADING FILES >>>"
scp $UPLOAD_FILES $SERVER:$REMOTE_FOLDER
echo "<<< DONE <<<"
echo

# execute
set +e # bug: ssh returns error code 1 for unknown reasons
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
if [[ $? != 0 && $? != 1 ]]; then
  exit 1
fi
set -e

# donwload results
echo ">>> DOWNLOADING FILES >>>"
scp $SERVER:$REMOTE_FOLDER"$DOWNLOAD_FILES" .
echo "<<< DONE <<<"
echo

# end
sleep 5