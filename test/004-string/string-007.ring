
//
// File:    string-003.ring
// Author:  GeneralSandman - generalsandman@163.com
// Date:    2022-10-20
//

package main

import {
	fmt;
	debug;
  vm;
}

// TestDetail: 测试多行字符串

fn main() {
    var string string_value;

    string_value = `
    ring-v0.3.1-beta Copyright (C) 2021-2025 ring.wiki, ZhenhuLi
    
    // 李振虎
    // English test
    // اختبار اللغة العربية
    // тест русского языка
    // परीक्षण हिंदी भाषा
    // テスト日本語
    // 테스트 한국어
    // परीक्षण नेपाली भाषा
    // δοκιμή ελληνικών
    // טעסץ ייִדיש
    `;
    fmt::println_string(string_value);


    string_value = "123\n456\n789";

}
