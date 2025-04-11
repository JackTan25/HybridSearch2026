#!/bin/bash

echo "#    - name: install pysdk"
sudo docker exec hybridsearch_build bash -c "cd /hybridsearch/python && python3 setup.py install"
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi

echo "#    - name: start hybridsearch debug version"
sudo docker exec hybridsearch_build bash -c "cd /hybridsearch/ && rm -fr /var/hybridsearch && cmake-build-debug/src/hybridsearch > debug.log 2>&1" &
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi

echo "#    - name: pysdk & sqllogictest debug version"
sudo docker exec hybridsearch_build bash -c "cd /hybridsearch/ && python3 tools/sqllogictest.py"
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi

echo "#    - name: stop hybridsearch debug version"
sudo kill `pidof hybridsearch`
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi

echo "#    - name: start hybridsearch release version"
sudo docker exec hybridsearch_build bash -c "cd /hybridsearch/ && rm -fr /var/hybridsearch && cmake-build-release/src/hybridsearch > debug.log 2>&1" &
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi

echo "#    - name: pysdk & sqllogictest release version"
sudo docker exec hybridsearch_build bash -c "cd /hybridsearch/ && python3 tools/sqllogictest.py"
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi

echo "#    - name: stop hybridsearch release version"
sudo kill `pidof hybridsearch`
if [ $? -ne 0 ] ; then
    echo "### failure! $? ###"
    exit 1
fi
