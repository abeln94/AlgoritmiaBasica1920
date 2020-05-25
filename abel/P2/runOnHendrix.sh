#!/bin/bash
##############################################################
# Uploads files to hendrix, executes them, downloads results #
##############################################################

# settings
SERVER=a544125@hendrix.cps.unizar.es
REMOTE_FOLDER='~/.remoteCompiler/'
UPLOAD_FILES="Makefile *.cpp *.txt"
DOWNLOAD_FILES="*.txt"
RUNNABLE="./nikita 500"

# error as soon as possible
set -e
# but say what failed
err_report() {
    echo "Error on line $1 due to error code $2"
}
trap 'err_report $LINENO $?' ERR

# create remote folder
echo ">>> CREATING REMOTE FOLDER >>>"
ssh $SERVER <<< "rm -r $REMOTE_FOLDER ; mkdir $REMOTE_FOLDER"
echo "<<< DONE <<<"
echo

# upload files
echo ">>> UPLOADING FILES >>>"
scp $UPLOAD_FILES $SERVER:$REMOTE_FOLDER
echo "<<< DONE <<<"
echo

# execute
echo ">>> EXECUTING >>>"
ssh -tt $SERVER << EOF

  set -e
  cd $REMOTE_FOLDER
  make
  $RUNNABLE
  set +e
  exit 0
EOF
echo "<<< DONE <<<"

# donwload results
echo ">>> DOWNLOADING FILES >>>"
scp $SERVER:$REMOTE_FOLDER"$DOWNLOAD_FILES" .
echo "<<< DONE <<<"
echo

# deleting remote folder
echo ">>> DELETING REMOTE FOLDER >>>"
ssh $SERVER <<< "rm -r $REMOTE_FOLDER"
echo "<<< DONE <<<"
echo

# end
echo "Exiting..."
sleep 5