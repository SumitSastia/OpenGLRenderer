$srcDir   = "src"
$buildDir = "build"

$watcher = New-Object System.IO.FileSystemWatcher
$watcher.Path = $srcDir
$watcher.Filter = "*.*"
$watcher.IncludeSubdirectories = $true
$watcher.EnableRaisingEvents = $true

$building = $false

Register-ObjectEvent $watcher Changed -Action {
    if ($building) { return }
    $building = $true

    Write-Host "Change detected → building..."

    cmake --build $buildDir --config Debug

    # signal running exe
    New-Item -Path "restart.flag" -ItemType File -Force | Out-Null

    $building = $false
}

Write-Host "Watching for changes..."
while ($true) { Start-Sleep 1 }
