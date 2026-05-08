# Scapegoat tree

## Description

This project implements a Scapegoat Tree for storing key-value pairs. Both keys and values are unsigned integers. Duplicate keys are supported, and each node stores links to its children, its parent, and the associated value.

## What it supports
- insert new elements while preserving tree order;
- remove elements by key;
- print the tree in sorted order, with an optional lower key bound;
- search for all elements with the same key;
- find the closest key that is not equal to the requested one.

### Notes
- if multiple elements share the same key, search returns all of them;
- search results are returned as pointers, not copies;
- when deleting a duplicate key, the element index must be specified.

## How to compile project

1) Clone repo

`git clone https://github.com/VtoroyChadaev/scapegoat-tree.git`

2) Go to root directory

`cd your/abs/path/scapegoat-tree`

3) Create `build` directory

`mkdir ./build`

4) Go to `project` directory

`cd ./project`

5) Run make

(a) `make lib=static`

or (to use linked library)

(b) `make lib=dynamic`

## How to run program

**IMPORTANT**

If you use dynamic library, run this way:

`LD_LIBRARY_PATH=../build/ ./program`

else

`./program`

## How to insert values to the tree

You don't have to insert values manually

Instead, you can add values using files with data

One can find examples in `./project/examples` directory

**IMPORTANT**

When inserting values manually, to **end input** you should type ASCII DLE symbol (data link escape, 16th in ASCII Table)

One can insert it using <C-p> (ctrl+p) combination

## How to test DS for time-complexity

1) Go to tests directory

`cd ./tests`

2) Create `build` directory

`mkdir ./build`

3) Run make

`make lib=static`

Some shell scripts are presented to time-test Data Structure

Results also available in txt files

**TIP**

you can examine all available options by running

`./program -h`
