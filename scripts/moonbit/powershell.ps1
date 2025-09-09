#!/usr/bin/env pwsh
#
# Moonbit 编程语言 Windows 安装脚本
# 功能：在 Windows 系统上自动下载、安装和配置 Moonbit 开发环境
# 支持平台：x86 64-bit Windows
# 使用方法：.\powershell.ps1 或通过环境变量设置版本
#

# 确定要安装的版本
# 优先从环境变量 MOONBIT_INSTALL_VERSION 获取，否则使用 'latest'
if ($env:MOONBIT_INSTALL_VERSION) {
  $Version = $env:MOONBIT_INSTALL_VERSION
} else {
  $Version = "latest"
}

# 替换版本号中的 '+' 字符为 URL 编码的 '%2B'
$Version = $Version -replace '\+', '%2B'

# 检查系统架构是否为 x86 64-bit
if ($env:PROCESSOR_ARCHITECTURE -ne "AMD64") {
  Write-Output "Install Failed:"
  Write-Output "MoonBit for Windows is currently only available for x86 64-bit Windows.`n"
  exit 1
}

# 设置错误处理偏好为停止执行
$ErrorActionPreference = "Stop"

# 确定安装路径
# 优先从环境变量 MOON_HOME 获取，否则使用默认路径 $HOME\.moon
if ($env:MOON_HOME) {
  $MoonHome = $env:MOON_HOME
} else {
  $MoonHome = "${HOME}\.moon"
}

# 定义二进制文件目录和库目录
$MoonBin = "${MoonHome}\bin"
$MoonLib = "${MoonHome}\lib"

# Moonbit CLI 下载基础 URL
$CLI_MOONBIT = "https://cli.moonbitlang.com"

# 构建下载 URL
# 如果设置了开发版标志，则使用开发版
if ($env:MOONBIT_INSTALL_DEV) {
  $MoonbitUri = "$CLI_MOONBIT/binaries/$Version/moonbit-windows-x86_64-dev.zip"
} else {
  $MoonbitUri = "$CLI_MOONBIT/binaries/$Version/moonbit-windows-x86_64.zip"
}

# 核心库下载 URL
$CoreUri = "$CLI_MOONBIT/cores/core-$Version.zip"


try {
  # 创建安装目录（如果不存在）
  if (-not (Test-Path -Path $MoonHome -PathType Container)) {
    New-Item -Path $MoonHome -ItemType Directory
  }

  # 下载 Moonbit 二进制文件
  Write-Output "Downloading moonbit ..."
  Invoke-WebRequest -Uri $MoonbitUri -OutFile "${HOME}\moonbit.zip"

  # 清理旧的安装文件
  if (Test-Path -Path "$MoonHome\bin" -PathType Container) {
    Remove-Item -Force -Recurse "$MoonHome\bin"
  }
  if (Test-Path -Path "$MoonHome\lib" -PathType Container) {
    Remove-Item -Force -Recurse "$MoonHome\lib"
  }
  if (Test-Path -Path "$MoonHome\include" -PathType Container) {
    Remove-Item -Force -Recurse "$MoonHome\include"
  }

  # 解压二进制文件
  Expand-Archive "${HOME}\moonbit.zip" -DestinationPath $MoonHome -Force

  # 删除下载的压缩包
  Remove-Item -Force "${HOME}\moonbit.zip"

  # 下载核心库
  Write-Output "Downloading core ..."
  if (Test-Path -Path $MoonLib\core) {
    Remove-Item -Force -Recurse $MoonLib\core
  }

  # 根据版本类型处理核心库
  if ($Version -eq "bleeding") {
    # 对于开发版，从 GitHub 克隆
    git clone --depth 1 https://github.com/moonbitlang/core.git "$MoonLib\core"
    if ($LASTEXITCODE -ne 0) {
        Write-Output "Install Failed:"
        Write-Output "Failed to clone core from github"
        exit 1
    }
  } else {
    # 对于普通版本，从服务器下载
    Invoke-WebRequest -Uri $CoreUri -OutFile $MoonHome\core.zip
    Expand-Archive $MoonHome\core.zip -DestinationPath $MoonLib -Force
    Remove-Item -Force $MoonHome\core.zip
  }

  # 打包核心库
  Write-Output "Bundling core ..."
  Push-Location $MoonLib\core

  # 临时修改 PATH 环境变量以使用刚安装的 moon 命令
  $OldPath = $env:Path
  $env:Path = $MoonBin
  moon.exe bundle --warn-list -a --all
  moon.exe bundle --warn-list -a --target wasm-gc --quiet
  $env:PATH = $OldPath

  # 恢复之前的工作目录
  Pop-Location
} catch {
  # 捕获并显示错误信息
  Write-Output "Install Failed:"
  Write-Output $_.Exception.Message
  exit 1
}

# 配置环境变量
# 检查 MoonBin 是否已在 PATH 中
if ($env:Path -split ';' -notcontains $MoonBin) {
  # 获取当前用户的 PATH 环境变量
  $currentPath = [System.Environment]::GetEnvironmentVariable("PATH", [System.EnvironmentVariableTarget]::User)
  # 添加 MoonBin 到 PATH
  [System.Environment]::SetEnvironmentVariable("PATH", "$MoonBin;$currentPath", [System.EnvironmentVariableTarget]::User)

  Write-Output "Added ~/.moon/bin to the PATH."
  Write-Output "Please restart your terminal to use moonbit."
} else {
  Write-Output "Moonbit installed successfully."
}

# 显示验证和帮助信息
Write-Output "To verify the download binaries, check https://www.moonbitlang.com/download#verifying-binaries for instructions."
Write-Output "To know how to add shell completions, run 'moon shell-completion --help'"