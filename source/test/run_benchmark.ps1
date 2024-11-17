param (
    [string]$OutputLogDir,
    [string]$BinSearchProgram,
    [string]$BinSearchParam,
    [string]$IoThptWriteProgram,
    [string[]]$AdditionalArguments
)

# Создание каталога для логов
if (!(Test-Path -Path $OutputLogDir)) {
    New-Item -ItemType Directory -Path $OutputLogDir | Out-Null
}

# Конвертируем аргументы в строку для передачи программам
$binSearchParam = $BinSearchParam
$arguments = $AdditionalArguments -join " "

function Collect-ProcessStats {
    param (
        [System.Diagnostics.Process]$Process,
        [string]$LogFile
    )

    # Время начала выполнения
    $startTime = Get-Date
    $cpuUsagePrev = 0
    $sysTimePrev = 0
    $userTimePrev = 0
    $contextSwitchesPrev = 0

    while (-not $Process.HasExited) {
        try {
            # Получение статистики процессора и памяти с использованием Process API
            $cpuUsage = $Process.TotalProcessorTime.TotalMilliseconds
            $sysTime = $Process.PrivilegedProcessorTime.TotalMilliseconds
            $userTime = $Process.UserProcessorTime.TotalMilliseconds
            $memoryUsage = $Process.WorkingSet64
            $contextSwitches = $Process.Handles  # Псевдокод, необходимо будет использовать Performance Counters или WMI для точных данных

            # Расчет времени ожидания
            $waitTime = $cpuUsage - $userTime - $sysTime

            # Расчет USER%, SYS%, WAIT%
            $userPercentage = if ($cpuUsage - $userTime -eq 0) { 0 } else { ($userTime / $cpuUsage) * 100 }
            $sysPercentage = if ($cpuUsage - $sysTime -eq 0) { 0 } else { ($sysTime / $cpuUsage) * 100 }
            $waitPercentage = if ($cpuUsage - $waitTime -eq 0) { 0 } else { ($waitTime / $cpuUsage) * 100 }

            # Запись статистики в лог
            $timestamp = (Get-Date).ToString("o")
            "$timestamp, CPU_Usage (ms): $cpuUsage, UserTime (ms): $userTime, SysTime (ms): $sysTime, WaitTime (ms): $waitTime, CPU_User%: $userPercentage, CPU_Sys%: $sysPercentage, CPU_Wait%: $waitPercentage, Memory_Usage (bytes): $memoryUsage, ContextSwitches: $contextSwitches" |
                Out-File -Append -FilePath $LogFile

            # Задержка между сборами статистики
            Start-Sleep -Milliseconds 100
        } catch {
            Write-Host "Error collecting process data for PID $($Process.Id): $_"
            break
        }
    }

    # Время завершения
    $endTime = Get-Date
    $executionTime = $endTime - $startTime
    Write-Host "Total execution time: $($executionTime.TotalSeconds) seconds"
}


$ProcmonPath = "C:\Program Files\Procmon\Procmon.exe"

# Запуск Process Monitor для записи активности процессов
Write-Host "Starting Process Monitor..."
if (Test-Path -Path $ProcmonPath) {
    $procmonProcess = Start-Process -FilePath $ProcmonPath -ArgumentList "/Quiet", "/Minimized", "/Backingfile", "$OutputLogDir\procmon_capture.pml" -PassThru
    Write-Host "Process Monitor started with PID: $($procmonProcess.Id)"
} else {
    Write-Host "Error: Process Monitor executable not found at $ProcmonPath."
}

# Запуск и сбор метрик для bin-search программы
if ($BinSearchProgram) {
    Write-Host "Running bin-search benchmark..."

    try {
        # Запускаем процесс и перенаправляем вывод для диагностики
        $binSearchProcess = Start-Process -FilePath $BinSearchProgram `
                                           -ArgumentList $binSearchParam `
                                           -PassThru `
                                           -RedirectStandardOutput "$OutputLogDir\bin_search_output.log" `
                                           -RedirectStandardError "$OutputLogDir\bin_search_error.log"

        # Проверяем, стартовал ли процесс
        if ($binSearchProcess -and -not $binSearchProcess.HasExited) {
            Write-Host "Bin-search program running with PID: $($binSearchProcess.Id)"

            # Начинаем сбор статистики в отдельном лог-файле
            Collect-ProcessStats -Process $binSearchProcess -LogFile "$OutputLogDir\bin_search.log"
            Write-Host "Bin-search program finished."
        } else {
            Write-Host "Error: Bin-search program did not start correctly or exited immediately."
            Write-Host "Check $OutputLogDir\bin_search_error.log for details."
        }
    } catch {
        Write-Host "Exception occurred while starting bin-search program: $_"
    }
} else {
    Write-Host "Warning: Bin-search program path is empty. Skipping bin-search execution."
}

if ($IoThptWriteProgram) {
    # Запуск и сбор метрик для IO throughput write программы
    Write-Host "Running IO throughput write benchmark..."
     try {
            # Запускаем процесс и перенаправляем вывод для диагностики
            $IoThptWriteProcess = Start-Process -FilePath $IoThptWriteProgram `
                                               -ArgumentList $AdditionalArguments `
                                               -PassThru `
                                               -RedirectStandardOutput "$OutputLogDir\io_thpt_write_output.log" `
                                               -RedirectStandardError "$OutputLogDir\io_thpt_write_error.log"

            # Проверяем, стартовал ли процесс
            if ($IoThptWriteProcess -and -not $IoThptWriteProcess.HasExited) {
                Write-Host "IoThptWrite program running with PID: $($IoThptWriteProcess.Id)"

                # Начинаем сбор статистики в отдельном лог-файле
                Collect-ProcessStats -Process $IoThptWriteProcess -LogFile "$OutputLogDir\io_thpt_write.log"
                Write-Host "IoThptWrite program finished."
            } else {
                Write-Host "Error: IoThptWrite program did not start correctly or exited immediately."
                Write-Host "Check $OutputLogDir\io_thpt_write_error.log for details."
            }
        } catch {
            Write-Host "Exception occurred while starting io_thpt_write program: $_"
        }
    } else {
        Write-Host "Warning: io_thpt_write program path is empty. Skipping io_thpt_write execution."
    }


if ($procmonProcess -and !$procmonProcess.HasExited) {
    Write-Host "Waiting for Process Monitor to finish..."
    $procmonProcess.WaitForExit()
    Write-Host "Process Monitor finished. FlameGraph equivalent saved to $OutputLogDir\procmon_capture.pml"
}


