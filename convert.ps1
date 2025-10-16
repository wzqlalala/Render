<#
.SYNOPSIS
将指定文件转换为 UTF-8 带 BOM 编码和 LF 行尾格式
#>

# 1. 定义要处理的文件类型
$fileTypes = @("*.h", "*.H", "*.hh", "*.hpp", "*.hxx", "*.cpp", "*.cxx", "*.cc", "*.c", "*.C", "*.inl")

# 2. 获取文件列表
$files = Get-ChildItem -Recurse -Include $fileTypes

# 3. 处理每个文件
foreach ($file in $files) {
    try {
        # 读取文件内容（自动检测原始编码）
        $content = [System.IO.File]::ReadAllText($file.FullName)
        
        # 替换行尾为 LF（Unix 风格）
        $newContent = $content -replace "`r`n", "`n"
        
        # 写入文件（UTF-8 带 BOM）
        [System.IO.File]::WriteAllText(
            $file.FullName,
            $newContent,
            [System.Text.UTF8Encoding]::new($true))  # ✅ 改这里：true 表示带 BOM
        
        Write-Host "✔ 转换成功: $($file.FullName)" -ForegroundColor Green
    }
    catch {
        Write-Host "❌ 转换失败: $($file.FullName) - $_" -ForegroundColor Red
    }
}

Write-Host "✅ 所有文件已转换为 UTF-8 带 BOM 且使用 LF 行尾" -ForegroundColor Cyan
