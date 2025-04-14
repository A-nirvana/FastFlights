# run.ps1

$exe = "dist/FlightReservationSystem.exe"

if (Test-Path $exe) {
    Write-Host "🚀 Running Flight Reservation System..."
    Start-Process -NoNewWindow -Wait -FilePath ".\$exe"
} else {
    Write-Host "❌ Executable not found. Please build the project first using build.ps1"
}
