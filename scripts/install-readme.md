### 快速安装

#### Linux/macOS
```bash
curl -fsSL https://ring.wiki/install.sh | bash
# 或指定版本
curl -fsSL https://ring.wiki/install.sh | bash -s v1.0.0
```

#### Windows (PowerShell)
```powershell
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
irm https://ring.wiki/install.ps1 | iex
# 或指定版本
irm https://ring.wiki/install.ps1 | iex -ArgumentList v1.0.0
```