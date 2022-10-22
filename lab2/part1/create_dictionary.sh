#!/bin/bash

while read -r line; do
    echo $line >> ./dictionary/$line
done < <(strings ./target | grep -E '^[A-Za-z0-9_]+$')