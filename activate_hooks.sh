#!/bin/bash
rm -rf .git/hooks
cd .git
ln -s ../hooks hooks
