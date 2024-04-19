Write-Host "Rebuilding projects..."
Remove-Item -Path "build" -Recurse -ErrorAction Ignore
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build 

if ($LASTEXITCODE -ne 0) 
{
    Write-Host "Build failed." -ForegroundColor Red
}
else
{
    Write-Host "Build succeeded." -ForegroundColor Green
}