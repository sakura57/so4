#!/bin/bash

for i in *.hxx # or whatever other pattern...
do
  if ! grep -q Copyright $i
  then
    cat ../CopyNotice.txt $i >$i.new && mv $i.new $i
  fi
done