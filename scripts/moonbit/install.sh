 #!/bin/bash
#
# Moonbit 编程语言安装脚本
# 功能：自动下载、安装和配置 Moonbit 开发环境
# 支持平台：macOS (x86_64/arm64)、Linux (x86_64)
# 使用方法：./install.sh [版本号] 或 ./install.sh --help
#

# 颜色和样式定义
Color_Off=''      # 重置颜色
Red=''            # 红色
Green=''          # 绿色
Dim=''            # 暗白色
Bold_White=''     # 粗体白色
Bold_Green=''     # 粗体绿色

# 检查是否在终端中运行，如果是则启用颜色
if [[ -t 1 ]]; then
    # 重置
    Color_Off='\033[0m' # 文本重置

    # 常规颜色
    Red='\033[0;31m'   # 红色
    Green='\033[0;32m' # 绿色
    Dim='\033[0;2m'    # 暗白色

    # 粗体颜色
    Bold_Green='\033[1;32m' # 粗体绿色
    Bold_White='\033[1m'    # 粗体白色
fi

# 错误处理函数
# 参数：错误信息
# 功能：以红色显示错误信息并退出脚本
error() {
  echo -e "${Red}error${Color_Off}: $*" >&2
  exit 1
}

# 信息输出函数
# 参数：信息内容
# 功能：以暗白色显示普通信息
info() {
  echo -e "${Dim}$* ${Color_Off}"
}

# 粗体信息输出函数
# 参数：信息内容
# 功能：以粗体白色显示重要信息
info_bold() {
  echo -e "${Bold_White}$* ${Color_Off}"
}

# 成功信息输出函数
# 参数：信息内容
# 功能：以绿色显示成功信息
success() {
  echo -e "${Green}$* ${Color_Off}"
}

# 初始化参数数组
ARGUMENTS=()

# 解析命令行参数
for i in "$@"; do
  case "$i" in
    -h|--help)  # 帮助参数
      HELP=true
      ;;
    -*)         # 未知选项
      error "Unknown option: $i"
      ;;
    *)          # 版本参数
      ARGUMENTS+=("$i")
      ;;
  esac

done

# 显示帮助信息
if [[ $HELP ]]; then
  info_bold "Usage: $0 [options] [arguments]"
  echo
  echo "Options:"
  echo "  -h, --help  Show this help message and exit"
  echo
  echo "Arguments:"
  echo "  None        install the latest version of moonbit"
  echo "  [VERSION]   install the specified version of moonbit"
  exit 0
fi

# 检查参数数量
if [[ ${#ARGUMENTS[@]} -gt 1 ]]; then
  error "Too many arguments"
fi

# 检测目标平台（操作系统和架构）
target=''
case $(uname -ms) in
'Darwin x86_64')
    target=darwin-x86_64
    ;;
'Darwin arm64')
    target=darwin-aarch64
    ;;
'Linux x86_64')
    target=linux-x86_64
    ;;
esac

# 检查是否支持当前平台
if [[ -z $target ]]; then
  error "Unsupported platform: $(uname -ms)"
fi

# 如果设置了开发版标志，则使用开发版目标
if [[ -n $MOONBIT_INSTALL_DEV ]]; then
  target="$target-dev"
fi

# 确定要安装的版本（默认为 latest）
version=${ARGUMENTS[0]:-latest}
# 替换版本号中的 '+' 字符为 URL 编码的 '%2B'
version=${version//+/%2B}

# Moonbit CLI 下载地址
CLI_MOONBIT="https://cli.moonbitlang.com"

# 构建下载 URL
moonbit_uri="$CLI_MOONBIT/binaries/$version/moonbit-$target.tar.gz"
core_uri="$CLI_MOONBIT/cores/core-$version.tar.gz"

# 安装路径设置
moon_home="${MOON_HOME:-$HOME/.moon}"  # Moonbit 主目录，默认为 ~/.moon
bin_dir=$moon_home/bin                 # 二进制文件目录
exe=$bin_dir/moon                     # 主可执行文件
moonbit_dest=$HOME/moonbit.tar.gz     # 下载的二进制压缩包
lib_dir=$moon_home/lib                # 库目录
core_dest=$lib_dir/core.tar.gz        # 核心库压缩包

# 检查 moon_home 是否设置
if [[ -z $moon_home ]]; then
  error "MOON_HOME is not set"
fi

# 创建安装目录
mkdir -p "$moon_home" ||
  error "Failed to create directory \"$moon_home\""

# 下载 Moonbit 二进制文件
echo "Downloading moonbit ..."
curl --fail --location --progress-bar --output "$moonbit_dest" "$moonbit_uri" ||
  error "Failed to download moonbit from \"$moonbit_uri\""

# 清理旧的安装文件
rm -rf "$moon_home/bin" ||
  error "Failed to remove existing moonbit binaries"

rm -rf "$moon_home/lib" ||
  error "Failed to remove existing moonbit libraries"

rm -rf "$moon_home/include" ||
  error "Failed to remove existing moonbit includes"

# 解压二进制文件
tar xf "$moonbit_dest" --directory="$moon_home" ||
  error "Failed to extract moonbit to \"$moon_home\""

# 删除下载的压缩包
rm -f "$moonbit_dest" ||
  error "Failed to remove \"$moonbit_dest\""

# 设置二进制文件权限
pushd "$bin_dir" >/dev/null || error "Failed to change directory to \"$bin_dir\""
  for i in *; do
    chmod +x "$i" ||
      error "Failed to make \"$i\" executable"
  done
  chmod +x ./internal/tcc ||
    error "Failed to make tcc executable"
popd >/dev/null || error "Failed to change directory to previous directory"

# 清理旧的核心库
rm -rf "$lib_dir/core" ||
  error "Failed to remove existing core"

# 下载核心库
echo "Downloading core ..."
if [[ $version == "bleeding" ]]; then
  # 如果是开发版，从 GitHub 克隆
  git clone --depth 1 https://github.com/moonbitlang/core.git "$lib_dir/core" ||
    error "Failed to clone core from github"
else
  # 否则从服务器下载
  curl --fail --location --progress-bar --output "$core_dest" "$core_uri" ||
    error "Failed to download core from \"$core_uri\""

  # 解压核心库
tar xf "$core_dest" --directory="$lib_dir" ||
    error "Failed to extract core to \"$lib_dir\""

  # 删除核心库压缩包
rm -f "$core_dest" ||
    error "Failed to remove \"$core_dest\""
fi

# 打包核心库
echo "Bundling core ..."

PATH=$bin_dir $exe bundle --warn-list -a --all --source-dir "$lib_dir"/core ||
  error "Failed to bundle core"

# 如果是开发版，为 LLVM 后端打包
if [[ $version == "bleeding" ]]; then
  PATH=$bin_dir $exe bundle --warn-list -a --target llvm --source-dir "$lib_dir"/core ||
    error "Failed to bundle core for llvm backend"
fi

# 为 WebAssembly GC 后端打包
PATH=$bin_dir $exe bundle --warn-list -a --target wasm-gc --source-dir "$lib_dir"/core --quiet ||
  error "Failed to bundle core to wasm-gc"

# 路径美化函数（将绝对路径中的 $HOME 替换为 ~）
tildify() {
  if [[ $1 = $HOME/* ]]; then
    local replacement=\~/ # 代换字符串
      echo "${1/$HOME\//$replacement}"
    else
      echo "$1"
  fi
}

# 安装成功信息
success "moonbit was installed successfully to $Bold_Green$(tildify "$moon_home")"

echo "To verify the downloaded binaries, check https://www.moonbitlang.com/download#verifying-binaries for instructions."

echo "To know how to add shell completions, run 'moon shell-completion --help'"

# 检查是否已在 PATH 中
if command -v moon >/dev/null 2>&1; then
  echo "Run 'moon help' to get started"
  exit 0
fi

# 配置环境变量
tilde_bin_dir=$(tildify "$bin_dir")
bin_env=${bin_dir//"/\"}
refresh_command=''

# 处理路径中的 $HOME
if [[ $bin_env = $HOME/* ]]; then
  bin_env=${bin_env/$HOME\//\$HOME/}
fi
quoted_new_path_env="\"$bin_env:\$PATH\""

# 根据不同的 shell 配置环境变量
case $(basename "$SHELL") in
fish)
  commands=(
    "fish_add_path \"$bin_env\""
  )

  fish_config=$HOME/.config/fish/config.fish
  tilde_fish_config=$(tildify "$fish_config")

  if [[ -w $fish_config ]]; then
    {
      echo -e "\n# moonbit"
      for cmd in "${commands[@]}"; do
        echo "$cmd"
      done
    } >> "$fish_config"

    info "Added \"$tilde_bin_dir\" to \$PATH in \"$tilde_fish_config\""
    refresh_command="source $tilde_fish_config"
  else
    echo "Manually add the directory to $tilde_fish_config (or similar):"

    for command in "${commands[@]}"; do
      info_bold "  $command"
    done
  fi
  ;;
zsh)
  commands=(
    "export PATH=$quoted_new_path_env"
  )
  zsh_config=$HOME/.zshrc
  tilde_zsh_config=$(tildify "$zsh_config")

  if [[ -w $zsh_config ]]; then
    {
      echo -e '\n# moonbit'

      for command in "${commands[@]}"; do
        echo "$command"
      done
    } >>"$zsh_config"

    info "Added \"$tilde_bin_dir\" to \$PATH in \"$tilde_zsh_config\""

    refresh_command="source $tilde_zsh_config"
  else
    echo "Manually add the directory to $tilde_zsh_config (or similar):"

    for command in "${commands[@]}"; do
      info_bold "  $command"
    done
  fi
  ;;
bash)
  commands=(
    "export PATH=$quoted_new_path_env"
  )
  bash_config=$HOME/.bashrc
  tilde_bash_config=$(tildify "$bash_config")

  if [[ -w $bash_config ]]; then
    {
      echo -e '\n# moonbit'

      for command in "${commands[@]}"; do
        echo "$command"
      done
    } >>"$bash_config"

    info "Added \"$tilde_bin_dir\" to \$PATH in \"$tilde_bash_config\""

    refresh_command="source $tilde_bash_config"
  else
    echo "Manually add the directory to $tilde_bash_config (or similar):"

    for command in "${commands[@]}"; do
      info_bold "  $command"
    done
  fi
  ;;
esac

# 显示启动命令
info "To get started, run:"

if [[ $refresh_command ]]; then
  info_bold "  $refresh_command"
fi

info_bold "  moon help"