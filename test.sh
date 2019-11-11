#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./kzcc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

try 0 0
try 128 128
try 20 "5+10+8-3"
try 80 "47 + 81 - 10+4 - 42"

echo OK
