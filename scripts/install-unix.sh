#!/bin/bash

# ... existing code ...

# 优化系统检测逻辑
detect_platform() {
    local os=$(uname -s | tr '[:upper:]' '[:lower:]')
    local arch=$(uname -m)

    # 架构转换
    case $arch in
        x86_64) arch="amd64" ;;
        aarch64) arch="arm64" ;;
        i386) arch="386" ;;
        *) error "Unsupported architecture: $arch" ;;
    esac

    echo "$os-$arch"
}

# 动态获取最新版本
get_latest_version() {
    curl -sSL https://ring.wiki/api/latest-version
}

# 主安装流程
main() {
    local version=${1:-$(get_latest_version)}
    local platform=$(detect_platform)
    local filename="ring-v${version}-${platform}.tar.gz"
    local url="https://ring.wiki/package/${filename}"
    local tmp_dir=$(mktemp -d)

    info_bold "Installing Ring $version for $platform..."

    # 下载安装包
    if ! curl -fSL --retry 3 --output "${tmp_dir}/${filename}" "$url"; then
        error "Failed to download $url"
    fi

    # 解压安装
    mkdir -p "${tmp_dir}/ring"
    if ! tar -zxf "${tmp_dir}/${filename}" -C "${tmp_dir}/ring"; then
        error "Failed to extract ${filename}"
    fi

    # 安装到系统
    local install_prefix=${RING_HOME:-/usr/local}
    mkdir -p "${install_prefix}/bin" "${install_prefix}/lib/ring/std"
    install -m 0755 "${tmp_dir}/ring/bin/ring" "${install_prefix}/bin/"
    cp -r "${tmp_dir}/ring/std/" "${install_prefix}/lib/ring/"

    # 清理临时文件
    rm -rf "$tmp_dir"

    success "Ring $version installed successfully to ${install_prefix}/bin"
    echo "To get started, run: ring --version"
}

# ... existing code ...