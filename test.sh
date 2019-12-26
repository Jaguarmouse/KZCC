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

try 0 "0;"
try 128 "128;"
try 20 "5+10+8-3;"
try 80 "47 + 81 - 10+4 - 42;"
try 4 '10-3*2;'
try 8  '9/3*2+4/2;'
try 18 '(10-4)*(3*4/(3*2-2));'
try 38 '-10 * + 3 + (11 * 6 + + 2);'
try 0 '8 > 8;'
try 1 '10 <= 11;'
try 1 '10 == 10;'
try 0 '10 != 10;'
try 0 '4+3;8-4*2;'
try 9 'aboon = 3; nemo = 5;aboon+nemo+1;'
try 6 'a=b=3;(a+3*b)/2;'
try 6 'return 6;'

echo OK
