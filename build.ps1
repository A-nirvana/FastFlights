# build.ps1

Write-Host "🔧 Building Flight Reservation System..."

# Set include and source folders
$includePath = "include"
$srcFiles = Get-ChildItem -Path "src" -Filter *.cpp | ForEach-Object { $_.FullName }
$mainFile = "main.cpp"
$output = "dist/FlightReservationSystem.exe"

# Full compilation command
g++ -I$includePath $srcFiles $mainFile -o $output

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful!"
} else {
    Write-Host "❌ Build failed!"
}
