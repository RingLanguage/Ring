//
// File:    io.ring
// Author:  GeneralSandman - generalsandman@163.com
// CDate:   2023-10-08
// UDate:   2023-10-08
//

package io

import {
	fmt;
}

global {

}


@native
fn exist(string path) -> (bool);

@native
fn open(string path) -> (int);

@native
fn create(string path) -> (int);

@native
fn seek(int fd, int64 offset, int whence);

@native
fn read_all(int fd) -> (string);

@native
fn write(int fd, string buffer);

@native
fn close(int fd) -> (int);

@native
fn remove(string path);

