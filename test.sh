#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./kzcc "$input" > tmp.s
  gcc tmp.s -o tmp
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 42 '42'

try 18 '9-4+13'

try 18 ' 9 - 4 + 13 '

try 47 "5+6*7"

try 15 "5*(9-6)"

try 4 "(3+5)/2"

echo OK
