#!/bin/bash

UNAME=`uname`

MACOS_RELEASE="/Ring.tar.gz"
LINUX_RELEASE="/Ring.tar.gz"
WINDOWS_RELEASE="/Ring.tar.gz"


if [[ "$UNAME" = "Linux" ]]; then
    echo "Linux"
elif [[ "$UNAME" = "Darwin" ]]; then
    echo "Darwin"
elif [[ "$UNAME" = "Windows" ]]; then
    echo "Windows"
else 
    echo "Unknow Operating System"
    exit 1
fi


VERSION=""

wget https://ring.wiki/package/ring-v1.0.0-linux-386.tar.gz

echo "\033[32m[+]Download Ring Package Success\033[0m"

tar -zxvf ring-v1.0.0-linux-386.tar.gz
echo "\033[32m[+]Decompression Ring Package Success\033[0m"

cd Ring


INSTALL_TOP="/usr/local
INSTALL_BIN= $(INSTALL_TOP)/bin
INSTALL_PACK_STD = $(INSTALL_TOP)/lib/ring/std

CMD_INSTALL= install -p
CMD_INSTALL_EXEC= install -p -m 0755
CMD_INSTALL_DATA= install -p -m 0644


ring version
echo "\033[32m[+]Check Ring Version Success\033[0m"


install -p -m 0755 ring /usr/local/bin


# 获取版本号参数
get_version() {
    local version_arg=$1
    if [[ -n "$version_arg" ]]; then
        echo "$version_arg"
    else
        # 从latest版本文件获取最新版本号
        curl -sSL https://ring.wiki/latest-version
    fi
}

# 主安装流程
main() {
    local requested_version=$1
    local version=$(get_version "$requested_version")
    local base_url="https://ring.wiki"
    
    # 根据是否指定版本选择不同的基础URL
    if [[ -n "$requested_version" ]]; then
        base_url="${base_url}/${version}"
    else
        base_url="${base_url}/latest"
    fi
    
    local platform=$(detect_platform)
    local filename="ring-v${version}-${platform}.tar.gz"
    local url="${base_url}/package/${filename}"
    
    # ... 下载和安装逻辑 ...
}

# 执行主函数
main "$@"