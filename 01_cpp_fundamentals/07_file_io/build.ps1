# PowerShell скрипт для компиляции и запуска лабораторных работ
# Использование: .\build.ps1 [номер_лабы | all | clean]

param(
    [Parameter(Position=0)]
    [string]$Target = "all"
)

$CXX = "g++"
$CXXFLAGS = "-std=c++17", "-Wall", "-Wextra", "-O2"

function Write-ColorOutput($ForegroundColor) {
    $fc = $host.UI.RawUI.ForegroundColor
    $host.UI.RawUI.ForegroundColor = $ForegroundColor
    if ($args) {
        Write-Output $args
    }
    $host.UI.RawUI.ForegroundColor = $fc
}

function Build-Lab {
    param([string]$FileName)
    
    $BaseName = [System.IO.Path]::GetFileNameWithoutExtension($FileName)
    $ExeName = "$BaseName.exe"
    
    Write-ColorOutput Yellow "Компиляция $FileName..."
    
    $CompileCmd = "$CXX $($CXXFLAGS -join ' ') $FileName -o $ExeName"
    
    try {
        Invoke-Expression $CompileCmd
        if ($LASTEXITCODE -eq 0) {
            Write-ColorOutput Green "✓ $ExeName успешно скомпилирован"
            return $true
        } else {
            Write-ColorOutput Red "✗ Ошибка компиляции $FileName"
            return $false
        }
    } catch {
        Write-ColorOutput Red "✗ Исключение при компиляции: $_"
        return $false
    }
}

function Run-Lab {
    param([string]$ExeName)
    
    if (Test-Path $ExeName) {
        Write-ColorOutput Cyan "`n=========================================="
        Write-ColorOutput Cyan "Запуск $ExeName"
        Write-ColorOutput Cyan "==========================================`n"
        
        & ".\$ExeName"
        
        Write-ColorOutput Cyan "`n=========================================="
        Write-ColorOutput Green "Завершено: $ExeName"
        Write-ColorOutput Cyan "==========================================`n"
    } else {
        Write-ColorOutput Red "✗ Исполняемый файл $ExeName не найден"
    }
}

function Clean-All {
    Write-ColorOutput Yellow "Очистка..."
    
    # Удаляем исполняемые файлы
    $executables = @(
        "01_basic_text_io.exe",
        "02_binary_io.exe",
        "03_error_handling.exe",
        "04_advanced_techniques.exe"
    )
    
    foreach ($exe in $executables) {
        if (Test-Path $exe) {
            Remove-Item $exe -Force
            Write-ColorOutput Green "✓ Удалён $exe"
        }
    }
    
    # Удаляем файлы данных
    $dataFiles = @(
        "output.txt",
        "students.dat",
        "test_data.txt",
        "config.ini",
        "formatted_output.txt",
        "perf_test*.txt",
        "data.csv",
        "large_file.txt",
        "test_format.txt"
    )
    
    foreach ($pattern in $dataFiles) {
        Get-ChildItem -Filter $pattern -ErrorAction SilentlyContinue | ForEach-Object {
            Remove-Item $_.FullName -Force
            Write-ColorOutput Green "✓ Удалён $($_.Name)"
        }
    }
    
    Write-ColorOutput Green "`n✓ Очистка завершена"
}

# Главная логика
switch ($Target) {
    "clean" {
        Clean-All
    }
    "all" {
        Write-ColorOutput Cyan @"
╔════════════════════════════════════════════════════════╗
║  Лабораторные работы: Файловый ввод/вывод C++         ║
║  Компиляция всех программ                             ║
╚════════════════════════════════════════════════════════╝
"@
        
        $labs = @(
            "01_basic_text_io.cpp",
            "02_binary_io.cpp",
            "03_error_handling.cpp",
            "04_advanced_techniques.cpp"
        )
        
        $successCount = 0
        foreach ($lab in $labs) {
            if (Build-Lab $lab) {
                $successCount++
            }
            Write-Output ""
        }
        
        Write-ColorOutput Cyan "=========================================="
        Write-ColorOutput Green "Успешно скомпилировано: $successCount из $($labs.Count)"
        Write-ColorOutput Cyan "=========================================="
        
        if ($successCount -eq $labs.Count) {
            Write-ColorOutput Yellow "`nДля запуска всех лабораторных: .\build.ps1 run"
            Write-ColorOutput Yellow "Для запуска конкретной: .\01_basic_text_io.exe"
        }
    }
    "run" {
        Write-ColorOutput Cyan @"
╔════════════════════════════════════════════════════════╗
║  Запуск всех лабораторных работ                       ║
╚════════════════════════════════════════════════════════╝
"@
        
        $exes = @(
            "01_basic_text_io.exe",
            "02_binary_io.exe",
            "03_error_handling.exe",
            "04_advanced_techniques.exe"
        )
        
        foreach ($exe in $exes) {
            Run-Lab $exe
        }
    }
    {$_ -match "^\d+$"} {
        $labNum = $_.PadLeft(2, '0')
        $labFiles = Get-ChildItem -Filter "${labNum}_*.cpp"
        
        if ($labFiles) {
            $labFile = $labFiles[0].Name
            if (Build-Lab $labFile) {
                $exeName = [System.IO.Path]::ChangeExtension($labFile, "exe")
                Run-Lab $exeName
            }
        } else {
            Write-ColorOutput Red "✗ Лабораторная работа #$Target не найдена"
        }
    }
    default {
        Write-ColorOutput Yellow @"
Использование: .\build.ps1 [команда]

Команды:
  all         - Компилировать все лабораторные (по умолчанию)
  run         - Запустить все скомпилированные программы
  1-4         - Скомпилировать и запустить конкретную лабораторную
  clean       - Удалить все скомпилированные файлы и данные

Примеры:
  .\build.ps1           # Компилировать всё
  .\build.ps1 1         # Компилировать и запустить лабу #1
  .\build.ps1 run       # Запустить все программы
  .\build.ps1 clean     # Очистить всё

Или компилируйте вручную:
  g++ -std=c++17 -Wall -Wextra -O2 01_basic_text_io.cpp -o 01_basic_text_io.exe
"@
    }
}


