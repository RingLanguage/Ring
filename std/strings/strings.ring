//
// File:    time.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2023-04-14
//

package strings

import {

}

// join concatenates the elements of its first argument to create a single string. The separator
// string sep is placed between elements in the resulting string.
fn join(var string[] elems, var string sep) -> (string) {
	var string result;

	var int i = 0;

	for i = 0; i < len(elems); i++ {
		result = result + elems[i];
		if i != len(elems) - 1 {
			result = result + sep;
		}
	}
	
	return result;
}

// has_prefix reports whether the string s begins with prefix.
fn has_prefix(var string s, var string prefix) -> (bool) {
	if len(s) < len(prefix) {
		return false;
	}
	return s[0:len(prefix)] == prefix;
}

// has_suffix reports whether the string s ends with suffix.
fn has_suffix(var string s, var string suffix) -> (bool) {
	if len(s) < len(suffix) {
		return false;
	}
	return s[len(s)-len(suffix):] == suffix;
}

// trim_prefix returns s without the provided leading prefix string.
// If s doesn't start with prefix, s is returned unchanged.
fn trim_prefix(var string s, var string prefix) -> (string) {
	if has_prefix(s, prefix) {
		return s[len(prefix):];
	}
	return s;
}

// trim_suffix returns s without the provided trailing suffix string.
// If s doesn't end with suffix, s is returned unchanged.
fn trim_suffix(var string s, var string suffix) -> (string) {
	if has_suffix(s, suffix) {
		return s[:len(s)-len(suffix)];
	}
	return s;
}
