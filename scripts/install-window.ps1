<#
.SYNOPSIS
Installs Ring programming language on Windows

.DESCRIPTION
Automatically downloads and installs the latest Ring version for your architecture
#>

$ErrorActionPreference = "Stop"

# 检测系统架构
function Detect-Architecture {
    $arch = $Env:PROCESSOR_ARCHITECTURE
    if ($arch -eq "AMD64") { return "amd64" }
    if ($arch -eq "x86") { return "386" }
    throw "Unsupported architecture: $arch"
}

# 获取最新版本
function Get-LatestVersion {
    Invoke-RestMethod -Uri "https://ring.wiki/api/latest-version"
}

# 主安装流程
function Install-Ring {
    param([string]$Version)

    $platform = "windows-$(Detect-Architecture)"
    $version = if ($Version) { $Version } else { Get-LatestVersion }
    $filename = "ring-v${version}-${platform}.zip"
    $url = "https://ring.wiki/package/${filename}"
    $tmpDir = Join-Path $Env:TEMP "ring-install"
    $installDir = if ($Env:RING_HOME) { $Env:RING_HOME } else { "$Env:USERPROFILE\.ring" }

    Write-Host "Installing Ring $version for $platform..."

    # 创建临时目录
    New-Item -ItemType Directory -Path $tmpDir | Out-Null

    try {
        # 下载安装包
        Invoke-WebRequest -Uri $url -OutFile "$tmpDir\$filename"

        # 解压安装
        Expand-Archive -Path "$tmpDir\$filename" -DestinationPath $tmpDir

        # 创建安装目录
        New-Item -ItemType Directory -Path (Join-Path $installDir "bin") | Out-Null
        New-Item -ItemType Directory -Path (Join-Path $installDir "lib
ing\std") | Out-Null

        # 复制文件
        Copy-Item -Path "$tmpDir
ingin
ing.exe" -Destination (Join-Path $installDir "bin")
        Copy-Item -Path "$tmpDir
ing\std\*" -Destination (Join-Path $installDir "lib
ing\std") -Recurse

        # 添加到环境变量
        $path = [Environment]::GetEnvironmentVariable("PATH", "User")
        if (-not $path.Contains("$installDirin")) {
            [Environment]::SetEnvironmentVariable("PATH", "$path;$installDirin", "User")
            Write-Host "Added $installDirin to user PATH"
            Write-Host "Please restart your terminal for changes to take effect"
        }

        Write-Host "Ring $version installed successfully!"
        Write-Host "To get started, run: ring --version"
    }
    finally {
        # 清理临时文件
        Remove-Item -Path $tmpDir -Recurse -Force
    }
}

# 执行安装
Install-Ring @args