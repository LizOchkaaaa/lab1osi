param (
    [int]$NumInstances,
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
$multiLogDir = Join-Path -Path $OutputLogDir -ChildPath "multiple"
if (!(Test-Path -Path $multiLogDir)) {
    New-Item -ItemType Directory -Path $multiLogDir | Out-Null
}
$multiLogDir = Resolve-Path $multiLogDir
$ioThptArgs = if ($AdditionalArguments) {
    $AdditionalArguments -join " "
} else {
    ""
}


function Collect-ProcessStats {
    param (
        [System.Diagnostics.Process]$Process,
        [string]$LogFile
    )

    $startTime = Get-Date
    while (-not $Process.HasExited) {
        try {
            $cpuUsage = $Process.TotalProcessorTime.TotalMilliseconds
            $sysTime = $Process.PrivilegedProcessorTime.TotalMilliseconds
            $userTime = $Process.UserProcessorTime.TotalMilliseconds
            $memoryUsage = $Process.WorkingSet64

            $timestamp = (Get-Date).ToString("o")
            "$timestamp, CPU_Usage (ms): $cpuUsage, UserTime (ms): $userTime, SysTime (ms): $sysTime, Memory_Usage (bytes): $memoryUsage" |
                Out-File -Append -FilePath $LogFile

            Start-Sleep -Milliseconds 100
        } catch {
            Write-Host "Error collecting process data for PID $($Process.Id): $_"
            break
        }
    }

    $endTime = Get-Date
    $executionTime = $endTime - $startTime
    Write-Host "Total execution time: $($executionTime.TotalSeconds) seconds"
}

function Start-Instance {
    param (
        [int]$InstanceId,
        [string]$OutputDir,
        [string]$ProgramPath,
        [string]$ProgramArgs
    )

    $instanceDir = Join-Path -Path $OutputDir -ChildPath "instance_$InstanceId"
    if (!(Test-Path -Path $instanceDir)) {
        New-Item -ItemType Directory -Path $instanceDir | Out-Null
    }

    try {
        # Выводим информацию о текущем процессе
        Write-Host "Starting instance $InstanceId with Program: $ProgramPath, Args: $ProgramArgs"

        # Создание путей для логов
        $stdoutLog = Join-Path -Path $instanceDir -ChildPath "stdout.log"
        $stderrLog = Join-Path -Path $instanceDir -ChildPath "stderr.log"
        $statLog = Join-Path -Path $instanceDir -ChildPath "stats.log"

        # Проверим путь программы
        if (-not (Test-Path -Path $ProgramPath)) {
            Write-Host "Error: Program path does not exist: $ProgramPath"
            return
        }

        # Запуск процесса
        $process = Start-Process -FilePath $ProgramPath `
                                  -ArgumentList $ProgramArgs `
                                  -PassThru `
                                  -RedirectStandardOutput $stdoutLog `
                                  -RedirectStandardError $stderrLog `
                                  -WorkingDirectory $OutputDir `
                                  -Wait  # Добавим ожидание завершения процесса

        # Проверка кода завершения
        if ($process.ExitCode -ne 0) {
            Write-Host "Error: Process exited with code $($process.ExitCode)"
            $stderrContent = Get-Content -Path $stderrLog
            Write-Host "Error output from stderr.log: $stderrContent"
        } else {
            Write-Host "Process completed successfully."
        }
    } catch {
        Write-Host "Exception occurred in instance $InstanceId"
    }
}



# Создание фоново запускаемой функции для тестирования программы
function Run-BenchmarkInstances {
    param (
        [string]$ProgramName,
        [int]$InstanceCount,
        [string]$ProgramPath,
        [string]$ProgramArgs,
        [string]$multiLogDir
    )

    if (!$ProgramPath) {
        Write-Host "Warning: $ProgramName program path is empty. Skipping $ProgramName execution."
        return
    }

    $ProgramPath = Resolve-Path $ProgramPath
    Write-Host "Running $InstanceCount instances of $ProgramName benchmark..."

    $jobs = @()
    for ($i = 1; $i -le $InstanceCount; $i++) {
        Write-Host "Creating job for instance $i of $ProgramName"

        $jobs += Start-Job -ScriptBlock {
            param ($id, $logDir, $program, $arguments, $name, $multiLogDir)

            # Диагностика внутри задания
            $LogFile = Join-Path -Path $logDir -ChildPath "job_log_$id.txt"
            Write-Host "LogFile path: $LogFile"
            Write-Output "Job started for $name instance $id" | Out-File -Append -FilePath $LogFile
            Write-Output "Program: $program, Args: $arguments" | Out-File -Append -FilePath $LogFile

            try {
                Start-Instance -InstanceId $id -OutputDir $logDir -ProgramPath $program -ProgramArgs $arguments
            } catch {
                Write-Output "Error in $name instance $id" | Out-File -Append -FilePath $LogFile
            }
        } -ArgumentList $i, $multiLogDir, $ProgramPath, $ProgramArgs, $ProgramName

    }

    # Ожидание завершения всех процессов
    $jobs | ForEach-Object {
        $_ | Wait-Job | Out-Null
        Write-Host "Output from job $($_.InstanceId):"
        Receive-Job -Job $_ | ForEach-Object { Write-Host $_ }
        Remove-Job -Job $_
    }

    Write-Host "All $InstanceCount instances of $ProgramName completed."
}


# Аргументы перед вызовом Run-BenchmarkInstances
Run-BenchmarkInstances -ProgramName "BinSearch" `
    -InstanceCount $NumInstances `
    -ProgramPath $BinSearchProgram `
    -ProgramArgs $BinSearchParam `
    -multiLogDir $multiLogDir

Run-BenchmarkInstances -ProgramName "IoThptWrite" `
    -InstanceCount $NumInstances `
    -ProgramPath $IoThptWriteProgram `
    -ProgramArgs $ioThptArgs `
    -multiLogDir $multiLogDir
